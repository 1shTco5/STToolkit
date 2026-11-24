#pragma once

#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FSTGraphEditorApp;

class FSTGraphDetailsTabFactory : public FWorkflowTabFactory {
public:
    explicit FSTGraphDetailsTabFactory(const TSharedRef<FSTGraphEditorApp>& InApp);

    static const FName TabId;

protected:
    //~Begin FWorkflowTabFactory Interface
    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
    //~End FWorkflowTabFactory Interface
};
