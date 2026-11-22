#include "DialogGraphSchema.h"

#include "DialogEndGraphNode.h"
#include "DialogGraphNode.h"
#include "DialogStartGraphNode.h"
#include "DialogNodeInfo.h"

void UDialogGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const {
    TSharedPtr<FNewNodeAction> NewNodeAction(new FNewNodeAction(
            UDialogGraphNode::StaticClass(),
            FText::FromString("Nodes"),
            FText::FromString("New Dialog Node"),
            FText::FromString("Make a new dialog node"),
            0)
        );
    
    TSharedPtr<FNewNodeAction> NewEndNodeAction(new FNewNodeAction(
            UDialogEndGraphNode::StaticClass(),
            FText::FromString("Nodes"),
            FText::FromString("New End Node"),
            FText::FromString("Make a new end node"),
            0)
        );

    ContextMenuBuilder.AddAction(NewNodeAction);
    ContextMenuBuilder.AddAction(NewEndNodeAction);
}

const FPinConnectionResponse UDialogGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const {
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

void UDialogGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const {
    UDialogStartGraphNode* StartNode = NewObject<UDialogStartGraphNode>(&Graph);
    StartNode->CreateNewGuid();
    StartNode->NodePosX = 0;
    StartNode->NodePosY = 0;
    StartNode->CreateDefaultInputPin();

    Graph.AddNode(StartNode, true, true);
    Graph.Modify();
}

UEdGraphNode* FNewNodeAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                            bool bSelectNewNode) {
    UDialogGraphNodeBase* Result = NewObject<UDialogGraphNodeBase>(ParentGraph, m_ClassTemplate);
    Result->CreateNewGuid();
    Result->NodePosX = Location.X;
    Result->NodePosY = Location.Y;
    Result->InitNodeInfo(Result);

    UEdGraphPin* InputPin = Result->CreateDefaultInputPin();
    Result->CreateDefaultOutputPins();

    if (FromPin != nullptr) {
        Result->GetSchema()->TryCreateConnection(FromPin, InputPin);
    }

    ParentGraph->Modify();
    ParentGraph->AddNode(Result, true, true);
    return Result;
}