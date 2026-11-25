#include "Graph/STDialogueNodeTypes.h"

#include "STDialogueNodeInfo.h"

#pragma region USTGraphMiddleNode Interface
TSubclassOf<USTGraphNodeInfoBase> USTDialogueMiddleNode::GetNodeInfoClass() const {
    return USTDialogueMiddleNodeInfo::StaticClass();
}
#pragma endregion
