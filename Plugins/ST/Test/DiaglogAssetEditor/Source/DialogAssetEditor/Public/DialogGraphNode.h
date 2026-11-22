#pragma once

#include "DialogGraphNodeBase.h"
#include "DialogNodeInfo.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogGraphNode.generated.h"

UCLASS()
class UDialogGraphNode : public UDialogGraphNodeBase {
    GENERATED_BODY()

public:
    // ~Begin UEdGraphNode Interface
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Orange); }
    virtual bool CanUserDeleteNode() const override { return true; };
    virtual void GetNodeContextMenuActions(
        class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
    // ~End UEdGraphNode Interface

    // ~Begin DialogGraphNodeBase Interface
    virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection Direction, FName Name) override;
    virtual UEdGraphPin* CreateDefaultInputPin() override;
    virtual void CreateDefaultOutputPins() override;
    virtual void InitNodeInfo(UObject* Outer) override { m_NodeInfo = NewObject<UDialogNodeInfo>(Outer); }
    virtual UDialogNodeInfoBase* GetNodeInfo() const override { return m_NodeInfo; }
    virtual void SetNodeInfo(UDialogNodeInfoBase* NodeInfo) override { m_NodeInfo = Cast<UDialogNodeInfo>(NodeInfo); };
    virtual EDialogNodeType GetDialogNodeType() const override { return EDialogNodeType::DialogNode; }
    virtual void OnPropertiesChanged() override { SyncPinsWithResponses(); }
    // ~End DialogGraphNodeBase Interface

public:
    UDialogNodeInfo* GetDialogNodeInfo() const { return m_NodeInfo; }
    void SyncPinsWithResponses();

private:
    UPROPERTY()
    UDialogNodeInfo* m_NodeInfo;
};