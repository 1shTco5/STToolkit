#include "DialogPlayer.h"
#include "DialogAsset.h"
#include "DialogEndNodeInfo.h"
#include "DialogNodeInfo.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

DEFINE_LOG_CATEGORY_STATIC(DialogPlayerSub, Log, All);

void UDialogPlayer::PlayDialog(UDialogAsset* DialogAsset, APlayerController* PC,
                               std::function<void(EDialogNodeAction Action, FString Data)> OnDialogEnded) {
    m_OnDialogEnded = OnDialogEnded;
    UDialogRuntimeGraph* Graph = DialogAsset->Graph;

    // Get the start node first
    for (UDialogRuntimeNode* Node : Graph->Nodes) {
        if (Node->NodeType == EDialogNodeType::StartNode) {
            m_CurrentNode = Node;
            break;
        }
    }

    if (m_CurrentNode == nullptr) {
        UE_LOG(DialogPlayerSub, Error, TEXT("No start node in dialog."));
        return;
    }

    // Create and display the dialog UI
    if (IsValid(PC) && DialogUIClass) {
        m_DialogUI = CreateWidget<UUserWidget>(PC, DialogUIClass);
        m_DialogUI->AddToViewport();
    }

    // Play the first index to get started
    ChooseOptionAtIndex(0);
}

void UDialogPlayer::ChooseOptionAtIndex(int Index) {
    // Move to the given index if we can
    if (Index >= m_CurrentNode->OutputPins.Num() || Index < 0) {
        UE_LOG(DialogPlayerSub, Error, TEXT("Invalid response option at index %d"), Index);
        return;
    }

    UDialogRuntimePin* OutputPin = m_CurrentNode->OutputPins[Index];
    if (OutputPin->Connection != nullptr) {
        m_CurrentNode = OutputPin->Connection->Parent;
    } else {
        // No connection so we'll assume it's an end node
        m_CurrentNode = nullptr;
    }

    if (m_CurrentNode != nullptr && m_CurrentNode->NodeType == EDialogNodeType::DialogNode) {
        UDialogNodeInfo* NodeInfo = Cast<UDialogNodeInfo>(m_CurrentNode->NodeInfo);

        if (UTextBlock* DialogText = Cast<UTextBlock>(m_DialogUI->GetWidgetFromName(TEXT("DialogText")))) {
            DialogText->SetText(NodeInfo->DialogText);
        }
        if (UHorizontalBox* ResponsesBox = Cast<UHorizontalBox>(m_DialogUI->GetWidgetFromName(TEXT("ResponsesBox")))) {
            ResponsesBox->ClearChildren();
        }

        int OptionIndex = 0;
        for (FText Response : NodeInfo->DialogResponses) {
            if (UUserWidget* Button = CreateWidget(m_DialogUI->GetOwningPlayer(), QuestResponseButtonClass)) {
                //if (UButton* ResponseButton = Cast<UButton>(Button->GetWidgetFromName(TEXT("ResponseButton")))) {
                //}
                SetResponseButtonClick(Button, OptionIndex);
                if (UTextBlock* ResponseButtonText =
                    Cast<UTextBlock>(Button->GetWidgetFromName(TEXT("ResponseButtonText")))) {
                    ResponseButtonText->SetText(Response);
                }
                if (UHorizontalBox* ResponsesBox =
                    Cast<UHorizontalBox>(m_DialogUI->GetWidgetFromName(TEXT("ResponsesBox")))) {
                    UHorizontalBoxSlot* Slot = ResponsesBox->AddChildToHorizontalBox(Button);
                    Slot->SetPadding(FMargin(10));
                }
            }

            OptionIndex++;
        }
    } else if (m_CurrentNode == nullptr || m_CurrentNode->NodeType == EDialogNodeType::EndNode) {
        APlayerController* PC = m_DialogUI->GetOwningPlayer();
        m_DialogUI->RemoveFromParent();
        m_DialogUI = nullptr;

        EDialogNodeAction Action = EDialogNodeAction::None;
        FString ActionData = TEXT("");
        if (m_CurrentNode != nullptr) {
            UDialogEndNodeInfo* EndNodeInfo = Cast<UDialogEndNodeInfo>(m_CurrentNode->NodeInfo);
            Action = EndNodeInfo->Action;
            ActionData = EndNodeInfo->ActionData;
        }

        m_OnDialogEnded(Action, ActionData);
    }
}