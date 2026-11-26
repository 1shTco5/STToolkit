#pragma once

#include "GameFramework/Actor.h"
#include "Interface/STInteractableInterface.h"
#include "STInteractableActor.generated.h"

class USTInteractableComponent;
class USceneComponent;

UCLASS(Blueprintable)
class STINTERACTIONSYSTEM_API ASTInteractableActor : public AActor, public ISTInteractableInterface {
    GENERATED_BODY()

public:
    ASTInteractableActor();

    //~Begin ISTInteractableInterface Interface
    virtual bool CanInteract_Implementation(const FSTInteractionContext& Context) const override;
    virtual void OnInteractionCompleted_Implementation(const FSTInteractionContext& Context) override;
    //~End ISTInteractableInterface Interface

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<USTInteractableComponent> InteractableComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<USceneComponent> InteractionPoint;
};
