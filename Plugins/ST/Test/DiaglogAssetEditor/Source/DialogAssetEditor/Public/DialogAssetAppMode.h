#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class DialogAssetAppMode : public FApplicationMode {
public:
    DialogAssetAppMode(TSharedPtr<class DialogAssetEditorApp> App);

    virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
    virtual void PreDeactivateMode() override;
    virtual void PostActivateMode() override;

private:
    TWeakPtr<class DialogAssetEditorApp> m_App;
    FWorkflowAllowedTabSet m_Tabs;
};