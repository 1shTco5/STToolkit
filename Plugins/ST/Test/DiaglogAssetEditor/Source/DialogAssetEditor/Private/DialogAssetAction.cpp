#include "DialogAssetAction.h"
#include "DialogAsset.h"
#include "DialogAssetEditorApp.h"

DialogAssetAction::DialogAssetAction(EAssetTypeCategories::Type Category) {
    m_Category = Category;
}

FText DialogAssetAction::GetName() const {
    return FText::FromString(TEXT("ST Dialog Asset"));
}

FColor DialogAssetAction::GetTypeColor() const {
    return FColor::Orange;
}

UClass* DialogAssetAction::GetSupportedClass() const {
    return UDialogAsset::StaticClass();
}

void DialogAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                        const EAssetTypeActivationOpenedMethod OpenedMethod,
                                        TSharedPtr<IToolkitHost> EditWithinLevelEditor) {
    EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
    for (UObject* Object : InObjects) {
        if (UDialogAsset* DialogAsset = Cast<UDialogAsset>(Object)) {
            TSharedRef<DialogAssetEditorApp> Editor(new DialogAssetEditorApp());
            Editor->InitEditor(Mode, EditWithinLevelEditor, DialogAsset);
        }
    }
}

uint32 DialogAssetAction::GetCategories() {
    return m_Category;
}