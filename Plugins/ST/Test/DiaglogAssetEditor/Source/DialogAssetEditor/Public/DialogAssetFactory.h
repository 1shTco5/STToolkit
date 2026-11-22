#pragma once

#include "CoreMinimal.h"
#include "DialogAssetFactory.generated.h"

UCLASS()
class UDialogAssetFactory : public UFactory {
    GENERATED_BODY()

public:
    UDialogAssetFactory(const FObjectInitializer& ObjectInitializer);

    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                      UObject* Context, FFeedbackContext* Warn) override;
    virtual bool CanCreateNew() const override;
};