#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class CustomAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook {
public:
    // ~Begin FWorkflowCentricApplication Interface
    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
    void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost,
                    UObject* InObject);
    // ~End FWorkflowCentricApplication Interface

    // ~Begin FAssetEditorToolKit Interface 
    virtual FName GetToolkitFName() const override { return FName(TEXT("CustomAssetEditorApp")); }
    virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("CustomAssetEditorApp")); }
    virtual FString GetWorldCentricTabPrefix() const override { return TEXT("CustomAssetEditorApp"); }
    virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f); }
    virtual FString GetDocumentationLink() const override { return TEXT("https://github.com/1shTco5"); }

    virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit>& Toolkit) override {
    }

    virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit>& Toolkit) override {
    }

    virtual void OnClose() override;


    // ~End FAssetEditorToolKit Interface 

public:
    class UCustomAsset* GetWorkingAsset() const { return m_WorkingAsset; }
    class UEdGraph* GetWorkingGraph() const { return m_WorkingGraph; }

    void OnGraphChanged(const FEdGraphEditAction& EditAction);

protected:
    void UpdateWorkingAssetFromGraph();
    void UpdateEditorGraphFromWorkingAsset();

private:
    UPROPERTY()
    class UCustomAsset* m_WorkingAsset;

    UPROPERTY()
    class UEdGraph* m_WorkingGraph;

    FDelegateHandle m_GraphChangeListenerHandle;
};