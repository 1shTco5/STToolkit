#include "DialogEndGraphNode.h"

FText UDialogEndGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    if (m_NodeInfo && m_NodeInfo->Action != EDialogNodeAction::None) {
        FString Result = UEnum::GetDisplayValueAsText(m_NodeInfo->Action).ToString();
        if (!m_NodeInfo->ActionData.IsEmpty()) {
            FString ActionData = m_NodeInfo->ActionData;
            if (ActionData.Len() > 15) {
                ActionData = ActionData.Left(15) + TEXT("...");
            }
            Result += TEXT(" - ") + ActionData;
            return FText::FromString(Result);
        }
    }
    return FText::FromString("End");
}

void UDialogEndGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu,
                                                    class UGraphNodeContextMenuContext* Context) const {
    FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString("Dialog Node Actions"));

    UDialogEndGraphNode* Node = const_cast<UDialogEndGraphNode*>(this);
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

UEdGraphPin* UDialogEndGraphNode::CreateDialogPin(EEdGraphPinDirection Direction, FName Name) {
    FName Category = TEXT("Inputs");
    FName SubCategory = TEXT("EndPin");

    UEdGraphPin* Pin = CreatePin(Direction, Category, Name);
    Pin->PinType.PinSubCategory = SubCategory;
    return Pin;
}

UEdGraphPin* UDialogEndGraphNode::CreateDefaultInputPin() {
    return CreateDialogPin(EGPD_Input, FName("Finish"));
}