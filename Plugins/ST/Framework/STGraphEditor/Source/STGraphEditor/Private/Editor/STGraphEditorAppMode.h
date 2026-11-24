#pragma once

#include "WorkflowOrientedApp/ApplicationMode.h"

class FSTGraphEditorApp;

class FSTGraphEditorAppMode : public FApplicationMode {
public:
    explicit FSTGraphEditorAppMode(const TSharedRef<FSTGraphEditorApp>& InApp);
};
