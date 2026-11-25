#include "Graph/STGraphSchema.h"

#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "Graph/STGraphAddNodeAction.h"

const FName USTGraphSchema::FlowPinCategory(TEXT("Flow"));

#pragma region UEdGraphSchema Interface
FLinearColor USTGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const {
    return PinType.PinCategory == FlowPinCategory ? FLinearColor::White : Super::GetPinTypeColor(PinType);
}

const FPinConnectionResponse USTGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const {
    if (!A || !B || A->bNotConnectable || B->bNotConnectable || A->PinType.PinCategory != FlowPinCategory ||
        B->PinType.PinCategory != FlowPinCategory) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString(TEXT("Select two flow pins")));
    }

    const UEdGraphNode* NodeA = A->GetOwningNode();
    const UEdGraphNode* NodeB = B->GetOwningNode();
    if (NodeA == NodeB || NodeA->GetGraph() != NodeB->GetGraph() || A->Direction == B->Direction) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW,
                                      FText::FromString(TEXT("Connect an output to another node's input")));
    }
    if (A->LinkedTo.Contains(B)) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, FText::FromString(TEXT("Pins are already linked")));
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, FText::GetEmpty());
}

void USTGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const {
    const UEdGraphPin* FromPin = ContextMenuBuilder.FromPin;
    if (FromPin && (FromPin->bNotConnectable || FromPin->PinType.PinCategory != FlowPinCategory)) {
        return;
    }

    TArray<FSTGraphNodeMenuEntry> Entries;
    GetNodeMenuEntries(Entries);

    const FText Category = FText::FromString(TEXT("Nodes"));
    for (const FSTGraphNodeMenuEntry& Entry : Entries) {
        if (!Entry.NodeClass || Entry.NodeClass->HasAnyClassFlags(CLASS_Abstract)) {
            continue;
        }

        if (FromPin && ((Entry.Role == ESTGraphNodeRole::Start && FromPin->Direction != EGPD_Input) ||
                        (Entry.Role == ESTGraphNodeRole::End && FromPin->Direction != EGPD_Output))) {
            continue;
        }

        ContextMenuBuilder.AddAction(
            MakeShared<FSTGraphAddNodeAction>(Entry.NodeClass, Category, Entry.MenuText, Entry.Tooltip));
    }
}
#pragma endregion
