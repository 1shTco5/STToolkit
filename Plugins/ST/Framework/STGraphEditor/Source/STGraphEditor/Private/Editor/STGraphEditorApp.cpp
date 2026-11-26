#include "STGraphEditorApp.h"

#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphPin.h"
#include "Graph/STGraphNodeBase.h"
#include "Graph/STGraphSchema.h"
#include "GraphEditAction.h"
#include "IDetailsView.h"
#include "Misc/MessageDialog.h"
#include "STGraphAsset.h"
#include "STGraphEditorAppMode.h"
#include "STGraphNodeInfo.h"
#include "UObject/UObjectGlobals.h"

void FSTGraphEditorApp::Init(USTGraphAsset& Asset, EAssetOpenMethod InOpenMethod) {
    const TArray<UObject*> AssetsToEdit = {&Asset};
    InitAssetEditor(EToolkitMode::Standalone, nullptr, GetToolkitFName(), FTabManager::FLayout::NullLayout, true, true,
                    AssetsToEdit, false, false, InOpenMethod);

    ObservedGraph = Asset.GetEditorGraph();
    if (UEdGraph* Graph = GetEditorGraph()) {
        GraphChangedHandle = Graph->AddOnGraphChangedHandler(
            FOnGraphChanged::FDelegate::CreateSP(this, &FSTGraphEditorApp::OnGraphChanged));
    }

    const TSharedRef<FSTGraphEditorAppMode> GraphMode = MakeShared<FSTGraphEditorAppMode>(SharedThis(this));
    AddApplicationMode(GraphMode->GetModeName(), GraphMode);
    SetCurrentMode(GraphMode->GetModeName());
}

UEdGraph* FSTGraphEditorApp::GetEditorGraph() const { return ObservedGraph.Get(); }

void FSTGraphEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection) {
    SelectedNode = Selection.Num() == 1 ? Cast<USTGraphNodeBase>(*Selection.CreateConstIterator()) : nullptr;
    if (const TSharedPtr<IDetailsView> View = DetailsView.Pin()) {
        const USTGraphNodeBase* Node = SelectedNode.Get();
        View->SetObject(Node ? Node->GetNodeInfo() : nullptr);
    }
}

void FSTGraphEditorApp::SetDetailsView(const TSharedRef<IDetailsView>& InDetailsView) {
    if (const TSharedPtr<IDetailsView> PreviousView = DetailsView.Pin()) {
        PreviousView->OnFinishedChangingProperties().RemoveAll(this);
    }
    DetailsView = InDetailsView;
    InDetailsView->OnFinishedChangingProperties().AddSP(SharedThis(this), &FSTGraphEditorApp::OnNodeInfoChanged);
    const USTGraphNodeBase* Node = SelectedNode.Get();
    InDetailsView->SetObject(Node ? Node->GetNodeInfo() : nullptr);
}

void FSTGraphEditorApp::OnNodeInfoChanged(const FPropertyChangedEvent& Event) {
    USTGraphNodeBase* Node = SelectedNode.Get();
    UEdGraph* Graph = Node ? Node->GetGraph() : nullptr;
    if (Graph && Graph->Nodes.Contains(Node)) {
        Node->OnNodeInfoChanged();
        Graph->MarkPackageDirty();
        Graph->NotifyNodeChanged(Node);
    }
}

void FSTGraphEditorApp::OnGraphChanged(const FEdGraphEditAction& Action) {
    if ((Action.Action & GRAPHACTION_RemoveNode) && Action.Nodes.Contains(SelectedNode.Get())) {
        OnGraphSelectionChanged(FGraphPanelSelectionSet());
    }
}

bool FSTGraphEditorApp::UpdateRuntimeGraphFromEditorGraph(USTGraphAsset& Asset, FText& OutError) {
    OutError = FText::GetEmpty();
    const UEdGraph* Graph = Asset.GetEditorGraph();
    if (!Graph) {
        OutError = FText::FromString(TEXT("The asset has no editor graph."));
        return false;
    }

    TArray<const USTGraphNodeBase*> SourceNodes;
    TSet<const UEdGraphNode*> ExportedNodes;
    TSet<FGuid> NodeIds;
    const bool bRequiresRuntimeTypes = !IsEditorOnlyObject(&Asset);
    for (const UEdGraphNode* GraphNode : Graph->Nodes) {
        const USTGraphNodeBase* Node = Cast<USTGraphNodeBase>(GraphNode);
        if (!Node) {
            continue;
        }
        if (!Node->NodeGuid.IsValid() || NodeIds.Contains(Node->NodeGuid)) {
            OutError = FText::Format(FText::FromString(TEXT("Node '{0}' has an invalid or duplicate ID.")),
                                     FText::FromString(Node->GetName()));
            return false;
        }

        const USTGraphNodeInfoBase* Info = Node->GetNodeInfo();
        if (!Info || (bRequiresRuntimeTypes && IsEditorOnlyObject(Info->GetClass()))) {
            OutError = FText::Format(FText::FromString(TEXT("Node '{0}' needs a valid NodeInfo type.")),
                                     FText::FromString(Node->GetName()));
            return false;
        }
        if (const USTGraphMiddleNodeInfo* MiddleInfo = Cast<USTGraphMiddleNodeInfo>(Info)) {
            for (const FSTGraphOutputPinInfo& Output : MiddleInfo->Outputs) {
                const UScriptStruct* PayloadType = Output.Payload.GetScriptStruct();
                if (bRequiresRuntimeTypes && PayloadType && IsEditorOnlyObject(PayloadType)) {
                    OutError = FText::Format(FText::FromString(TEXT("Output '{0}' has an editor-only payload type.")),
                                             Output.DisplayName);
                    return false;
                }
            }
        }

        SourceNodes.Add(Node);
        ExportedNodes.Add(Node);
        NodeIds.Add(Node->NodeGuid);
    }

    FSTRuntimeGraph NewGraph;
    NewGraph.Nodes.Reserve(SourceNodes.Num());
    for (const USTGraphNodeBase* Node : SourceNodes) {
        FSTRuntimeNode& RuntimeNode = NewGraph.Nodes.AddDefaulted_GetRef();
        RuntimeNode.NodeId = Node->NodeGuid;
        RuntimeNode.Role = Node->GetNodeRole();

        TSet<FGuid> PinIds;
        for (const UEdGraphPin* Pin : Node->Pins) {
            if (!Pin || Pin->Direction != EGPD_Output) {
                continue;
            }
            if (!Pin->PinId.IsValid() || PinIds.Contains(Pin->PinId)) {
                OutError = FText::Format(FText::FromString(TEXT("Output '{0}' has an invalid or duplicate ID.")),
                                         Pin->GetDisplayName());
                return false;
            }
            if (Pin->PinType.PinCategory == USTGraphSchema::FlowPinCategory && Pin->LinkedTo.Num() > 1) {
                OutError = FText::Format(
                    FText::FromString(TEXT("Node '{0}' output '{1}' has multiple connections. Keep only one.")),
                    Node->GetNodeTitle(ENodeTitleType::ListView), Pin->GetDisplayName());
                return false;
            }

            PinIds.Add(Pin->PinId);
            FSTRuntimeOutputPin& Output = RuntimeNode.OutputPins.AddDefaulted_GetRef();
            Output.PinId = Pin->PinId;
            for (const UEdGraphPin* TargetPin : Pin->LinkedTo) {
                const UEdGraphNode* Target = TargetPin ? TargetPin->GetOwningNodeUnchecked() : nullptr;
                if (!Target || !ExportedNodes.Contains(Target) || TargetPin->Direction != EGPD_Input ||
                    !Target->Pins.Contains(TargetPin)) {
                    OutError = FText::Format(FText::FromString(TEXT("Output '{0}' has an invalid connection.")),
                                             Pin->GetDisplayName());
                    return false;
                }
                Output.TargetNodeIds.Add(Target->NodeGuid);
            }
        }
    }

    for (int32 Index = 0; Index < SourceNodes.Num(); ++Index) {
        NewGraph.Nodes[Index].NodeInfo = DuplicateObject(SourceNodes[Index]->GetNodeInfo(), &Asset);
    }
    Asset.RuntimeGraph = MoveTemp(NewGraph);
    return true;
}

#pragma region FWorkflowCentricApplication Interface
void FSTGraphEditorApp::OnClose() {
    if (const TSharedPtr<IDetailsView> View = DetailsView.Pin()) {
        View->OnFinishedChangingProperties().RemoveAll(this);
    }
    if (UEdGraph* Graph = GetEditorGraph()) {
        Graph->RemoveOnGraphChangedHandler(GraphChangedHandle);
    }
    FWorkflowCentricApplication::OnClose();
}
#pragma endregion

#pragma region FAssetEditorToolkit Interface
void FSTGraphEditorApp::SaveAsset_Execute() {
    const UEdGraph* Graph = GetEditorGraph();
    USTGraphAsset* Asset = Graph ? Cast<USTGraphAsset>(Graph->GetOuter()) : nullptr;
    if (!Asset || GetOpenMethod() != EAssetOpenMethod::Edit) {
        return;
    }

    FText Error;
    if (!UpdateRuntimeGraphFromEditorGraph(*Asset, Error)) {
        FMessageDialog::Open(EAppMsgType::Ok, Error);
        return;
    }

    Asset->MarkPackageDirty();
    FWorkflowCentricApplication::SaveAsset_Execute();
}
#pragma endregion
