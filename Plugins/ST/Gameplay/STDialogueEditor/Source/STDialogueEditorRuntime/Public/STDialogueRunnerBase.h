#pragma once

#include "Misc/Guid.h"
#include "STDialogueNodeInfo.h"
#include "STGraphRuntimeData.h"
#include "UObject/Object.h"
#include "STDialogueRunnerBase.generated.h"

class USTDialogueAsset;

UCLASS(BlueprintType, Blueprintable)
class STDIALOGUEEDITORRUNTIME_API USTDialogueRunnerBase : public UObject {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool Start(USTDialogueAsset* InDialogueAsset);

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    FSTRuntimeNode GetCurrentNode() const;

    UFUNCTION(BlueprintPure, Category = "Dialogue")
    bool GetResponsePayload(int32 OutputIndex, FSTDialogueOutputPinPayload& OutPayload, FGuid& OutPinId) const;

    UFUNCTION(BlueprintCallable, Category = "Dialogue")
    bool ChooseResponse(const FGuid& PinId);

protected:
    UFUNCTION(BlueprintNativeEvent, Category = "Dialogue")
    void OnCurrentNodeChanged(const FSTRuntimeNode& Node);
    virtual void OnCurrentNodeChanged_Implementation(const FSTRuntimeNode& Node);

private:
    UPROPERTY(Transient)
    TObjectPtr<USTDialogueAsset> DialogueAsset;

    FGuid CurrentNodeId;
};
