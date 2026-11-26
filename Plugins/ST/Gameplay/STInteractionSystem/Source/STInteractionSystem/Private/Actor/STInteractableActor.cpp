#include "Actor/STInteractableActor.h"

#include "Component/STInteractableComponent.h"
#include "Components/SceneComponent.h"

ASTInteractableActor::ASTInteractableActor() {
    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
    InteractionPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionPoint"));
    InteractionPoint->SetupAttachment(GetRootComponent());
    InteractableComponent = CreateDefaultSubobject<USTInteractableComponent>(TEXT("InteractableComponent"));
}

#pragma region ISTInteractableInterface
bool ASTInteractableActor::CanInteract_Implementation(const FSTInteractionContext& Context) const { return true; }

void ASTInteractableActor::OnInteractionCompleted_Implementation(const FSTInteractionContext& Context) {}
#pragma endregion
