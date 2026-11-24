#pragma once

#include "Factories/Factory.h"
#include "Templates/SubclassOf.h"
#include "STGraphAssetFactory.generated.h"

class UEdGraphSchema;

UCLASS(Abstract)
class STGRAPHEDITOR_API USTGraphAssetFactory : public UFactory {
    GENERATED_BODY()

public:
    //~Begin UFactory Interface
    virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                      UObject* Context, FFeedbackContext* Warn) override;
    //~End UFactory Interface

protected:
    // Concrete factories set SupportedClass and opt into the New Asset menu with bCreateNew.
    virtual TSubclassOf<UEdGraphSchema> GetGraphSchemaClass() const
        PURE_VIRTUAL(USTGraphAssetFactory::GetGraphSchemaClass, return nullptr;);
};
