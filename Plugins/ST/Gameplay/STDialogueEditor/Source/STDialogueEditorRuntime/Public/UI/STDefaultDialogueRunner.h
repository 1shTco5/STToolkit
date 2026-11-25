#pragma once

#include "STDialogueRunnerBase.h"
#include "STDefaultDialogueRunner.generated.h"

class USTDialogueWidget;

UCLASS(Blueprintable)
class STDIALOGUEEDITORRUNTIME_API USTDefaultDialogueRunner : public USTDialogueRunnerBase {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    void SetDialogueWidget(USTDialogueWidget* InWidget);

protected:
    //~Begin USTDialogueRunnerBase Interface
    virtual void OnCurrentNodeChanged_Implementation(const FSTRuntimeNode& Node) override;
    //~End USTDialogueRunnerBase Interface

private:
    void HandleResponseSelected(FGuid PinId);

    UPROPERTY(Transient)
    TObjectPtr<USTDialogueWidget> DialogueWidget;
};
