#pragma once

#include "CoreMinimal.h"
#include "DialogEndNodeInfo.h"
#include "DialogGraphNodeBase.h"
#include "DialogEndGraphNode.generated.h"


UCLASS()
class UDialogEndGraphNode : public UDialogGraphNodeBase {
    GENERATED_BODY()

public:
    // ~Begin UEdGraphNode Interface
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Blue); }
    virtual bool CanUserDeleteNode() const override { return true; };
    virtual void GetNodeContextMenuActions(
        class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
    // ~End UEdGraphNode Interface

    // ~Begin DialogGraphNodeBase Interface
    virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection Direction, FName Name) override;
    virtual UEdGraphPin* CreateDefaultInputPin() override;
    virtual void InitNodeInfo(UObject* Outer) override { m_NodeInfo = NewObject<UDialogEndNodeInfo>(Outer); }

    virtual void SetNodeInfo(UDialogNodeInfoBase* NodeInfo) override {
        m_NodeInfo = Cast<UDialogEndNodeInfo>(NodeInfo);
    }

    virtual UDialogNodeInfoBase* GetNodeInfo() const override { return m_NodeInfo; }
    virtual EDialogNodeType GetDialogNodeType() const override { return EDialogNodeType::EndNode; }
    virtual void OnPropertiesChanged() override { Modify(); }
    // ~End DialogGraphNodeBase Interface

private:
    UPROPERTY()
    UDialogEndNodeInfo* m_NodeInfo;
};