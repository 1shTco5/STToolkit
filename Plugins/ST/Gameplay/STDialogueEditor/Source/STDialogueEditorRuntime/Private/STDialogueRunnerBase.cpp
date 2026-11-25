#include "STDialogueRunnerBase.h"

#include "STDialogueAsset.h"

bool USTDialogueRunnerBase::Start(USTDialogueAsset* InDialogueAsset) {
    if (!IsValid(InDialogueAsset)) {
        return false;
    }

    const FSTRuntimeGraph& RuntimeGraph = InDialogueAsset->GetRuntimeGraph();
    const FSTRuntimeNode* StartNode = nullptr;
    for (const FSTRuntimeNode& Node : RuntimeGraph.GetNodes()) {
        if (Node.Role == ESTGraphNodeRole::Start) {
            StartNode = &Node;
            break;
        }
    }

    if (!StartNode || StartNode->OutputPins.Num() != 1) {
        return false;
    }

    USTDialogueAsset* PreviousAsset = DialogueAsset.Get();
    const FGuid PreviousNodeId = CurrentNodeId;
    DialogueAsset = InDialogueAsset;
    CurrentNodeId = StartNode->NodeId;
    if (ChooseResponse(StartNode->OutputPins[0].PinId)) {
        return true;
    }

    DialogueAsset = PreviousAsset;
    CurrentNodeId = PreviousNodeId;
    return false;
}

FSTRuntimeNode USTDialogueRunnerBase::GetCurrentNode() const {
    if (!DialogueAsset || !CurrentNodeId.IsValid()) {
        return {};
    }

    const FSTRuntimeNode* Node = DialogueAsset->GetRuntimeGraph().FindNode(CurrentNodeId);
    return Node ? *Node : FSTRuntimeNode{};
}

bool USTDialogueRunnerBase::GetResponsePayload(int32 OutputIndex, FSTDialogueOutputPinPayload& OutPayload,
                                               FGuid& OutPinId) const {
    OutPayload = {};
    OutPinId.Invalidate();
    if (!DialogueAsset || !CurrentNodeId.IsValid()) {
        return false;
    }

    const FSTRuntimeNode* Node = DialogueAsset->GetRuntimeGraph().FindNode(CurrentNodeId);
    if (!Node || !Node->OutputPins.IsValidIndex(OutputIndex)) {
        return false;
    }

    const FGuid& PinId = Node->OutputPins[OutputIndex].PinId;
    const FSTGraphOutputPinInfo* OutputInfo = PinId.IsValid() ? Node->FindOutputPinInfo(PinId) : nullptr;
    const FSTDialogueOutputPinPayload* Payload =
        OutputInfo ? OutputInfo->Payload.GetPtr<FSTDialogueOutputPinPayload>() : nullptr;
    if (!Payload) {
        return false;
    }

    OutPayload = *Payload;
    OutPinId = PinId;
    return true;
}

bool USTDialogueRunnerBase::ChooseResponse(const FGuid& PinId) {
    const FSTRuntimeNode CurrentNode = GetCurrentNode();
    if (!PinId.IsValid() ||
        (CurrentNode.Role != ESTGraphNodeRole::Start && CurrentNode.Role != ESTGraphNodeRole::Middle)) {
        return false;
    }

    const FSTRuntimeOutputPin* OutputPin = CurrentNode.OutputPins.FindByPredicate(
        [&PinId](const FSTRuntimeOutputPin& Output) { return Output.PinId == PinId; });
    if (!OutputPin || OutputPin->TargetNodeIds.Num() != 1 || !OutputPin->TargetNodeIds[0].IsValid()) {
        return false;
    }

    const FSTRuntimeNode* TargetNode = DialogueAsset->GetRuntimeGraph().FindNode(OutputPin->TargetNodeIds[0]);
    if (!TargetNode) {
        return false;
    }

    const bool bCanEnterEnd = CurrentNode.Role == ESTGraphNodeRole::Middle && TargetNode->Role == ESTGraphNodeRole::End;
    if (TargetNode->Role != ESTGraphNodeRole::Middle && !bCanEnterEnd) {
        return false;
    }

    CurrentNodeId = TargetNode->NodeId;
    OnCurrentNodeChanged(*TargetNode);
    return true;
}

void USTDialogueRunnerBase::OnCurrentNodeChanged_Implementation(const FSTRuntimeNode& /*Node*/) {}
