#pragma once

#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "Misc/Guid.h"
#include "UObject/ObjectPtr.h"
#include "STGraphRuntimeData.generated.h"

class USTGraphNodeInfoBase;
struct FSTGraphOutputPinInfo;

UENUM(BlueprintType)
enum class ESTGraphNodeRole : uint8 {
    Start,
    Middle,
    End,
    Unknown,
};

USTRUCT(BlueprintType)
struct STGRAPHEDITORRUNTIME_API FSTRuntimeOutputPin {
    GENERATED_BODY()

    // Matches the source output pin and its entry in NodeInfo, if one exists.
    UPROPERTY(BlueprintReadOnly, Category = "Graph")
    FGuid PinId;

    UPROPERTY(BlueprintReadOnly, Category = "Graph")
    TArray<FGuid> TargetNodeIds;
};

USTRUCT(BlueprintType)
struct STGRAPHEDITORRUNTIME_API FSTRuntimeNode {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Graph")
    FGuid NodeId;

    UPROPERTY(BlueprintReadOnly, Category = "Graph")
    ESTGraphNodeRole Role = ESTGraphNodeRole::Unknown;

    // The runtime asset owns this copy of the node data, including any output payloads.
    UPROPERTY(Instanced, BlueprintReadOnly, Category = "Graph")
    TObjectPtr<USTGraphNodeInfoBase> NodeInfo;

    UPROPERTY(BlueprintReadOnly, Category = "Graph")
    TArray<FSTRuntimeOutputPin> OutputPins;

    const FSTGraphOutputPinInfo* FindOutputPinInfo(const FGuid& PinId) const;
};

USTRUCT()
struct STGRAPHEDITORRUNTIME_API FSTRuntimeGraph {
    GENERATED_BODY()

    UPROPERTY()
    TArray<FSTRuntimeNode> Nodes;

    TConstArrayView<FSTRuntimeNode> GetNodes() const { return Nodes; }
    const FSTRuntimeNode* FindNode(const FGuid& NodeId) const;
};
