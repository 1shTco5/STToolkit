#include "TestPlayerController.h"
#include "DialogEndNodeInfo.h"
#include "DialogPlayer.h"
#include "DialogAsset.h"
#include "UObject/ConstructorHelpers.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

ATestPlayerController::ATestPlayerController() {
    static ConstructorHelpers::FObjectFinder<UDialogAsset> DialogAssetFinder(TEXT("/DialogAssetEditor/Test/TestDialog.TestDialog"));
    if (DialogAssetFinder.Succeeded()) {
        DialogAsset = DialogAssetFinder.Object;
    }
}

ATestPlayerController::~ATestPlayerController() {
}

void ATestPlayerController::BeginPlay() {
    Super::BeginPlay();

    // Play the dialog
    DialogPlayer = NewObject<UDialogPlayer>(this, DialogPlayerClass);
    DialogPlayer->PlayDialog(DialogAsset, this, [this](EDialogNodeAction Action, FString ActionData) {
        if (Action == EDialogNodeAction::StartQuest) {
            if (NewQuestOverlay != nullptr) {
                NewQuestOverlay->RemoveFromParent();
            }
            NewQuestOverlay = CreateWidget<UUserWidget>(this, NewQuestOverlayClass);
            if (UTextBlock* QuestNameText = Cast<
                UTextBlock>(NewQuestOverlay->GetWidgetFromName(TEXT("QuestNameText")))) {
                QuestNameText->SetText(FText::FromString(ActionData));
                NewQuestOverlay->AddToViewport();
            }
        }
    });
}