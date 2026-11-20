#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class CustomAssetAppMode : public FApplicationMode {
public:
    CustomAssetAppMode(TSharedPtr<class CustomAssetEditorApp> App);

    virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
    virtual void PreDeactivateMode() override;
    virtual void PostActivateMode() override;

private:
    TWeakPtr<class CustomAssetEditorApp> m_App;
    FWorkflowAllowedTabSet m_Tabs;
};