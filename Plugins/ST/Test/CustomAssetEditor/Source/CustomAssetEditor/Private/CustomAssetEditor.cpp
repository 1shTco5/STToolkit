// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAssetEditor.h"
#include "CustomAssetAction.h"
#include "IAssetTools.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "FCustomAssetEditorModule"

// 自定义节点颜色
class SCustomGraphPin : public SGraphPin {
public:
    SLATE_BEGIN_ARGS(SCustomGraphPin) {
        }

    SLATE_END_ARGS()

    void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) {
        SGraphPin::Construct(SGraphPin::FArguments(), inGraphPinObj);
    }

protected:
    virtual FSlateColor GetPinColor() const override {
        return FSlateColor(FLinearColor(255.f, 128.f, 0.f));
    }
};

// 定义子模式为CustomPin时使用CustomGraphPin, 其他情况默认
struct FCustomPinFactory : public FGraphPanelPinFactory {
public:
    virtual ~FCustomPinFactory() {
    }

    virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* pin) const override {
        if (FName(TEXT("CustomPin")) == pin->PinType.PinSubCategory) {
            return SNew(SCustomGraphPin, pin);
        }
        return nullptr;
    }
};

void FCustomAssetEditorModule::StartupModule() {
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    IAssetTools& AssetToolsModule = IAssetTools::Get();
    EAssetTypeCategories::Type AssetType = AssetToolsModule.RegisterAdvancedAssetCategory(
        FName(TEXT("Custom Asset")), FText::FromString("Custom Asset"));
    TSharedPtr<CustomAssetAction> AssetAction = MakeShareable(new CustomAssetAction(AssetType));
    AssetToolsModule.RegisterAssetTypeActions(AssetAction.ToSharedRef());

    // 设置CustomAsset图标样式
    m_StyleSet = MakeShareable(new FSlateStyleSet("CustomAssetEditorStyle"));
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("CustomAssetEditor");
    FString ContentDir = Plugin->GetContentDir();
    m_StyleSet->SetContentRoot(ContentDir);

    FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(
        m_StyleSet->RootToContentDir(TEXT("PropertySheet"), TEXT(".png")), FVector2D(128, 128));
    FSlateImageBrush* IconBrush = new FSlateImageBrush(
        m_StyleSet->RootToContentDir(TEXT("PropertySheet"), TEXT(".png")), FVector2D(128, 128));
    FSlateImageBrush* AddPinIcon = new FSlateImageBrush(
        m_StyleSet->RootToContentDir(TEXT("AddPin"), TEXT(".png")), FVector2D(128, 128));
    FSlateImageBrush* RemovePinIcon = new FSlateImageBrush(
        m_StyleSet->RootToContentDir(TEXT("RemovePin"), TEXT(".png")), FVector2D(128, 128));
    FSlateImageBrush* DeleteNodeIcon = new FSlateImageBrush(
        m_StyleSet->RootToContentDir(TEXT("DeleteNode"), TEXT(".png")), FVector2D(128, 128));

    m_StyleSet->Set(TEXT("ClassThumbnail.CustomAsset"), ThumbnailBrush);
    m_StyleSet->Set(TEXT("ClassIcon.CustomAsset"), IconBrush);
    m_StyleSet->Set(TEXT("CustomAssetEditor.AddPinIcon"), AddPinIcon);
    m_StyleSet->Set(TEXT("CustomAssetEditor.RemovePinIcon"), RemovePinIcon);
    m_StyleSet->Set(TEXT("CustomAssetEditor.DeleteNodeIcon"), DeleteNodeIcon);
    FSlateStyleRegistry::RegisterSlateStyle(*m_StyleSet);

    // 设置PinFactory
    m_PinFactory = MakeShareable(new FCustomPinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(m_PinFactory);
}

void FCustomAssetEditorModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    FSlateStyleRegistry::UnRegisterSlateStyle(*m_StyleSet);
    FEdGraphUtilities::UnregisterVisualPinFactory(m_PinFactory);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCustomAssetEditorModule, CustomAssetEditor)