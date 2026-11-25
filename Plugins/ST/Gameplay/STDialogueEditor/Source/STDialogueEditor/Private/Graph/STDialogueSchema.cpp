#include "Graph/STDialogueSchema.h"

#include "Graph/STDialogueNodeTypes.h"

#pragma region USTGraphSchema Interface
void USTDialogueSchema::GetNodeMenuEntries(TArray<FSTGraphNodeMenuEntry>& OutEntries) const {
    OutEntries.Add({USTDialogueStartNode::StaticClass(), ESTGraphNodeRole::Start,
                    FText::FromString(TEXT("Add Start Node")),
                    FText::FromString(TEXT("Create a dialogue entry node"))});
    OutEntries.Add({USTDialogueMiddleNode::StaticClass(), ESTGraphNodeRole::Middle,
                    FText::FromString(TEXT("Add Dialogue Node")), FText::FromString(TEXT("Create a dialogue node"))});
    OutEntries.Add({USTDialogueEndNode::StaticClass(), ESTGraphNodeRole::End, FText::FromString(TEXT("Add End Node")),
                    FText::FromString(TEXT("Create a dialogue end node"))});
}
#pragma endregion
