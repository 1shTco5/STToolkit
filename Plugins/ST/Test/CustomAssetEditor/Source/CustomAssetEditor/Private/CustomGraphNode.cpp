#include "CustomGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"

void UCustomGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu,
                                                 class UGraphNodeContextMenuContext* Context) const {
    FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString("Custom Node Actions"));

    UCustomGraphNode* Node = const_cast<UCustomGraphNode*>(this);
    Section.AddMenuEntry(
        TEXT("AddPinEntry"),
        FText::FromString("Add Pin"),
        FText::FromString("Create a new pin"),
        FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.AddPinIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [Node]() {
                //Node->CreatePin(EGPD_Output, TEXT("Outputs"), TEXT("Another Output"));
                Node->CreateCustomPin(EGPD_Output, TEXT("Another Output"));

                Node->GetGraph()->NotifyGraphChanged();
                Node->GetGraph()->Modify();
            }))
        );

    Section.AddMenuEntry(
        TEXT("RemovePinEntry"),
        FText::FromString("Remove Pin"),
        FText::FromString("Remove a pin"),
        FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.RemovePinIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [Node]() {
                UEdGraphPin* Pin = Node->GetPinAt(Node->Pins.Num() - 1);
                if (Pin->Direction != EGPD_Input) {
                    Node->RemovePin(Pin);

                    Node->GetGraph()->NotifyGraphChanged();
                    Node->GetGraph()->Modify();
                }
            }))
        );

    Section.AddMenuEntry(
        TEXT("DeleteNodeEntry"),
        FText::FromString("Delete Node"),
        FText::FromString("Delete this Node"),
        FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.DeleteNodeIcon")),
        FUIAction(FExecuteAction::CreateLambda(
            [Node]() {
                Node->GetGraph()->RemoveNode(Node);
            }))
        );
}

UEdGraphPin* UCustomGraphNode::CreateCustomPin(EEdGraphPinDirection Direction, FName Name) {
    FName Category = (Direction == EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
    FName Subcategory = TEXT("CustomPin");

    UEdGraphPin* Pin = CreatePin(
        Direction,
        Category,
        Name
        );
    Pin->PinType.PinSubCategory = Subcategory;

    return Pin;
}