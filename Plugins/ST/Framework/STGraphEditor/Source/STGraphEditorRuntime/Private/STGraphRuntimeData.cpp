#include "STGraphRuntimeData.h"

#include "STGraphNodeInfo.h"

const FSTGraphOutputPinInfo* FSTRuntimeNode::FindOutputPinInfo(const FGuid& PinId) const {
    const USTGraphMiddleNodeInfo* Info = Cast<USTGraphMiddleNodeInfo>(NodeInfo.Get());
    if (!Info) {
        return nullptr;
    }

    return Info->Outputs.FindByPredicate(
        [&PinId](const FSTGraphOutputPinInfo& Output) { return Output.PinId == PinId; });
}

const FSTRuntimeNode* FSTRuntimeGraph::FindNode(const FGuid& NodeId) const {
    return Nodes.FindByPredicate([&NodeId](const FSTRuntimeNode& Node) { return Node.NodeId == NodeId; });
}
