#pragma once

#include "CoreMinimal.h"
#include <functional>
#include "DialogEndNodeInfo.h"
#include "DialogPlayer.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UDialogPlayer : public UObject {
    GENERATED_BODY()

public:
    void PlayDialog(class UDialogAsset* DialogAsset, APlayerController* PC,
                    std::function<void(EDialogNodeAction Action, FString Data)> OnDialogEnded);
    UFUNCTION(BlueprintCallable)
    void ChooseOptionAtIndex(int Index);
    
    UFUNCTION(BlueprintImplementableEvent)
    void SetResponseButtonClick(UUserWidget* Button, int OptionIndex);

private:
    UPROPERTY()
    class UDialogAsset* m_PlayingAsset = nullptr;

    UPROPERTY()
    class UDialogRuntimeNode* m_CurrentNode = nullptr;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> DialogUIClass;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> QuestResponseButtonClass;
    
    UPROPERTY()
    UUserWidget* m_DialogUI;

    std::function<void(EDialogNodeAction Action, FString Data)> m_OnDialogEnded;
};