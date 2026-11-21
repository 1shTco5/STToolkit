#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class CustomAssetPrimaryTabFactory : public FWorkflowTabFactory {
public:
    CustomAssetPrimaryTabFactory(TSharedPtr<class CustomAssetEditorApp> App);

    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
    TWeakPtr<class CustomAssetEditorApp> m_App;
};