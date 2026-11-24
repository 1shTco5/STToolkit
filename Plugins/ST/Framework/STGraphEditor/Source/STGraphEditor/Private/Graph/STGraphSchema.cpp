#include "Graph/STGraphSchema.h"

#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"

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
#pragma endregion
