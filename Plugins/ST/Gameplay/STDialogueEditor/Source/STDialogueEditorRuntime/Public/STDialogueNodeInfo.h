#pragma once

#include "STGraphNodeInfo.h"
#include "STDialogueNodeInfo.generated.h"

USTRUCT(BlueprintType)
struct STDIALOGUEEDITORRUNTIME_API FSTDialogueOutputPinPayload {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (MultiLine = true))
    FText ResponseText;
};

UCLASS()
class STDIALOGUEEDITORRUNTIME_API USTDialogueMiddleNodeInfo : public USTGraphMiddleNodeInfo {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
    FText SpeakerName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (MultiLine = true))
    FText DialogueText;
};
