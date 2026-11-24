#include "STGraphAsset.h"

#if WITH_EDITORONLY_DATA
#include "EdGraph/EdGraph.h"

UEdGraph* USTGraphAsset::GetEditorGraph() const { return EditorGraph.Get(); }
#endif
