#pragma once

#include "EdGraph/EdGraphNode.h"
#include "STGraphRuntimeData.h"
#include "STGraphNodeBase.generated.h"

class USTGraphNodeInfoBase;

UCLASS(Abstract)
class STGRAPHEDITOR_API USTGraphNodeBase : public UEdGraphNode {
    GENERATED_BODY()

public:
    USTGraphNodeInfoBase* GetNodeInfo() const;
    virtual ESTGraphNodeRole GetNodeRole() const { return ESTGraphNodeRole::Unknown; }
    virtual void OnNodeInfoChanged() {}

    //~Begin UEdGraphNode Interface
    virtual void PostLoad() override;
    virtual void PostPlacedNewNode() override;
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
    virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
    //~End UEdGraphNode Interface

protected:
    virtual TSubclassOf<USTGraphNodeInfoBase> GetNodeInfoClass() const;
    virtual FText GetDefaultNodeTitle(ENodeTitleType::Type TitleType) const;

    UPROPERTY(Instanced)
    TObjectPtr<USTGraphNodeInfoBase> NodeInfo;

private:
    void InitializeNodeInfo();
};
