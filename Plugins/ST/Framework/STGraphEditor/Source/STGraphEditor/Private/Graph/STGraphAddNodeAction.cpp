#include "Graph/STGraphAddNodeAction.h"

#include "EdGraph/EdGraph.h"

FSTGraphAddNodeAction::FSTGraphAddNodeAction(TSubclassOf<USTGraphNodeBase> InNodeClass, FText InCategory,
                                             FText InDescription, FText InToolTip)
    : FEdGraphSchemaAction(MoveTemp(InCategory), MoveTemp(InDescription), MoveTemp(InToolTip), 0),
      NodeClass(InNodeClass) {}

#pragma region FEdGraphSchemaAction Interface
UEdGraphNode* FSTGraphAddNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                   const FVector2f& Location, bool bSelectNewNode) {
    if (!ParentGraph || !ParentGraph->bEditable || !NodeClass || NodeClass->HasAnyClassFlags(CLASS_Abstract)) {
        return nullptr;
    }

    FGraphNodeCreator<USTGraphNodeBase> NodeCreator(*ParentGraph);
    USTGraphNodeBase* Node = NodeCreator.CreateUserInvokedNode(bSelectNewNode, NodeClass);
    Node->NodePosX = FMath::RoundToInt(Location.X);
    Node->NodePosY = FMath::RoundToInt(Location.Y);
    NodeCreator.Finalize();
    if (FromPin) {
        Node->AutowireNewNode(FromPin);
    }
    ParentGraph->MarkPackageDirty();
    return Node;
}
#pragma endregion
