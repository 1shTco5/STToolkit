#include "Graph/STGraphNodeTypes.h"

#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphPin.h"
#include "Framework/Commands/UIAction.h"
#include "Graph/STGraphSchema.h"
#include "STGraphNodeInfo.h"
#include "ToolMenu.h"

namespace {
bool IsFlowOutputPin(const UEdGraphPin* Pin) {
    return Pin && !Pin->ParentPin && Pin->Direction == EGPD_Output &&
           Pin->PinType.PinCategory == USTGraphSchema::FlowPinCategory;
}
}  // namespace

#pragma region UEdGraphNode Interface
void USTGraphStartNode::AllocateDefaultPins() {
    CreatePin(EGPD_Output, USTGraphSchema::FlowPinCategory, FName(TEXT("Out")));
}

void USTGraphMiddleNode::AllocateDefaultPins() {
    CreatePin(EGPD_Input, USTGraphSchema::FlowPinCategory, FName(TEXT("In")))->bDefaultValueIsIgnored = true;
    CreatePin(EGPD_Output, USTGraphSchema::FlowPinCategory, FName(TEXT("Out")));
    SyncOutputInfoFromPins();
}

void USTGraphEndNode::AllocateDefaultPins() {
    CreatePin(EGPD_Input, USTGraphSchema::FlowPinCategory, FName(TEXT("In")))->bDefaultValueIsIgnored = true;
}
#pragma endregion

#pragma region USTGraphNodeBase Interface
void USTGraphMiddleNode::PostLoad() {
    Super::PostLoad();
    if (!IsTemplate()) {
        SyncOutputInfoFromPins();
        ApplyOutputDisplayNames();
    }
}

void USTGraphMiddleNode::OnNodeInfoChanged() { ApplyOutputDisplayNames(); }

TSubclassOf<USTGraphNodeInfoBase> USTGraphStartNode::GetNodeInfoClass() const {
    return USTGraphStartNodeInfo::StaticClass();
}

TSubclassOf<USTGraphNodeInfoBase> USTGraphMiddleNode::GetNodeInfoClass() const {
    return USTGraphMiddleNodeInfo::StaticClass();
}

TSubclassOf<USTGraphNodeInfoBase> USTGraphEndNode::GetNodeInfoClass() const {
    return USTGraphEndNodeInfo::StaticClass();
}

void USTGraphMiddleNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const {
    Super::GetNodeContextMenuActions(Menu, Context);
    if (Context->bIsDebugging || !Context->Graph || !Context->Graph->bEditable) {
        return;
    }

    USTGraphMiddleNode* Node = const_cast<USTGraphMiddleNode*>(this);
    if (!Context->Pin) {
        Menu->FindOrAddSection(TEXT("STGraphNodeActions"))
            .AddMenuEntry(TEXT("STGraphAddOutputPin"), FText::FromString(TEXT("Add Output Pin")),
                          FText::FromString(TEXT("Add a flow output to this node")), FSlateIcon(),
                          FUIAction(FExecuteAction::CreateUObject(Node, &USTGraphMiddleNode::AddOutputPin)));
    } else if (IsFlowOutputPin(Context->Pin)) {
        Menu->FindOrAddSection(TEXT("STGraphNodeActions"))
            .AddMenuEntry(TEXT("STGraphRemoveOutputPin"), FText::FromString(TEXT("Remove Output Pin")),
                          FText::FromString(TEXT("Remove this flow output and its connections")), FSlateIcon(),
                          FUIAction(FExecuteAction::CreateUObject(Node, &USTGraphMiddleNode::RemoveOutputPin,
                                                                  FEdGraphPinReference(Context->Pin))));
    }
}
#pragma endregion

void USTGraphMiddleNode::SyncOutputInfoFromPins() {
    USTGraphMiddleNodeInfo* Info = Cast<USTGraphMiddleNodeInfo>(GetNodeInfo());
    if (!Info) {
        return;
    }

    TArray<FSTGraphOutputPinInfo> Outputs;
    for (const UEdGraphPin* Pin : Pins) {
        if (!IsFlowOutputPin(Pin)) {
            continue;
        }

        const FSTGraphOutputPinInfo* Existing = Info->Outputs.FindByPredicate(
            [Pin](const FSTGraphOutputPinInfo& Entry) { return Entry.PinId == Pin->PinId; });
        if (Existing) {
            Outputs.Add(*Existing);
        } else {
            FSTGraphOutputPinInfo& Output = Outputs.AddDefaulted_GetRef();
            Output.PinId = Pin->PinId;
            Output.DisplayName = Pin->GetDisplayName();
        }
    }
    Info->Outputs = MoveTemp(Outputs);
}

void USTGraphMiddleNode::ApplyOutputDisplayNames() {
    USTGraphMiddleNodeInfo* Info = Cast<USTGraphMiddleNodeInfo>(GetNodeInfo());
    if (!Info) {
        return;
    }

    for (UEdGraphPin* Pin : Pins) {
        if (!IsFlowOutputPin(Pin)) {
            continue;
        }

        FSTGraphOutputPinInfo* Output = Info->Outputs.FindByPredicate(
            [Pin](const FSTGraphOutputPinInfo& Entry) { return Entry.PinId == Pin->PinId; });
        if (!Output) {
            continue;
        }

        const FText DefaultName = FText::FromName(Pin->PinName);
        if (Output->DisplayName.IsEmpty()) {
            Output->DisplayName = DefaultName;
        }
        Pin->PinFriendlyName = Output->DisplayName.EqualTo(DefaultName) ? FText::GetEmpty() : Output->DisplayName;
    }
}

void USTGraphMiddleNode::AddOutputPin() {
    UEdGraph* Graph = GetGraph();
    if (!Graph || !Graph->bEditable || !Graph->Nodes.Contains(this)) {
        return;
    }

    CreatePin(EGPD_Output, USTGraphSchema::FlowPinCategory, CreateUniquePinName(FName(TEXT("Out"))));
    SyncOutputInfoFromPins();
    Graph->MarkPackageDirty();
    Graph->NotifyNodeChanged(this);
}

void USTGraphMiddleNode::RemoveOutputPin(FEdGraphPinReference PinReference) {
    UEdGraph* Graph = GetGraph();
    UEdGraphPin* Pin = PinReference.Get();
    if (!Graph || !Graph->bEditable || !Graph->Nodes.Contains(this) || !IsFlowOutputPin(Pin) || !Pins.Contains(Pin)) {
        return;
    }

    Pin->BreakAllPinLinks(true);
    if (RemovePin(Pin)) {
        SyncOutputInfoFromPins();
        Graph->MarkPackageDirty();
        Graph->NotifyNodeChanged(this);
    }
}
