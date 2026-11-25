#include "UI/STDialogueResponseWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USTDialogueResponseWidget::SetResponse(const FText& Text, FGuid InPinId) {
    if (ResponseText) {
        ResponseText->SetText(Text);
    }
    PinId = InPinId;
}

#pragma region UUserWidget Interface
void USTDialogueResponseWidget::NativeOnInitialized() {
    Super::NativeOnInitialized();
    if (ResponseButton) {
        ResponseButton->OnClicked.AddDynamic(this, &USTDialogueResponseWidget::HandleClicked);
    }
}
#pragma endregion

void USTDialogueResponseWidget::HandleClicked() { OnSelected.ExecuteIfBound(PinId); }
