#include "Component/STInteractorComponent.h"

#include "Component/STInteractableComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Interface/STInteractableInterface.h"

namespace {
FSTInteractionContext MakeContext(USTInteractorComponent* Interactor, USTInteractableComponent* Target) {
    return {Interactor, Target, Interactor->GetOwner(), Target->GetOwner()};
}

bool IsCandidateValid(const FSTInteractionContext& Context) {
    if (!Context.InteractableComponent->CanInteract(Context)) return false;

    for (const USTInteractionCandidateFilter* Filter : Context.InteractableComponent->Filters) {
        if (IsValid(Filter) && !Filter->PassesFilter(Context)) return false;
    }
    return true;
}
} // namespace

USTInteractorComponent::USTInteractorComponent() {
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics;
}

#pragma region UActorComponent
void USTInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* TickFunction) {
    Super::TickComponent(DeltaTime, TickType, TickFunction);
    TryAdvanceDuringStage();

    const APawn* Pawn = Cast<APawn>(GetOwner());
    const APlayerController* Controller = Pawn ? Cast<APlayerController>(Pawn->GetController()) : nullptr;
    if (!Controller || !Controller->IsLocalController()) {
        ClearCandidates();
        return;
    }

    const auto PreviousTargets = MoveTemp(RawTargets);
    QueryRawTargets();
    FilterTargets();
    FocusedTarget = SelectFocusedTarget(*Pawn);
    RefreshPresentations(PreviousTargets);
}

void USTInteractorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    ClearCandidates();
    Super::EndPlay(EndPlayReason);
}
#pragma endregion

void USTInteractorComponent::ClearCandidates() {
    TArray<TObjectPtr<USTInteractableComponent>> Targets;
    Presentations.GenerateKeyArray(Targets);
    RawTargets.Reset();
    ValidTargets.Reset();
    FocusedTarget = nullptr;
    for (USTInteractableComponent* Target : Targets) StopPresentation(Target);
}

void USTInteractorComponent::QueryRawTargets() {
    RawTargets.Reset();
    if (QueryRadius <= 0.f) return;

    const AActor* Owner = GetOwner();
    const FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(STInteractionQuery), false, Owner);
    TArray<FOverlapResult> Overlaps;
    GetWorld()->OverlapMultiByChannel(Overlaps, Owner->GetActorLocation(), FQuat::Identity, QueryChannel,
                                      FCollisionShape::MakeSphere(QueryRadius), QueryParams);

    for (const FOverlapResult& Overlap : Overlaps) {
        AActor* Actor = Overlap.GetActor();
        if (!IsValid(Actor)) continue;
        if (auto* Target = Actor->FindComponentByClass<USTInteractableComponent>()) RawTargets.AddUnique(Target);
    }
}

void USTInteractorComponent::FilterTargets() {
    ValidTargets.Reset();
    for (USTInteractableComponent* Target : RawTargets) {
        if (IsValid(Target) && IsCandidateValid(MakeContext(this, Target))) ValidTargets.Add(Target);
    }
}

USTInteractableComponent* USTInteractorComponent::SelectFocusedTarget(const APawn& Pawn) const {
    if (ValidTargets.IsEmpty()) return nullptr;
    const FVector EyeLocation = Pawn.GetPawnViewLocation();
    const FVector Forward = Pawn.GetActorForwardVector();
    double BestScore = -2.0;
    USTInteractableComponent* BestTarget = nullptr;
    for (USTInteractableComponent* Target : ValidTargets) {
        if (!IsValid(Target)) continue;
        const USceneComponent* Point = Target->GetInteractionPoint();
        const FVector Location = Point ? Point->GetComponentLocation() : Target->GetOwner()->GetActorLocation();
        const double Score = FVector::DotProduct(Forward, (Location - EyeLocation).GetSafeNormal());
        if (Score > BestScore || (Score == BestScore && Target == FocusedTarget)) {
            BestScore = Score;
            BestTarget = Target;
        }
    }
    return BestTarget;
}

void USTInteractorComponent::RefreshPresentations(
    TConstArrayView<TObjectPtr<USTInteractableComponent>> PreviousTargets) {
    for (USTInteractableComponent* Target : PreviousTargets) {
        if (!RawTargets.Contains(Target)) StopPresentation(Target);
    }
    // Blueprint actions can end play and clear the component's target arrays.
    const auto Targets = RawTargets;
    for (USTInteractableComponent* Target : Targets) {
        if (!IsValid(Target) || !RawTargets.Contains(Target)) continue;
        const auto State = Target == FocusedTarget
                               ? ESTCandidateState::Focused
                               : ValidTargets.Contains(Target)
                               ? ESTCandidateState::Valid
                               : ESTCandidateState::Raw;
        UpdatePresentation(Target, State);
    }
}

void USTInteractorComponent::UpdatePresentation(USTInteractableComponent* Target, ESTCandidateState State) {
    if (const auto* Current = Presentations.Find(Target); Current && Current->State == State) return;
    StopPresentation(Target);
    if (!RawTargets.Contains(Target) || !IsValid(Target)) return;

    const FSTInteractionContext Context = MakeContext(this, Target);
    auto& Presentation = Presentations.Add(Target);
    Presentation.State = State;
    Presentation.Context = Context;
    const auto Templates = State == ESTCandidateState::Focused
                               ? Target->FocusedCandidateActions
                               : State == ESTCandidateState::Valid
                               ? Target->ValidCandidateActions
                               : Target->RawCandidateActions;
    for (USTInteractionAction* Template : Templates) {
        if (!IsValid(Template)) continue;
        if (Template->ExecutionPolicy == ESTInteractionExecutionPolicy::ServerOnly && !GetOwner()->HasAuthority())
            continue;
        USTInteractionAction* Action = DuplicateObject<USTInteractionAction>(Template, this);
        Action->ReceiveInitialize(Context);
        auto* Current = Presentations.Find(Target);
        if (!Current) {
            Action->ReceiveShutdown(Context);
            return;
        }
        Current->Actions.Add(Action);
        Action->ReceiveStart(Context);
        if (!Presentations.Contains(Target)) return;
    }
}

void USTInteractorComponent::StopPresentation(USTInteractableComponent* Target) {
    FSTCandidatePresentation Previous;
    if (!Presentations.RemoveAndCopyValue(Target, Previous)) return;
    for (USTInteractionAction* Action : Previous.Actions) {
        if (!IsValid(Action)) continue;
        Action->ReceiveStop(Previous.Context);
        Action->ReceiveShutdown(Previous.Context);
    }
}

void USTInteractorComponent::RequestInteractionWithFocus() {
    if (IsValid(FocusedTarget)) RequestInteraction(FocusedTarget);
}

void USTInteractorComponent::RequestInteraction(USTInteractableComponent* Target) {
    const AActor* Owner = GetOwner();
    if (!IsValid(Owner) || !Owner->HasAuthority() || !IsValid(Target) || !IsValid(Target->GetOwner())) return;

    const FSTInteractionContext Context = MakeContext(this, Target);
    const FSTInteractionValidationResult Validation = Target->ValidateInteractionRequest(Context);
    if (!Validation.bAllowed) return;

    ActiveInteractionContext = Context;
    ActiveStageIndex = INDEX_NONE;
    TryAdvanceDuringStage();
}

void USTInteractorComponent::TryAdvanceDuringStage() {
    const USTInteractableComponent* Target = ActiveInteractionContext.InteractableComponent.Get();
    if (!IsValid(Target)) return;
    for (const USTInteractionAction* Action : ActiveStageActions) {
        if (Action->bWaitForCompletion && !Action->IsCompleted()) return;
    }

    for (USTInteractionAction* Action : ActiveStageActions) {
        Action->ReceiveStop(ActiveInteractionContext);
        Action->ReceiveShutdown(ActiveInteractionContext);
    }
    ActiveStageActions.Reset();

    ++ActiveStageIndex;
    if (Target->DuringInteractionStages.IsValidIndex(ActiveStageIndex)) {
        StartInteractionStage();
    } else {
        FinishInteraction();
    }
}

void USTInteractorComponent::StartInteractionStage() {
    const APawn* Pawn = Cast<APawn>(GetOwner());
    const bool bLocalPlayer = Pawn && Pawn->IsPlayerControlled() && Pawn->IsLocallyControlled();
    const auto& Templates =
        ActiveInteractionContext.InteractableComponent->DuringInteractionStages[ActiveStageIndex].Actions;
    for (USTInteractionAction* Template : Templates) {
        if (!IsValid(Template)) continue;
        if (Template->ExecutionPolicy == ESTInteractionExecutionPolicy::OwningClientOnly && !bLocalPlayer) continue;

        USTInteractionAction* Action = DuplicateObject<USTInteractionAction>(Template, this);
        ActiveStageActions.Add(Action);
        Action->ReceiveInitialize(ActiveInteractionContext);
        Action->ReceiveStart(ActiveInteractionContext);
    }
}

void USTInteractorComponent::FinishInteraction() {
    const FSTInteractionContext CompletedContext = ActiveInteractionContext;
    ActiveInteractionContext = {};
    ActiveStageIndex = INDEX_NONE;

    AActor* TargetActor = CompletedContext.InteractableActor;
    if (IsValid(TargetActor) && TargetActor->Implements<USTInteractableInterface>()) {
        ISTInteractableInterface::Execute_OnInteractionCompleted(TargetActor, CompletedContext);
    }
}