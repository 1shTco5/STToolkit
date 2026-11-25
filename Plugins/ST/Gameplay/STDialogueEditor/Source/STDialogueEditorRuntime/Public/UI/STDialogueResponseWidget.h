#pragma once

#include "Blueprint/UserWidget.h"
#include "Misc/Guid.h"
#include "STDialogueResponseWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DELEGATE_OneParam(FSTDialogueResponseSelected, FGuid);

UCLASS(Blueprintable)
class STDIALOGUEEDITORRUNTIME_API USTDialogueResponseWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void SetResponse(const FText& Text, FGuid InPinId);

    FSTDialogueResponseSelected OnSelected;

protected:
    //~Begin UUserWidget Interface
    virtual void NativeOnInitialized() override;
    //~End UUserWidget Interface

private:
    UFUNCTION()
    void HandleClicked();

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ResponseButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ResponseText;

    FGuid PinId;
};
