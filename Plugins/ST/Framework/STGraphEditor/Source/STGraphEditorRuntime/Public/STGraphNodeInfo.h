#pragma once

#include "UObject/Object.h"
#include "Misc/Guid.h"
#include "StructUtils/InstancedStruct.h"
#include "STGraphNodeInfo.generated.h"

USTRUCT(BlueprintType)
struct STGRAPHEDITORRUNTIME_API FSTGraphOutputPinInfo {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Pin")
    FGuid PinId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pin")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pin", meta = (StructTypeConst, HideStructType))
    FInstancedStruct Payload;
};

UCLASS(BlueprintType)
class STGRAPHEDITORRUNTIME_API USTGraphNodeInfoBase : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Node")
    FText CustomTitle;
};

UCLASS()
class STGRAPHEDITORRUNTIME_API USTGraphStartNodeInfo : public USTGraphNodeInfoBase {
    GENERATED_BODY()
};

UCLASS()
class STGRAPHEDITORRUNTIME_API USTGraphMiddleNodeInfo : public USTGraphNodeInfoBase {
    GENERATED_BODY()

public:
    // The graph owns the pins. Each entry stores editable settings for an existing output pin.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, EditFixedSize, Category = "Pins",
              meta = (EditFixedOrder, TitleProperty = "DisplayName"))
    TArray<FSTGraphOutputPinInfo> Outputs;
};

UCLASS()
class STGRAPHEDITORRUNTIME_API USTGraphEndNodeInfo : public USTGraphNodeInfoBase {
    GENERATED_BODY()
};
