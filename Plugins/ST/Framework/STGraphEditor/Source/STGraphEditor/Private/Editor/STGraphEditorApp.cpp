#include "STGraphEditorApp.h"

#include "EdGraph/EdGraph.h"
#include "Graph/STGraphNodeBase.h"
#include "GraphEditAction.h"
#include "IDetailsView.h"
#include "STGraphAsset.h"
#include "STGraphEditorAppMode.h"
#include "STGraphNodeInfo.h"

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
