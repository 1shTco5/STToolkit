#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "STGraphSchema.generated.h"

UCLASS(Abstract)
class STGRAPHEDITOR_API USTGraphSchema : public UEdGraphSchema {
    GENERATED_BODY()

public:
    static const FName FlowPinCategory;

    //~Begin UEdGraphSchema Interface
    virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
    //~End UEdGraphSchema Interface
};
