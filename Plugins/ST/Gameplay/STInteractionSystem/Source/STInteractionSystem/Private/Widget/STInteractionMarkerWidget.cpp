#include "Widget/STInteractionMarkerWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/Actor.h"

void USTInteractionMarkerWidget::SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }

#pragma region UUserWidget
void USTInteractionMarkerWidget::NativeConstruct() {
    Super::NativeConstruct();
    SetAlignmentInViewport(FVector2D(0.5, 0.5));
    SetRenderOpacity(0.0f);
}

void USTInteractionMarkerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
    Super::NativeTick(MyGeometry, InDeltaTime);

    const AActor* Target = TargetActor.Get();
    FVector2D ScreenPosition = FVector2D::ZeroVector;
    const bool bProjected =
        IsValid(Target) && UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
                               GetOwningPlayer(), Target->GetActorLocation() + WorldOffset, ScreenPosition, true);
    if (bProjected) SetPositionInViewport(ScreenPosition, false);
    SetRenderOpacity(bProjected ? 1.0f : 0.0f);
}
#pragma endregion
