#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "DialogGraphSchema.generated.h"

UCLASS()
class UDialogGraphSchema : public UEdGraphSchema {
    GENERATED_BODY()

public:
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
    virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
};

USTRUCT()
struct FNewNodeAction : public FEdGraphSchemaAction {
    GENERATED_BODY()

public:
    FNewNodeAction() { return; }

    FNewNodeAction(
        UClass* ClassTemplate, FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping)
        : FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), m_ClassTemplate(ClassTemplate) {
    }

    virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                        bool bSelectNewNode = true) override;

protected:
    UClass* m_ClassTemplate = nullptr;
};