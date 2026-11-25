#pragma once

#include "CoreMinimal.h"
#include "Asset/STGraphAssetFactory.h"
#include "STDialogueAssetFactory.generated.h"

UCLASS()
class USTDialogueAssetFactory : public USTGraphAssetFactory {
    GENERATED_BODY()

public:
    USTDialogueAssetFactory();

protected:
    //~Begin USTGraphAssetFactory Interface
    virtual TSubclassOf<UEdGraphSchema> GetGraphSchemaClass() const override;
    //~End USTGraphAssetFactory Interface
};