#include "STExampleGraphAsset.h"

#pragma region UAssetDefinition Interface
FText UAssetDefinition_STExampleGraphAsset::GetAssetDisplayName() const {
    return FText::FromString(TEXT("ST Example Graph"));
}

TSoftClassPtr<UObject> UAssetDefinition_STExampleGraphAsset::GetAssetClass() const {
    return USTExampleGraphAsset::StaticClass();
}

#pragma endregion

#pragma region USTGraphMiddleNode Interface
void USTExampleGraphNode::PostLoad() {
    Super::PostLoad();
    if (IsTemplate()) {
        return;
    }

    USTExampleMiddleNodeInfo* Info = Cast<USTExampleMiddleNodeInfo>(NodeInfo);
    if (!Info) {
        Info = NewObject<USTExampleMiddleNodeInfo>(this);
        if (NodeInfo) {
            Info->CustomTitle = NodeInfo->CustomTitle;
            if (const USTGraphMiddleNodeInfo* OldInfo = Cast<USTGraphMiddleNodeInfo>(NodeInfo)) {
                Info->Outputs = OldInfo->Outputs;
            }
        }
        NodeInfo = Info;
    }
    if (Info->Description.IsEmpty()) {
        Info->Description = MoveTemp(Description_DEPRECATED);
    }

    if (Pins.IsEmpty()) {
        AllocateDefaultPins();
    }
}

TSubclassOf<USTGraphNodeInfoBase> USTExampleGraphNode::GetNodeInfoClass() const {
    return USTExampleMiddleNodeInfo::StaticClass();
}

const UScriptStruct* USTExampleGraphNode::GetOutputPayloadStruct() const {
    return FSTExampleOutputPinPayload::StaticStruct();
}

#pragma endregion

#pragma region USTGraphSchema Interface
void USTExampleGraphSchema::GetNodeMenuEntries(TArray<FSTGraphNodeMenuEntry>& OutEntries) const {
    OutEntries.Add({USTExampleStartNode::StaticClass(), ESTGraphNodeRole::Start,
                    FText::FromString(TEXT("Add Start Node")), FText::FromString(TEXT("Create a flow entry node"))});
    OutEntries.Add({USTExampleGraphNode::StaticClass(), ESTGraphNodeRole::Middle,
                    FText::FromString(TEXT("Add Middle Node")),
                    FText::FromString(TEXT("Create a node with dynamic flow outputs"))});
    OutEntries.Add({USTExampleEndNode::StaticClass(), ESTGraphNodeRole::End, FText::FromString(TEXT("Add End Node")),
                    FText::FromString(TEXT("Create a flow end node"))});
}

#pragma endregion

USTExampleGraphAssetFactory::USTExampleGraphAssetFactory() {
    SupportedClass = USTExampleGraphAsset::StaticClass();
    bCreateNew = true;
}

#pragma region USTGraphAssetFactory Interface
TSubclassOf<UEdGraphSchema> USTExampleGraphAssetFactory::GetGraphSchemaClass() const {
    return USTExampleGraphSchema::StaticClass();
}
#pragma endregion
