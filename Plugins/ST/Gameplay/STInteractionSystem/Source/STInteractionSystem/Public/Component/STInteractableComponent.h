#pragma once

#include "Components/ActorComponent.h"
#include "Action/STInteractionAction.h"
#include "Filter/STInteractionCandidateFilter.h"
#include "Validator/STInteractionRequestValidator.h"
#include "STInteractableComponent.generated.h"

class USceneComponent;

USTRUCT(BlueprintType)
struct STINTERACTIONSYSTEM_API FSTInteractionStage {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Interaction|During")
    TArray<TObjectPtr<USTInteractionAction>> Actions;
};

UCLASS(Blueprintable, ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class STINTERACTIONSYSTEM_API USTInteractableComponent : public UActorComponent {
    GENERATED_BODY()
public:
    /** Returns the owner's interaction point, or null if no point is present. */
    UFUNCTION(BlueprintPure, Category = "Interaction")
    USceneComponent* GetInteractionPoint() const;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Interaction|Pre")
    TArray<TObjectPtr<USTInteractionCandidateFilter>> Filters;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Interaction|Request")
    TArray<TObjectPtr<USTInteractionRequestValidator>> Validators;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Interaction|Pre")
    TArray<TObjectPtr<USTInteractionAction>> RawCandidateActions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Interaction|Pre")
    TArray<TObjectPtr<USTInteractionAction>> ValidCandidateActions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Interaction|Pre")
    TArray<TObjectPtr<USTInteractionAction>> FocusedCandidateActions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|During")
    TArray<FSTInteractionStage> DuringInteractionStages;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Interaction|Post")
    TArray<TObjectPtr<USTInteractionAction>> PostInteractionActions;
};
