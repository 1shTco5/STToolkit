#pragma once

#include "Asset/STGraphAssetDefinition.h"
#include "Asset/STGraphAssetFactory.h"
#include "Graph/STGraphNodeTypes.h"
#include "Graph/STGraphSchema.h"
#include "STGraphAsset.h"
#include "STGraphNodeInfo.h"
#include "STExampleGraphAsset.generated.h"

UCLASS()
class USTExampleGraphAsset : public USTGraphAsset {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString SomeString;

    UPROPERTY(EditAnywhere)
    bool SomeBool;

    UPROPERTY(EditAnywhere)
    int32 SomeNumber;
};

UCLASS()
class UAssetDefinition_STExampleGraphAsset : public USTGraphAssetDefinition {
    GENERATED_BODY()

public:
    //~Begin UAssetDefinition Interface
    virtual FText GetAssetDisplayName() const override;
    virtual TSoftClassPtr<UObject> GetAssetClass() const override;
    //~End UAssetDefinition Interface
};

UCLASS()
class USTExampleStartNode : public USTGraphStartNode {
    GENERATED_BODY()
};

UCLASS()
class USTExampleMiddleNodeInfo : public USTGraphMiddleNodeInfo {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Node")
    FString Description;
};

UCLASS()
class USTExampleGraphNode : public USTGraphMiddleNode {
    GENERATED_BODY()

public:
    //~Begin USTGraphMiddleNode Interface
    virtual void PostLoad() override;

protected:
    virtual TSubclassOf<USTGraphNodeInfoBase> GetNodeInfoClass() const override;
    //~End USTGraphMiddleNode Interface

private:
    UPROPERTY()
    FString Description_DEPRECATED;
};

UCLASS()
class USTExampleEndNode : public USTGraphEndNode {
    GENERATED_BODY()
};

UCLASS()
class USTExampleGraphSchema : public USTGraphSchema {
    GENERATED_BODY()

protected:
    //~Begin USTGraphSchema Interface
    virtual void GetNodeMenuEntries(TArray<FSTGraphNodeMenuEntry>& OutEntries) const override;
    //~End USTGraphSchema Interface
};

UCLASS()
class USTExampleGraphAssetFactory : public USTGraphAssetFactory {
    GENERATED_BODY()

public:
    USTExampleGraphAssetFactory();

protected:
    //~Begin USTGraphAssetFactory Interface
    virtual TSubclassOf<UEdGraphSchema> GetGraphSchemaClass() const override;
    //~End USTGraphAssetFactory Interface
};
