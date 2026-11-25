#pragma once

#include "STGraphNodeInfo.h"
#include "STDialogueNodeInfo.generated.h"

UCLASS()
class STDIALOGUEEDITORRUNTIME_API USTDialogueMiddleNodeInfo : public USTGraphMiddleNodeInfo {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Dialogue")
    FText SpeakerName;

    UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = true))
    FText DialogueText;
};
