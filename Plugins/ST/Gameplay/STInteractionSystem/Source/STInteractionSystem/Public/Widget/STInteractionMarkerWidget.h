#pragma once

#include "Blueprint/UserWidget.h"
#include "STInteractionMarkerWidget.generated.h"

class AActor;

UCLASS(Abstract, Blueprintable)
class STINTERACTIONSYSTEM_API USTInteractionMarkerWidget : public UUserWidget {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Interaction|Marker")
    void SetTargetActor(AActor* InTargetActor);

    /** Offset from the target actor's origin in world space. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Marker",
              meta = (ExposeOnSpawn = "true", Units = "cm"))
    FVector WorldOffset = FVector(0.0, 0.0, 60.0);

    /** Text available to the widget subclass for presentation. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Marker", meta = (ExposeOnSpawn = "true"))
    FText MarkerText;

protected:
    //~Begin UUserWidget Interface
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    //~End UUserWidget Interface

private:
    TWeakObjectPtr<AActor> TargetActor;
};
