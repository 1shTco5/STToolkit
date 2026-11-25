#pragma once

#include "STGraphNodeInfo.h"
#include "STDialogueNodeInfo.generated.h"

USTRUCT()
struct STDIALOGUEEDITORRUNTIME_API FSTDialogueOutputPinPayload {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = true))
    FText ResponseText;
};

UCLASS()
class STDIALOGUEEDITORRUNTIME_API USTDialogueMiddleNodeInfo : public USTGraphMiddleNodeInfo {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Dialogue")
    FText SpeakerName;

    UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = true))
    FText DialogueText;
};
