// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSlateStyleSet;

class FSTGraphEditorModule : public IModuleInterface {
public:
    //~Begin IModuleInterface Interface
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //~End IModuleInterface Interface

private:
    void RegisterAssetIcons();

    TSharedPtr<FSlateStyleSet> AssetStyle;
};
