// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAssetEditor.h"
#include "CustomAssetAction.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FCustomAssetEditorModule"

void FCustomAssetEditorModule::StartupModule() {
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    IAssetTools& AssetToolsModule = IAssetTools::Get();
    EAssetTypeCategories::Type AssetType = AssetToolsModule.RegisterAdvancedAssetCategory(
        FName(TEXT("Custom Asset")), FText::FromString("Custom Asset"));
    TSharedPtr<CustomAssetAction> AssetAction = MakeShareable(new CustomAssetAction(AssetType));
    AssetToolsModule.RegisterAssetTypeActions(AssetAction.ToSharedRef());
}

void FCustomAssetEditorModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCustomAssetEditorModule, CustomAssetEditor)