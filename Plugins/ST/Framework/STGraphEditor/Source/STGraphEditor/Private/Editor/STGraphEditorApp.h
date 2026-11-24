#pragma once

#include "GraphEditor.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class IDetailsView;
class UEdGraph;
class USTGraphAsset;
class USTGraphNodeBase;
struct FEdGraphEditAction;
struct FPropertyChangedEvent;

class FSTGraphEditorApp : public FWorkflowCentricApplication {
public:
    void Init(USTGraphAsset& Asset, EAssetOpenMethod InOpenMethod);
    UEdGraph* GetEditorGraph() const;
    void OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection);
    void SetDetailsView(const TSharedRef<IDetailsView>& InDetailsView);

    //~Begin FWorkflowCentricApplication Interface
    virtual void OnClose() override;
    //~End FWorkflowCentricApplication Interface

    //~Begin FAssetEditorToolkit Interface
    virtual FName GetToolkitFName() const override { return TEXT("STGraphEditor"); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("ST Graph Editor")); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("ST Graph"); }
    //~End FAssetEditorToolkit Interface

    //~Begin FBaseToolkit Interface
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(FColor::Orange); }
    //~End FBaseToolkit Interface

private:
    void OnGraphChanged(const FEdGraphEditAction& Action);
    void OnNodeInfoChanged(const FPropertyChangedEvent& Event);

    TWeakPtr<IDetailsView> DetailsView;
    TWeakObjectPtr<USTGraphNodeBase> SelectedNode;
    TWeakObjectPtr<UEdGraph> ObservedGraph;
    FDelegateHandle GraphChangedHandle;
};
