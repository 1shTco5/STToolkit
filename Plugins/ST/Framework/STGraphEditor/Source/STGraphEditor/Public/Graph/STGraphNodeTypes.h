#pragma once

#include "Graph/STGraphNodeBase.h"
#include "STGraphNodeTypes.generated.h"

struct FEdGraphPinReference;

UCLASS(Abstract)
class STGRAPHEDITOR_API USTGraphStartNode : public USTGraphNodeBase {
    GENERATED_BODY()

public:
    //~Begin UEdGraphNode Interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Red; }
    //~End UEdGraphNode Interface

protected:
    //~Begin USTGraphNodeBase Interface
    virtual TSubclassOf<USTGraphNodeInfoBase> GetNodeInfoClass() const override;
    virtual FText GetDefaultNodeTitle(ENodeTitleType::Type TitleType) const override {
        return FText::FromString(TEXT("Start"));
    }
    //~End USTGraphNodeBase Interface
};

UCLASS(Abstract)
class STGRAPHEDITOR_API USTGraphMiddleNode : public USTGraphNodeBase {
    GENERATED_BODY()

public:
    //~Begin UEdGraphNode Interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Orange); }
    //~End UEdGraphNode Interface

    //~Begin USTGraphNodeBase Interface
    virtual void PostLoad() override;
    virtual void OnNodeInfoChanged() override;
    virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
    virtual TSubclassOf<USTGraphNodeInfoBase> GetNodeInfoClass() const override;
    virtual FText GetDefaultNodeTitle(ENodeTitleType::Type TitleType) const override {
        return FText::FromString(TEXT("Middle"));
    }
    //~End USTGraphNodeBase Interface

private:
    void SyncOutputInfoFromPins();
    void ApplyOutputDisplayNames();
    void AddOutputPin();
    void RemoveOutputPin(FEdGraphPinReference PinReference);
};

UCLASS(Abstract)
class STGRAPHEDITOR_API USTGraphEndNode : public USTGraphNodeBase {
    GENERATED_BODY()

public:
    //~Begin UEdGraphNode Interface
    virtual void AllocateDefaultPins() override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Blue; }
    //~End UEdGraphNode Interface

protected:
    //~Begin USTGraphNodeBase Interface
    virtual TSubclassOf<USTGraphNodeInfoBase> GetNodeInfoClass() const override;
    virtual FText GetDefaultNodeTitle(ENodeTitleType::Type TitleType) const override {
        return FText::FromString(TEXT("End"));
    }
    //~End USTGraphNodeBase Interface
};
