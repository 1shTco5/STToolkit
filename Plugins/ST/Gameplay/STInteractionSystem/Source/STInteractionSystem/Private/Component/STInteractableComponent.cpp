#include "Component/STInteractableComponent.h"

#include "Actor/STInteractableActor.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/STInteractableInterface.h"

bool USTInteractableComponent::CanInteract(const FSTInteractionContext& Context) const {
    AActor* Owner = GetOwner();
    return IsValid(Owner) && (!Owner->Implements<USTInteractableInterface>() ||
                              ISTInteractableInterface::Execute_CanInteract(Owner, Context));
}

FSTInteractionValidationResult USTInteractableComponent::ValidateInteractionRequest(
    const FSTInteractionContext& Context) const {
    if (!CanInteract(Context)) {
        return {false, NSLOCTEXT("STInteractionSystem", "TargetUnavailable", "This target cannot be interacted with.")};
    }

    for (const USTInteractionRequestValidator* Validator : Validators) {
        if (!IsValid(Validator)) continue;
        FSTInteractionValidationResult Result = Validator->Validate(Context);
        if (!Result.bAllowed) return Result;
    }

    return {true, FText::GetEmpty()};
}

USceneComponent* USTInteractableComponent::GetInteractionPoint() const {
    const AActor* Owner = GetOwner();
    if (!IsValid(Owner)) return nullptr;
    if (const ASTInteractableActor* Interactable = Cast<ASTInteractableActor>(Owner)) {
        return IsValid(Interactable->InteractionPoint) ? Interactable->InteractionPoint.Get() : nullptr;
    }

    static const FName PointName(TEXT("InteractionPoint"));
    for (UActorComponent* Component : Owner->GetComponents()) {
        if (IsValid(Component) && Component->GetFName() == PointName) return Cast<USceneComponent>(Component);
    }
    return nullptr;
}
