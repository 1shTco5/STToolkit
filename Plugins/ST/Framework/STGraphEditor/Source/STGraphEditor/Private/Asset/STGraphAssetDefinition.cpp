#include "Asset/STGraphAssetDefinition.h"

#include "Editor/STGraphEditorApp.h"
#include "STGraphAsset.h"

#pragma region UAssetDefinition Interface
FLinearColor USTGraphAssetDefinition::GetAssetColor() const { return FLinearColor(FColor::Orange); }

TConstArrayView<FAssetCategoryPath> USTGraphAssetDefinition::GetAssetCategories() const {
    return GetDefaultAssetCategories();
}

#pragma endregion

#pragma region UAssetDefinitionDefault Interface
EAssetCommandResult USTGraphAssetDefinition::OpenAssets(const FAssetOpenArgs& OpenArgs) const {
    EAssetCommandResult Result = EAssetCommandResult::Unhandled;
    for (USTGraphAsset* Asset : OpenArgs.LoadObjects<USTGraphAsset>()) {
        if (Asset->GetEditorGraph()) {
            const TSharedRef<FSTGraphEditorApp> App = MakeShared<FSTGraphEditorApp>();
            App->Init(*Asset, OpenArgs.OpenMethod);
            Result = EAssetCommandResult::Handled;
        }
    }
    return Result;
}

#pragma endregion

TConstArrayView<FAssetCategoryPath> USTGraphAssetDefinition::GetDefaultAssetCategories() {
    static const FAssetCategoryPath Categories[] = {FAssetCategoryPath(FText::FromString(TEXT("ST Graph Editor")))};
    return Categories;
}
