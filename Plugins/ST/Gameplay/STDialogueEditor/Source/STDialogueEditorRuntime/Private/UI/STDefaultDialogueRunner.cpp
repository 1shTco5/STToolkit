#include "UI/STDefaultDialogueRunner.h"

#include "UI/STDialogueWidget.h"

void USTDefaultDialogueRunner::SetDialogueWidget(USTDialogueWidget* InWidget) {
    if (DialogueWidget && DialogueWidget != InWidget && DialogueWidget->OnResponseSelected.IsBoundToObject(this)) {
        DialogueWidget->OnResponseSelected.Unbind();
        DialogueWidget->RemoveFromParent();
    }

    DialogueWidget = InWidget;
    if (DialogueWidget) {
        DialogueWidget->OnResponseSelected.BindUObject(this, &USTDefaultDialogueRunner::HandleResponseSelected);
        OnCurrentNodeChanged(GetCurrentNode());
    }
}

#pragma region USTDialogueRunnerBase Interface
void USTDefaultDialogueRunner::OnCurrentNodeChanged_Implementation(const FSTRuntimeNode& Node) {
    if (!DialogueWidget) {
        return;
    }

    const USTDialogueMiddleNodeInfo* Info =
        Node.Role == ESTGraphNodeRole::Middle ? Cast<USTDialogueMiddleNodeInfo>(Node.NodeInfo.Get()) : nullptr;
    if (!Info) {
        DialogueWidget->RemoveFromParent();
        return;
    }

    DialogueWidget->SetDialogueLine(Info->SpeakerName, Info->DialogueText);
    for (int32 Index = 0; Index < Node.OutputPins.Num(); ++Index) {
        FSTDialogueOutputPinPayload Payload;
        FGuid PinId;
        if (GetResponsePayload(Index, Payload, PinId)) {
            DialogueWidget->AddResponse(Payload.ResponseText, PinId);
        }
    }

    if (!DialogueWidget->IsInViewport()) {
        DialogueWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        DialogueWidget->AddToViewport();
    }
}
#pragma endregion

void USTDefaultDialogueRunner::HandleResponseSelected(FGuid PinId) { ChooseResponse(PinId); }
