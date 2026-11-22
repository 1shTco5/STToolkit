#include "DialogGraphNode.h"

#include "DialogNodeInfo.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"

FText UDialogGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    if (m_NodeInfo->Title.IsEmpty()) {
        FString DialogTextStr = m_NodeInfo->DialogText.ToString();
        if (DialogTextStr.Len() > 15) {
            DialogTextStr = DialogTextStr.Left(15) + TEXT("...");
        }
        return FText::FromString(DialogTextStr);
    }
    return m_NodeInfo->Title;
}

void UDialogGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu,
                                                 class UGraphNodeContextMenuContext* Context) const {
    FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString("Dialog Node Actions"));

    UDialogGraphNode* Node = const_cast<UDialogGraphNode*>(this);
    Section.AddMenuEntry(
        TEXT("AddPinEntry"),
        FText::FromString("Add Pin"),
        FText::FromString("Create a new pin"),
        FSlateIcon(TEXT("DialogAssetEditorStyle"), TEXT("DialogAssetEditor.AddPinIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [Node]() {
                //Node->CreatePin(EGPD_Output, TEXT("Outputs"), TEXT("Another Output"));
                //Node->CreateDialogPin(EGPD_Output, TEXT("Another Output"));
                Node->GetDialogNodeInfo()->DialogResponses.Add(FText::FromString("Response"));
                Node->SyncPinsWithResponses();
                
                Node->GetGraph()->NotifyGraphChanged();
                Node->GetGraph()->Modify();
            }))
        );

    Section.AddMenuEntry(
        TEXT("RemovePinEntry"),
        FText::FromString("Remove Pin"),
        FText::FromString("Remove a pin"),
        FSlateIcon(TEXT("DialogAssetEditorStyle"), TEXT("DialogAssetEditor.RemovePinIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [Node]() {
                UEdGraphPin* Pin = Node->GetPinAt(Node->Pins.Num() - 1);
                if (Pin->Direction != EGPD_Input) {
                    UDialogNodeInfo* NodeInfo = Node->GetDialogNodeInfo();
                    NodeInfo->DialogResponses.RemoveAt(NodeInfo->DialogResponses.Num() - 1);
                    Node->SyncPinsWithResponses();

                    Node->GetGraph()->NotifyGraphChanged();
                    Node->GetGraph()->Modify();
                }
            }))
        );

    Section.AddMenuEntry(
        TEXT("DeleteNodeEntry"),
        FText::FromString("Delete Node"),
        FText::FromString("Delete this Node"),
        FSlateIcon(TEXT("DialogAssetEditorStyle"), TEXT("DialogAssetEditor.DeleteNodeIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [Node]() {
                Node->GetGraph()->RemoveNode(Node);
            }))
        );
}

UEdGraphPin* UDialogGraphNode::CreateDialogPin(EEdGraphPinDirection Direction, FName Name) {
    FName Category = (Direction == EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
    FName Subcategory = TEXT("DialogPin");

    UEdGraphPin* Pin = CreatePin(
        Direction,
        Category,
        Name
        );
    Pin->PinType.PinSubCategory = Subcategory;

    return Pin;
}

UEdGraphPin* UDialogGraphNode::CreateDefaultInputPin() {
    return CreateDialogPin(EGPD_Input, FName("Diaplay"));
}

void UDialogGraphNode::CreateDefaultOutputPins() {
    m_NodeInfo->DialogResponses.Add(FText::FromString("Continue"));
    OnPropertiesChanged();
}

void UDialogGraphNode::SyncPinsWithResponses() {
    // Sync the pins on the node with the dialog responses
    // We're going to assume the first pin is always the
    // input pin
    UDialogNodeInfo* NodeInfo = GetDialogNodeInfo();
    int NumGraphNodePins = Pins.Num() - 1;
    int NumInfoPins = NodeInfo->DialogResponses.Num();

    while (NumGraphNodePins > NumInfoPins) {
        RemovePinAt(NumGraphNodePins - 1, EGPD_Output);
        NumGraphNodePins--;
    }
    while (NumInfoPins > NumGraphNodePins) {
        CreateDialogPin(
            EGPD_Output,
            FName(NodeInfo->DialogResponses[NumGraphNodePins].ToString())
            );
        NumGraphNodePins++;
    }

    int Index = 1;
    for (const FText& Option : NodeInfo->DialogResponses) {
        GetPinAt(Index)->PinName = FName(Option.ToString());
        Index++;
    }
}