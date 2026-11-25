#include "Graph/STDialogueNodeTypes.h"

#include "STDialogueNodeInfo.h"

#pragma region USTGraphMiddleNode Interface
TSubclassOf<USTGraphNodeInfoBase> USTDialogueMiddleNode::GetNodeInfoClass() const {
    return USTDialogueMiddleNodeInfo::StaticClass();
}

const UScriptStruct* USTDialogueMiddleNode::GetOutputPayloadStruct() const {
    return FSTDialogueOutputPinPayload::StaticStruct();
}
#pragma endregion
