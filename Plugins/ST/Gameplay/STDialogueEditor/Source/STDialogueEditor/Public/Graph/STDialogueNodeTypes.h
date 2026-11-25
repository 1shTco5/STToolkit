#pragma once

#include "Graph/STGraphNodeTypes.h"
#include "STDialogueNodeTypes.generated.h"

UCLASS()
class USTDialogueStartNode : public USTGraphStartNode {
    GENERATED_BODY()
};

UCLASS()
class USTDialogueMiddleNode : public USTGraphMiddleNode {
    GENERATED_BODY()

protected:
    //~Begin USTGraphMiddleNode Interface
    virtual TSubclassOf<USTGraphNodeInfoBase> GetNodeInfoClass() const override;
    //~End USTGraphMiddleNode Interface
};

UCLASS()
class USTDialogueEndNode : public USTGraphEndNode {
    GENERATED_BODY()
};
