#include "Component/STInteractableComponent.h"

#include "Actor/STInteractableActor.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"

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
