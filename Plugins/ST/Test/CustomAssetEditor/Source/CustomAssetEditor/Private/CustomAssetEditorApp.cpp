#include "CustomAssetEditorApp.h"

#include "CustomAssetAppMode.h"
#include "CustomAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "CustomGraphSchema.h"
#include "CustomRuntimeGraph.h"
#include "CustomGraphNode.h"

void CustomAssetEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) {
    FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void CustomAssetEditorApp::InitEditor(const EToolkitMode::Type Mode,
                                      const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject) {
    TArray<UObject*> ObjectsToEdit;
    ObjectsToEdit.Add(InObject);

    m_WorkingAsset = Cast<UCustomAsset>(InObject);
    m_WorkingGraph = FBlueprintEditorUtils::CreateNewGraph(
        m_WorkingAsset, NAME_None, UEdGraph::StaticClass(), UCustomGraphSchema::StaticClass());

    InitAssetEditor(
        Mode,
        InitToolkitHost,
        TEXT("CustomAssetEditor"),
        FTabManager::FLayout::NullLayout,
        true,
        true,
        ObjectsToEdit);

    AddApplicationMode(TEXT("CustomAssetAppMode"), MakeShareable(new CustomAssetAppMode(SharedThis(this))));

    SetCurrentMode(TEXT("CustomAssetAppMode"));

    UpdateEditorGraphFromWorkingAsset();

    m_GraphChangeListenerHandle = m_WorkingGraph->AddOnGraphChangedHandler(
        FOnGraphChanged::FDelegate::CreateSP(this, &CustomAssetEditorApp::OnGraphChanged));
}

void CustomAssetEditorApp::OnClose() {
    UpdateWorkingAssetFromGraph();
    m_WorkingGraph->RemoveOnGraphChangedHandler(m_GraphChangeListenerHandle);
    FAssetEditorToolkit::OnClose();
}

void CustomAssetEditorApp::OnGraphChanged(const FEdGraphEditAction& EditAction) {
    UpdateWorkingAssetFromGraph();
}

void CustomAssetEditorApp::UpdateWorkingAssetFromGraph() {
    if (m_WorkingAsset == nullptr || m_WorkingGraph == nullptr) return;

    // Update the state we need into our saveable format
    UCustomRuntimeGraph* RuntimeGraph = NewObject<UCustomRuntimeGraph>(m_WorkingAsset);
    m_WorkingAsset->Graph = RuntimeGraph;

    TArray<std::pair<FGuid, FGuid>> Connections;
    TMap<FGuid, UCustomRuntimePin*> IdToPinMap;
    // First create all the nodes/pins and record the Connections
    for (UEdGraphNode* UINode : m_WorkingGraph->Nodes) {
        UCustomRuntimeNode* RuntimeNode = NewObject<UCustomRuntimeNode>(RuntimeGraph);
        RuntimeNode->Position = FVector2D(UINode->NodePosX, UINode->NodePosY);

        for (UEdGraphPin* UIPin : UINode->Pins) {
            UCustomRuntimePin* RuntimePin = NewObject<UCustomRuntimePin>(RuntimeNode);
            RuntimePin->PinName = UIPin->PinName;
            RuntimePin->PinId = UIPin->PinId;

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

        RuntimeGraph->Nodes.Add(RuntimeNode);
    }

    // Now make all the connections
    for (std::pair<FGuid, FGuid> Connection : Connections) {
        UCustomRuntimePin* Pin1 = IdToPinMap[Connection.first];
        UCustomRuntimePin* Pin2 = IdToPinMap[Connection.second];
        Pin1->Connection = Pin2;
    }
}

void CustomAssetEditorApp::UpdateEditorGraphFromWorkingAsset() {
    if (m_WorkingAsset->Graph == nullptr) {
        return;
    }

    // Create all the nodes/pins first
    TArray<std::pair<FGuid, FGuid>> Connections;
    TMap<FGuid, UEdGraphPin*> IdToPinMap;
    for (UCustomRuntimeNode* RuntimeNode : m_WorkingAsset->Graph->Nodes) {
        UCustomGraphNode* NewNode = NewObject<UCustomGraphNode>(m_WorkingGraph);
        NewNode->CreateNewGuid();
        NewNode->NodePosX = RuntimeNode->Position.X;
        NewNode->NodePosY = RuntimeNode->Position.Y;

        if (RuntimeNode->InputPin != nullptr) {
            UCustomRuntimePin* Pin = RuntimeNode->InputPin;
            UEdGraphPin* UIPin = NewNode->CreateCustomPin(EGPD_Input, Pin->PinName);
            UIPin->PinId = Pin->PinId;

            if (Pin->Connection != nullptr) {
                Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
            }
            IdToPinMap.Add(Pin->PinId, UIPin);
        }

        for (UCustomRuntimePin* Pin : RuntimeNode->OutputPins) {
            UEdGraphPin* UIPin = NewNode->CreateCustomPin(EGPD_Output, Pin->PinName);
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