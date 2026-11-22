#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class DialogAssetPropertyTabFactory : public FWorkflowTabFactory {
public:
    DialogAssetPropertyTabFactory(TSharedPtr<class DialogAssetEditorApp> App);

    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
    TWeakPtr<class DialogAssetEditorApp> m_App;
};