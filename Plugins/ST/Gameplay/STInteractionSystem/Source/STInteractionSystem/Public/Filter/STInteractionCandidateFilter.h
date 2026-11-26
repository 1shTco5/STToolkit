#pragma once

#include "UObject/Object.h"
#include "STInteractionContext.h"
#include "STInteractionCandidateFilter.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class STINTERACTIONSYSTEM_API USTInteractionCandidateFilter : public UObject {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    bool PassesFilter(const FSTInteractionContext& Context) const;
    virtual bool PassesFilter_Implementation(const FSTInteractionContext& Context) const;
};
