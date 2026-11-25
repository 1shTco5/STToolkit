#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "STGraphRuntimeData.h"
#include "Templates/SubclassOf.h"
#include "STGraphSchema.generated.h"

class USTGraphNodeBase;

struct FSTGraphNodeMenuEntry {
    TSubclassOf<USTGraphNodeBase> NodeClass;
    ESTGraphNodeRole Role = ESTGraphNodeRole::Middle;
    FText MenuText;
    FText Tooltip;
};

UCLASS(Abstract)
class STGRAPHEDITOR_API USTGraphSchema : public UEdGraphSchema {
    GENERATED_BODY()

public:
    static const FName FlowPinCategory;

    //~Begin UEdGraphSchema Interface
    virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
    //~End UEdGraphSchema Interface

protected:
    virtual void GetNodeMenuEntries(TArray<FSTGraphNodeMenuEntry>& OutEntries) const {}
};
