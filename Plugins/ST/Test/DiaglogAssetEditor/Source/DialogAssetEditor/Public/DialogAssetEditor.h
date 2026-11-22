// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

class FDialogAssetEditorModule : public IModuleInterface {
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
private:
    TSharedPtr<FSlateStyleSet> m_StyleSet = nullptr;
    TSharedPtr<struct FDialogPinFactory> m_PinFactory = nullptr;
};