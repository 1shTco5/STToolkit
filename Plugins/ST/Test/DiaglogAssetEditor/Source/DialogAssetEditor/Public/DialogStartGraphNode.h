#pragma once

#include "CoreMinimal.h"
#include "DialogGraphNodeBase.h"
#include "DialogStartGraphNode.generated.h"

UCLASS()
class UDialogStartGraphNode : public UDialogGraphNodeBase {
    GENERATED_BODY()

public:
    // ~Begin UEdGraphNode Interface
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString("Start"); }
    virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor(FColor::Red); }
    virtual bool CanUserDeleteNode() const override { return false; };
    // ~End UEdGraphNode Interface
    
    // ~Begin DialogGraphNodeBase Interface
    virtual UEdGraphPin* CreateDialogPin(EEdGraphPinDirection Direction, FName Name) override;
    virtual UEdGraphPin* CreateDefaultInputPin() override; 
    virtual EDialogNodeType GetDialogNodeType() const override { return EDialogNodeType::StartNode; }
    // ~End DialogGraphNodeBase Interface
};