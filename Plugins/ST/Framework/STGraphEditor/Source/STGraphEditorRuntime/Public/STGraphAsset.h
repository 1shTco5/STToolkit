#pragma once

#include "UObject/Object.h"
#include "STGraphAsset.generated.h"

#if WITH_EDITORONLY_DATA
class UEdGraph;
#endif

UCLASS(Abstract)
class STGRAPHEDITORRUNTIME_API USTGraphAsset : public UObject {
    GENERATED_BODY()

#if WITH_EDITORONLY_DATA
public:
    UEdGraph* GetEditorGraph() const;

private:
    friend class USTGraphAssetFactory;

    UPROPERTY()
    TObjectPtr<UEdGraph> EditorGraph;
#endif
};
