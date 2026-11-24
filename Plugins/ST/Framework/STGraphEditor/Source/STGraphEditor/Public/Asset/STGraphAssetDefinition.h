#pragma once

#include "AssetDefinitionDefault.h"
#include "STGraphAssetDefinition.generated.h"

UCLASS(Abstract)
class STGRAPHEDITOR_API USTGraphAssetDefinition : public UAssetDefinitionDefault {
    GENERATED_BODY()

public:
    //~Begin UAssetDefinition Interface
    virtual FLinearColor GetAssetColor() const override final;
    virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
    //~End UAssetDefinition Interface

    //~Begin UAssetDefinitionDefault Interface
    virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
    //~End UAssetDefinitionDefault Interface

    static TConstArrayView<FAssetCategoryPath> GetDefaultAssetCategories();
};
