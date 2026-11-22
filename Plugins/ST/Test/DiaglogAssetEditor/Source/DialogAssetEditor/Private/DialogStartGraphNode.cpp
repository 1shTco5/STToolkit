#include "DialogStartGraphNode.h"

UEdGraphPin* UDialogStartGraphNode::CreateDialogPin(EEdGraphPinDirection Direction, FName Name) {
    FName Category = TEXT("Outputs");
    FName SubCategory = TEXT("StartPin");

    UEdGraphPin* Pin = CreatePin(Direction, Category, Name);
    Pin->PinType.PinSubCategory = SubCategory;
    return Pin;
}

UEdGraphPin* UDialogStartGraphNode::CreateDefaultInputPin() {
    return CreateDialogPin(EGPD_Output, "Start");
}