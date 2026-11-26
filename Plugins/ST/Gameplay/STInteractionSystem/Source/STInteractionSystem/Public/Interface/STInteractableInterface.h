#pragma once

#include "UObject/Interface.h"
#include "STInteractionContext.h"
#include "STInteractableInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class STINTERACTIONSYSTEM_API USTInteractableInterface : public UInterface {
    GENERATED_BODY()
};

class STINTERACTIONSYSTEM_API ISTInteractableInterface {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    bool CanInteract(const FSTInteractionContext& Context) const;

    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void OnInteractionCompleted(const FSTInteractionContext& Context);
};
