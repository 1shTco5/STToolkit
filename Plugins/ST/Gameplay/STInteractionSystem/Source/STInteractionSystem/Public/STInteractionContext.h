#pragma once

#include "CoreMinimal.h"
#include "STInteractionContext.generated.h"

class USTInteractorComponent;
class USTInteractableComponent;

USTRUCT(BlueprintType)
struct STINTERACTIONSYSTEM_API FSTInteractionContext {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<USTInteractorComponent> InteractorComponent = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<USTInteractableComponent> InteractableComponent = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<AActor> InteractorActor = nullptr;

    UPROPERTY(BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<AActor> InteractableActor = nullptr;
};
