#pragma once

#include "CoreMinimal.h"
#include "STDialogueAsset.h"
#include "Asset/STGraphAssetDefinition.h"
#include "STDialogueAssetDefinition.generated.h"

UCLASS()
class UAssetDefinition_STDialogueAsset : public USTGraphAssetDefinition {
    GENERATED_BODY()

public:
    //~Begin UAssetDefinition Interface
    virtual FText GetAssetDisplayName() const override { return FText::FromString("ST Dialogue"); }
    virtual TSoftClassPtr<UObject> GetAssetClass() const override { return USTDialogueAsset::StaticClass(); };
    //~End UAssetDefinition Interface
};