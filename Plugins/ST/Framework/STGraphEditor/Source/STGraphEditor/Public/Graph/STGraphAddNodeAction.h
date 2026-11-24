#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "Graph/STGraphNodeBase.h"

struct STGRAPHEDITOR_API FSTGraphAddNodeAction : public FEdGraphSchemaAction {
    FSTGraphAddNodeAction(TSubclassOf<USTGraphNodeBase> InNodeClass, FText InCategory, FText InDescription,
                          FText InToolTip);

    //~Begin FEdGraphSchemaAction Interface
    virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2f& Location,
                                        bool bSelectNewNode = true) override;
    //~End FEdGraphSchemaAction Interface

private:
    TSubclassOf<USTGraphNodeBase> NodeClass;
};
