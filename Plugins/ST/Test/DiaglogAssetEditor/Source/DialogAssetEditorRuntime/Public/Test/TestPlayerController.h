#pragma once

#include "CoreMinimal.h"
#include "TestPlayerController.generated.h"

class UDialogPlayer;
class UDialogAsset;

UCLASS()
class ATestPlayerController : public APlayerController {
    GENERATED_BODY()

public:
    ATestPlayerController();
    virtual ~ATestPlayerController();

    virtual void BeginPlay() override;

protected:
    UPROPERTY()
    UDialogAsset* DialogAsset;
    
    UPROPERTY()
    UDialogPlayer* DialogPlayer;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UDialogPlayer> DialogPlayerClass;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> NewQuestOverlayClass;
    
    UPROPERTY()
    UUserWidget* NewQuestOverlay;
};