#include "DialogAssetEditorApp.h"

#include "DialogAssetAppMode.h"
#include "DialogAsset.h"
#include "DialogEndGraphNode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "DialogGraphSchema.h"
#include "DialogRuntimeGraph.h"
#include "DialogGraphNode.h"
#include "DialogStartGraphNode.h"
#include "DialogNodeInfo.h"

DEFINE_LOG_CATEGORY_STATIC(DialogAssetEditorAppSub, Log, All)

void DialogAssetEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) {
    FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void DialogAssetEditorApp::InitEditor(const EToolkitMode::Type Mode,
                                      const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject) {
    TArray<UObject*> ObjectsToEdit;
    ObjectsToEdit.Add(InObject);

    m_WorkingAsset = Cast<UDialogAsset>(InObject);
    m_WorkingAsset->SetPreSaveListener([this]() {
        OnWorkingAssetPreSave();
    });

    m_WorkingGraph = FBlueprintEditorUtils::CreateNewGraph(
        m_WorkingAsset, NAME_None, UEdGraph::StaticClass(), UDialogGraphSchema::StaticClass());

    InitAssetEditor(
        Mode,
        InitToolkitHost,
        TEXT("DialogAssetEditor"),
        FTabManager::FLayout::NullLayout,
        true,
        true,
        ObjectsToEdit);

    AddApplicationMode(TEXT("DialogAssetAppMode"), MakeShareable(new DialogAssetAppMode(SharedThis(this))));

    SetCurrentMode(TEXT("DialogAssetAppMode"));

    UpdateEditorGraphFromWorkingAsset();
}

void DialogAssetEditorApp::OnClose() {
    UpdateWorkingAssetFromGraph();
    m_WorkingAsset->SetPreSaveListener(nullptr);
    FAssetEditorToolkit::OnClose();
}

void DialogAssetEditorApp::SetSelectedNodeDetailsView(TSharedPtr<class IDetailsView> DetailsView) {
    m_SelectedNodeDetailsView = DetailsView;
    m_SelectedNodeDetailsView->OnFinishedChangingProperties().AddRaw(
        this, &DialogAssetEditorApp::OnNodeDetailsViewPropertiesUpdated);
}

void DialogAssetEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection) {
    if (UDialogGraphNodeBase* SelectedNode = GetSelectedNode(Selection)) {
        m_SelectedNodeDetailsView->SetObject(SelectedNode->GetNodeInfo());
    } else {
        m_SelectedNodeDetailsView->SetObject(nullptr);
    }
}

void DialogAssetEditorApp::OnNodeDetailsViewPropertiesUpdated(const FPropertyChangedEvent& Event) {
    if (m_WorkingGraphUI) {
        if (UDialogGraphNodeBase* SelectedNode = GetSelectedNode(m_WorkingGraphUI->GetSelectedNodes())) {
            SelectedNode->OnPropertiesChanged();
        }
        m_WorkingGraphUI->NotifyGraphChanged();
    }
}

void DialogAssetEditorApp::OnWorkingAssetPreSave() {
    UpdateWorkingAssetFromGraph();
}

void DialogAssetEditorApp::UpdateWorkingAssetFromGraph() {
    if (m_WorkingAsset == nullptr || m_WorkingGraph == nullptr) return;

    // Update the state we need into our saveable format
    UDialogRuntimeGraph* RuntimeGraph = NewObject<UDialogRuntimeGraph>(m_WorkingAsset);
    m_WorkingAsset->Graph = RuntimeGraph;

    TArray<std::pair<FGuid, FGuid>> Connections;
    TMap<FGuid, UDialogRuntimePin*> IdToPinMap;
    // First create all the nodes/pins and record the Connections
    for (UEdGraphNode* UINode : m_WorkingGraph->Nodes) {
        UDialogRuntimeNode* RuntimeNode = NewObject<UDialogRuntimeNode>(RuntimeGraph);
        RuntimeNode->Position = FVector2D(UINode->NodePosX, UINode->NodePosY);

        for (UEdGraphPin* UIPin : UINode->Pins) {
            UDialogRuntimePin* RuntimePin = NewObject<UDialogRuntimePin>(RuntimeNode);
            RuntimePin->PinName = UIPin->PinName;
            RuntimePin->PinId = UIPin->PinId;
            RuntimePin->Parent = RuntimeNode;

            // Only record the output side of the connection since this is a directed graph
            if (UIPin->HasAnyConnections() && UIPin->Direction == EGPD_Output) {
                // Only 1 connection is allowed so just take the first one
                std::pair<FGuid, FGuid> Connection = std::make_pair(UIPin->PinId, UIPin->LinkedTo[0]->PinId);
                Connections.Add(Connection);
            }

            IdToPinMap.Add(UIPin->PinId, RuntimePin);
            if (UIPin->Direction == EGPD_Input) {
                RuntimeNode->InputPin = RuntimePin;
            } else {
                RuntimeNode->OutputPins.Add(RuntimePin);
            }
        }

        UDialogGraphNodeBase* UIDialogNode = Cast<UDialogGraphNodeBase>(UINode);
        RuntimeNode->NodeType = UIDialogNode->GetDialogNodeType();
        RuntimeNode->NodeInfo = UIDialogNode->GetNodeInfo();

        RuntimeGraph->Nodes.Add(RuntimeNode);
    }

    // Now make all the connections
    for (std::pair<FGuid, FGuid> Connection : Connections) {
        UDialogRuntimePin* Pin1 = IdToPinMap[Connection.first];
        UDialogRuntimePin* Pin2 = IdToPinMap[Connection.second];
        Pin1->Connection = Pin2;
    }
}

void DialogAssetEditorApp::UpdateEditorGraphFromWorkingAsset() {
    if (m_WorkingAsset->Graph == nullptr) {
        m_WorkingGraph->GetSchema()->CreateDefaultNodesForGraph(*m_WorkingGraph);
        return;
    }

    // Create all the nodes/pins first
    TArray<std::pair<FGuid, FGuid>> Connections;
    TMap<FGuid, UEdGraphPin*> IdToPinMap;
    for (UDialogRuntimeNode* RuntimeNode : m_WorkingAsset->Graph->Nodes) {
        UDialogGraphNodeBase* NewNode = nullptr;
        switch (RuntimeNode->NodeType) {
            case EDialogNodeType::StartNode:
                NewNode = NewObject<UDialogStartGraphNode>(m_WorkingGraph);
                break;
            case EDialogNodeType::DialogNode:
                NewNode = NewObject<UDialogGraphNode>(m_WorkingGraph);
                break;
            case EDialogNodeType::EndNode:
                NewNode = NewObject<UDialogEndGraphNode>(m_WorkingGraph);
                break;
            default:
                UE_LOG(DialogAssetEditorAppSub, Error,
                       TEXT("DialogAssetEditorApp::UpdateEditorGraphFromWorkingAsset: Unknow node type"));
                break;
        }
        if (!NewNode) continue;

        NewNode->CreateNewGuid();
        NewNode->NodePosX = RuntimeNode->Position.X;
        NewNode->NodePosY = RuntimeNode->Position.Y;

        if (RuntimeNode->NodeInfo) {
            NewNode->SetNodeInfo(DuplicateObject(RuntimeNode->NodeInfo, RuntimeNode));
        } else {
            NewNode->InitNodeInfo(NewNode);
        }

        if (RuntimeNode->InputPin) {
            UDialogRuntimePin* Pin = RuntimeNode->InputPin;
            UEdGraphPin* UIPin = NewNode->CreateDialogPin(EGPD_Input, Pin->PinName);
            UIPin->PinId = Pin->PinId;

            if (Pin->Connection) {
                Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
            }
            IdToPinMap.Add(Pin->PinId, UIPin);
        }

        for (UDialogRuntimePin* Pin : RuntimeNode->OutputPins) {
            UEdGraphPin* UIPin = NewNode->CreateDialogPin(EGPD_Output, Pin->PinName);
            UIPin->PinId = Pin->PinId;

            if (Pin->Connection != nullptr) {
                Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
            }
            IdToPinMap.Add(Pin->PinId, UIPin);
        }

        m_WorkingGraph->AddNode(NewNode, true, true);
    }

    for (std::pair<FGuid, FGuid> Connection : Connections) {
        UEdGraphPin* FromPin = IdToPinMap[Connection.first];
        UEdGraphPin* ToPin = IdToPinMap[Connection.second];
        FromPin->LinkedTo.Add(ToPin);
        ToPin->LinkedTo.Add(FromPin);
    }
}

UDialogGraphNodeBase* DialogAssetEditorApp::GetSelectedNode(const FGraphPanelSelectionSet& Selection) const {
    for (UObject* Obj : Selection) {
        if (UDialogGraphNodeBase* Node = Cast<UDialogGraphNodeBase>(Obj)) {
            return Node;
        }
    }
    return nullptr;
}