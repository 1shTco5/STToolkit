#include "UI/STDialogueWidget.h"

#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

void USTDialogueWidget::SetDialogueLine(const FText& InSpeakerName, const FText& InDialogueText) {
    if (!ResponseList || !SpeakerText || !DialogueText) {
        return;
    }

    ResponseList->ClearChildren();
    SpeakerText->SetText(InSpeakerName);
    DialogueText->SetText(InDialogueText);
    if (SpeakerArea) {
        SpeakerArea->SetVisibility(InSpeakerName.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
    }
}

void USTDialogueWidget::AddResponse(const FText& Text, FGuid PinId) {
    if (!ResponseList || !ResponseWidgetClass) {
        return;
    }

    USTDialogueResponseWidget* Response = CreateWidget<USTDialogueResponseWidget>(this, ResponseWidgetClass);
    if (Response) {
        Response->SetResponse(Text, PinId);
        Response->OnSelected.BindUObject(this, &USTDialogueWidget::HandleResponseSelected);
        ResponseList->AddChild(Response);
    }
}

void USTDialogueWidget::HandleResponseSelected(FGuid PinId) { OnResponseSelected.ExecuteIfBound(PinId); }
