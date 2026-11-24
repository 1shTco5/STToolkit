// Copyright Epic Games, Inc. All Rights Reserved.

#include "STGraphEditor.h"

#include "AssetToolsModule.h"
#include "Brushes/SlateImageBrush.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Asset/STGraphAssetDefinition.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#pragma region IModuleInterface Interface
void FSTGraphEditorModule::StartupModule() {
    IAssetTools& AssetTools = IAssetTools::Get();
    for (const FAssetCategoryPath& Category : USTGraphAssetDefinition::GetDefaultAssetCategories()) {
        AssetTools.RegisterAdvancedAssetCategory(Category.GetCategory(), Category.GetCategoryText());
    }

    RegisterAssetIcons();
}

void FSTGraphEditorModule::ShutdownModule() {
    if (AssetStyle) {
        FSlateStyleRegistry::UnRegisterSlateStyle(*AssetStyle);
        AssetStyle.Reset();
    }
}

#pragma endregion

void FSTGraphEditorModule::RegisterAssetIcons() {
    const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("STGraphEditor"));
    if (!Plugin) {
        return;
    }

    const FString IconPath = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Resources/Icon/FlowGraph.png"));
    if (!FPaths::FileExists(IconPath)) {
        return;
    }

    AssetStyle = MakeShared<FSlateStyleSet>(TEXT("STGraphEditorStyle"));
    AssetStyle->Set(TEXT("ClassIcon.STGraphAsset"), new FSlateImageBrush(IconPath, FVector2D(16.0, 16.0)));
    AssetStyle->Set(TEXT("ClassThumbnail.STGraphAsset"), new FSlateImageBrush(IconPath, FVector2D(64.0, 64.0)));
    FSlateStyleRegistry::RegisterSlateStyle(*AssetStyle);
}

IMPLEMENT_MODULE(FSTGraphEditorModule, STGraphEditor)
