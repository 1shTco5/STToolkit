#include "CustomGraphSchema.h"
#include "CustomGraphNode.h"

void UCustomGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const {
    TSharedPtr<FNewNodeAction> NewNodeAction(new FNewNodeAction(
            FText::FromString("Nodes"),
            FText::FromString("New Node"),
            FText::FromString("Makes a new node"),
            0)
        );

    ContextMenuBuilder.AddAction(NewNodeAction);
}

const FPinConnectionResponse UCustomGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const {
    // Neither can be nullptr
    if (A == nullptr || B == nullptr) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Need 2 pins"));
    }

    // Make sure the directions are different (no inputs to inputs or outputs to outputs)
    if (A->Direction == B->Direction) {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Inputs can only connect to outputs"));
    }

    // Else allow it but break all connections
    return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

UEdGraphNode* FNewNodeAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                            bool bSelectNewNode) {
    UCustomGraphNode* Result = NewObject<UCustomGraphNode>(ParentGraph);
    Result->CreateNewGuid();
    Result->NodePosX = Location.X;
    Result->NodePosY = Location.Y;

    //Result->CreatePin(EGPD_Input, TEXT("Inputs"), TEXT("SomeInput"));
    //Result->CreatePin(EGPD_Output, TEXT("Outputs"), TEXT("Output1"));
    //Result->CreatePin(EGPD_Output, TEXT("Outputs"), TEXT("Output2"));
    
    UEdGraphPin* InputPin = Result->CreateCustomPin(EGPD_Input, TEXT("SomeInput"));
    Result->CreateCustomPin(EGPD_Output, TEXT("Output1"));
    Result->CreateCustomPin(EGPD_Output, TEXT("Output2"));
    
    if (FromPin != nullptr) {
        Result->GetSchema()->TryCreateConnection(FromPin, InputPin);
    }

    ParentGraph->Modify();
    ParentGraph->AddNode(Result, true, true);
    return Result;
}