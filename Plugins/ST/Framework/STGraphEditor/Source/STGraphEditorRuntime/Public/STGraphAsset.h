#pragma once

#include "UObject/Object.h"
#include "STGraphRuntimeData.h"
#include "STGraphAsset.generated.h"

class UEdGraph;

UCLASS(Abstract)
class STGRAPHEDITORRUNTIME_API USTGraphAsset : public UObject {
    GENERATED_BODY()

public:
    const FSTRuntimeGraph& GetRuntimeGraph() const { return RuntimeGraph; }

private:
    UPROPERTY()
    FSTRuntimeGraph RuntimeGraph;

#if WITH_EDITORONLY_DATA
public:
    UEdGraph* GetEditorGraph() const;

private:
    friend class USTGraphAssetFactory;
    friend class FSTGraphEditorApp;

    UPROPERTY()
    TObjectPtr<UEdGraph> EditorGraph;
#endif
};
