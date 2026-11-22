#pragma once

#include "EdGraph/EdGraphNode.h"
#include "DialogRuntimeGraph.h"
#include "DialogGraphNodeBase.generated.h"

UCLASS()
class UDialogGraphNodeBase : public UEdGraphNode {
    GENERATED_BODY()

public:
    virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection Direction, FName Name) { return nullptr; }
    virtual UEdGraphPin* CreateDefaultInputPin() { return nullptr; }
    virtual void CreateDefaultOutputPins() { return; }
    virtual void InitNodeInfo(UObject* Outer) { return; }
    virtual void SetNodeInfo(UDialogNodeInfoBase* NodeInfo) { return; }
    virtual UDialogNodeInfoBase* GetNodeInfo() const { return nullptr; }
    virtual EDialogNodeType GetDialogNodeType() const { return EDialogNodeType::Unknown; }
    virtual void OnPropertiesChanged() { return; }
};