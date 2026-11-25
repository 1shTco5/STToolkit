#pragma once

#include "Blueprint/UserWidget.h"
#include "Misc/Guid.h"
#include "UI/STDialogueResponseWidget.h"
#include "STDialogueWidget.generated.h"

class UPanelWidget;
class UTextBlock;
class UWidget;

UCLASS(Blueprintable)
class STDIALOGUEEDITORRUNTIME_API USTDialogueWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void SetDialogueLine(const FText& InSpeakerName, const FText& InDialogueText);
    void AddResponse(const FText& Text, FGuid PinId);

    FSTDialogueResponseSelected OnResponseSelected;

private:
    void HandleResponseSelected(FGuid PinId);

    UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
    TSubclassOf<USTDialogueResponseWidget> ResponseWidgetClass;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SpeakerText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DialogueText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPanelWidget> ResponseList;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<UWidget> SpeakerArea;
};
