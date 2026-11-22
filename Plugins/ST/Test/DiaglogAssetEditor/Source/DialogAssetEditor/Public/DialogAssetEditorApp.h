#pragma once

#include "CoreMinimal.h"
#include "SGraphPanel.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class DialogAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook {
public:
    // ~Begin FWorkflowCentricApplication Interface
    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
    // ~End FWorkflowCentricApplication Interface

    // ~Begin FAssetEditorToolKit Interface 
    virtual FName GetToolkitFName() const override { return FName(TEXT("DialogAssetEditorApp")); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("DialogAssetEditorApp")); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("DialogAssetEditorApp"); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f); }
    virtual FString GetDocumentationLink() const override { return TEXT("https://github.com/1shTco5"); }
    virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit) override { return; }
    virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit) override { return; }
    virtual void OnClose() override;
    // ~End FAssetEditorToolKit Interface 

public:
    void InitEditor(
        const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject);

    class UDialogAsset* GetWorkingAsset() const { return m_WorkingAsset; }
    class UEdGraph* GetWorkingGraph() const { return m_WorkingGraph; }
    void SetWorkingGraphUI(TSharedPtr<SGraphEditor> WorkingGraphUI) { m_WorkingGraphUI = WorkingGraphUI; }
    void SetSelectedNodeDetailsView(TSharedPtr<class IDetailsView> DetailsView);

    void OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection);
    void OnNodeDetailsViewPropertiesUpdated(const FPropertyChangedEvent& Event);
    void OnWorkingAssetPreSave();

protected:
    void UpdateWorkingAssetFromGraph();
    void UpdateEditorGraphFromWorkingAsset();
    class UDialogGraphNodeBase* GetSelectedNode(const FGraphPanelSelectionSet& Selection) const;

private:
    UPROPERTY()
    class UDialogAsset* m_WorkingAsset;

    UPROPERTY() // Working graph model
    class UEdGraph* m_WorkingGraph;

    // Working graph UI (the slate widget that display the graph) 
    TSharedPtr<SGraphEditor> m_WorkingGraphUI;
    TSharedPtr<class IDetailsView> m_SelectedNodeDetailsView;
};