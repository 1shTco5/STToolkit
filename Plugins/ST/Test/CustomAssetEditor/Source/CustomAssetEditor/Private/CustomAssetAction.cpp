#include "CustomAssetAction.h"
#include "CustomAsset.h"
#include "CustomAssetEditorApp.h"

CustomAssetAction::CustomAssetAction(EAssetTypeCategories::Type Category) {
    m_Category = Category;
}

FText CustomAssetAction::GetName() const {
    return FText::FromString(TEXT("ST Custom Asset"));
}

FColor CustomAssetAction::GetTypeColor() const {
    return FColor::Orange;
}

UClass* CustomAssetAction::GetSupportedClass() const {
    return UCustomAsset::StaticClass();
}

void CustomAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                        const EAssetTypeActivationOpenedMethod OpenedMethod,
                                        TSharedPtr<IToolkitHost> EditWithinLevelEditor) {
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (UObject* Object : InObjects) {
        if (UCustomAsset* CustomAsset = Cast<UCustomAsset>(Object)) {
            TSharedRef<CustomAssetEditorApp> Editor(new CustomAssetEditorApp());
            Editor->InitEditor(Mode, EditWithinLevelEditor, CustomAsset);
        }
    }
}

uint32 CustomAssetAction::GetCategories() {
    return m_Category;
}