#include "Graph/STGraphNodeBase.h"

#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphPin.h"
#include "Framework/Commands/UIAction.h"
#include "Graph/STGraphSchema.h"
#include "STGraphNodeInfo.h"
#include "ToolMenu.h"

USTGraphNodeInfoBase* USTGraphNodeBase::GetNodeInfo() const { return NodeInfo.Get(); }

TSubclassOf<USTGraphNodeInfoBase> USTGraphNodeBase::GetNodeInfoClass() const {
    return USTGraphNodeInfoBase::StaticClass();
}

FText USTGraphNodeBase::GetDefaultNodeTitle(ENodeTitleType::Type TitleType) const {
    return Super::GetNodeTitle(TitleType);
}

void USTGraphNodeBase::InitializeNodeInfo() {
    if (!NodeInfo && !IsTemplate()) {
        const TSubclassOf<USTGraphNodeInfoBase> InfoClass = GetNodeInfoClass();
        if (InfoClass && !InfoClass->HasAnyClassFlags(CLASS_Abstract)) {
            NodeInfo = NewObject<USTGraphNodeInfoBase>(this, InfoClass);
        }
    }
}

#pragma region UEdGraphNode Interface
void USTGraphNodeBase::PostLoad() {
    Super::PostLoad();
    InitializeNodeInfo();
}

void USTGraphNodeBase::PostPlacedNewNode() {
    Super::PostPlacedNewNode();
    InitializeNodeInfo();
}

FText USTGraphNodeBase::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    return NodeInfo && !NodeInfo->CustomTitle.IsEmpty() ? NodeInfo->CustomTitle : GetDefaultNodeTitle(TitleType);
}

void USTGraphNodeBase::AutowireNewNode(UEdGraphPin* FromPin) {
    const UEdGraphSchema* Schema = GetSchema();
    if (!Schema || !FromPin || FromPin->PinType.PinCategory != USTGraphSchema::FlowPinCategory) {
        return;
    }

    for (UEdGraphPin* Pin : Pins) {
        if (Pin && Pin->Direction != FromPin->Direction &&
            Pin->PinType.PinCategory == USTGraphSchema::FlowPinCategory && Schema->TryCreateConnection(FromPin, Pin)) {
            return;
        }
    }
}

void USTGraphNodeBase::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const {
    Super::GetNodeContextMenuActions(Menu, Context);
    if (Context->Pin || Context->bIsDebugging || !Context->Graph || !Context->Graph->bEditable ||
        !CanUserDeleteNode()) {
        return;
    }

    const TWeakObjectPtr<USTGraphNodeBase> WeakNode(const_cast<USTGraphNodeBase*>(this));
    Menu->FindOrAddSection(TEXT("STGraphNodeActions"))
        .AddMenuEntry(
            TEXT("STGraphDeleteNode"), FText::FromString(TEXT("Delete Node")),
            FText::FromString(TEXT("Delete this node and its connections")), FSlateIcon(),
            FUIAction(FExecuteAction::CreateLambda([WeakNode] {
                USTGraphNodeBase* Node = WeakNode.Get();
                UEdGraph* Graph = Node ? Node->GetGraph() : nullptr;
                if (!Graph || !Graph->bEditable || !Graph->Nodes.Contains(Node) || !Node->CanUserDeleteNode()) {
                    return;
                }

                Node->DestroyNode();
            })));
}
#pragma endregion
