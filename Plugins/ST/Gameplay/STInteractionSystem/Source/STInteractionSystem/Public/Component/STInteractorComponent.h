#pragma once

#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "STInteractionContext.h"
#include "STInteractorComponent.generated.h"

class APawn;
class USTInteractableComponent;
class USTInteractionAction;

enum class ESTCandidateState : uint8 { Raw, Valid, Focused };

USTRUCT()
struct FSTCandidatePresentation {
    GENERATED_BODY()

    ESTCandidateState State = ESTCandidateState::Raw;

    UPROPERTY()
    FSTInteractionContext Context;

    UPROPERTY()
    TArray<TObjectPtr<USTInteractionAction>> Actions;
};

UCLASS(Blueprintable, ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class STINTERACTIONSYSTEM_API USTInteractorComponent : public UActorComponent {
    GENERATED_BODY()
public:
    USTInteractorComponent();

    /** Radius of the overlap query centered on the owning pawn. Zero disables discovery. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Query", meta = (ClampMin = "0", Units = "cm"))
    float QueryRadius = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Query")
    TEnumAsByte<ECollisionChannel> QueryChannel = ECC_GameTraceChannel1;

    USTInteractableComponent* GetFocusedTarget() const { return FocusedTarget; }

    //~Begin UActorComponent Interface
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* TickFunction) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    //~End UActorComponent Interface

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void RequestInteractionWithFocus();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void RequestInteraction(USTInteractableComponent* Target);

private:
    void QueryRawTargets();
    void FilterTargets();
    USTInteractableComponent* SelectFocusedTarget(const APawn& Pawn) const;
    void RefreshPresentations(TConstArrayView<TObjectPtr<USTInteractableComponent>> PreviousTargets);
    void ClearCandidates();
    void UpdatePresentation(USTInteractableComponent* Target, ESTCandidateState State);
    void StopPresentation(USTInteractableComponent* Target);

    UPROPERTY(Transient)
    TMap<TObjectPtr<USTInteractableComponent>, FSTCandidatePresentation> Presentations;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
    TArray<TObjectPtr<USTInteractableComponent>> RawTargets;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
    TArray<TObjectPtr<USTInteractableComponent>> ValidTargets;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USTInteractableComponent> FocusedTarget;
};
