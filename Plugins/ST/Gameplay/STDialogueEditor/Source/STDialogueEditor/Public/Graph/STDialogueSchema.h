#pragma once

#include "CoreMinimal.h"
#include "Graph/STGraphSchema.h"
#include "STDialogueSchema.generated.h"

UCLASS()
class USTDialogueSchema : public USTGraphSchema {
    GENERATED_BODY()

protected:
    //~Begin USTGraphSchema Interface
    virtual void GetNodeMenuEntries(TArray<FSTGraphNodeMenuEntry>& OutEntries) const override;
    //~End USTGraphSchema Interface
};
