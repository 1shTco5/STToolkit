// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogAssetEditor.h"
#include "DialogAssetAction.h"
#include "IAssetTools.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "FDialogAssetEditorModule"

// 自定义节点颜色
class SDialogGraphPin : public SGraphPin {
public:
    SLATE_BEGIN_ARGS(SDialogGraphPin) {
        }

    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj) {
        SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
    }

protected:
    virtual FSlateColor GetPinColor() const override {
        return FSlateColor(FLinearColor(FColor::Orange));
    }
};

class SDialogStartGraphPin : public SGraphPin {
public:
    SLATE_BEGIN_ARGS(SDialogStartGraphPin) {
        }

    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj) {
        SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
    }

protected:
    virtual FSlateColor GetPinColor() const override {
        return FSlateColor(FLinearColor(FColor::Red));
    }
};

class SDialogEndGraphPin : public SGraphPin {
public:
    SLATE_BEGIN_ARGS(SDialogEndGraphPin) {
        }

    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj) {
        SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
    }

protected:
    virtual FSlateColor GetPinColor() const override {
        return FSlateColor(FLinearColor(FColor::Blue));
    }
};

// 定义子模式为DialogPin时使用DialogGraphPin, 其他情况默认
struct FDialogPinFactory : public FGraphPanelPinFactory {
public:
    virtual ~FDialogPinFactory() {
    }

    virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override {
        if (FName(TEXT("DialogPin")) == Pin->PinType.PinSubCategory) {
            return SNew(SDialogGraphPin, Pin);
        } else if (FName(TEXT("StartPin")) == Pin->PinType.PinSubCategory) {
            return SNew(SDialogStartGraphPin, Pin);
        } else if (FName(TEXT("EndPin")) == Pin->PinType.PinSubCategory) {
            return SNew(SDialogEndGraphPin, Pin);
        }
        return nullptr;
    }
};

void FDialogAssetEditorModule::StartupModule() {
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    IAssetTools& AssetToolsModule = IAssetTools::Get();
    EAssetTypeCategories::Type AssetType = AssetToolsModule.RegisterAdvancedAssetCategory(
        FName(TEXT("Dialog Asset")), FText::FromString("Dialog Asset"));
    TSharedPtr<DialogAssetAction> AssetAction = MakeShareable(new DialogAssetAction(AssetType));
    AssetToolsModule.RegisterAssetTypeActions(AssetAction.ToSharedRef());

    // 设置DialogAsset图标样式
    m_StyleSet = MakeShareable(new FSlateStyleSet("DialogAssetEditorStyle"));
    TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("DialogAssetEditor");
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

    m_StyleSet->Set(TEXT("ClassThumbnail.DialogAsset"), ThumbnailBrush);
    m_StyleSet->Set(TEXT("ClassIcon.DialogAsset"), IconBrush);
    m_StyleSet->Set(TEXT("DialogAssetEditor.AddPinIcon"), AddPinIcon);
    m_StyleSet->Set(TEXT("DialogAssetEditor.RemovePinIcon"), RemovePinIcon);
    m_StyleSet->Set(TEXT("DialogAssetEditor.DeleteNodeIcon"), DeleteNodeIcon);
    FSlateStyleRegistry::RegisterSlateStyle(*m_StyleSet);

    // 设置PinFactory
    m_PinFactory = MakeShareable(new FDialogPinFactory());
    FEdGraphUtilities::RegisterVisualPinFactory(m_PinFactory);
}

void FDialogAssetEditorModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    FSlateStyleRegistry::UnRegisterSlateStyle(*m_StyleSet);
    FEdGraphUtilities::UnregisterVisualPinFactory(m_PinFactory);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDialogAssetEditorModule, DialogAssetEditor)