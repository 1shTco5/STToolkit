#pragma once

#include "UObject/Object.h"
#include "STInteractionContext.h"
#include "STInteractionRequestValidator.generated.h"

USTRUCT(BlueprintType)
struct STINTERACTIONSYSTEM_API FSTInteractionValidationResult {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bAllowed = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText FailureReason;
};

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class STINTERACTIONSYSTEM_API USTInteractionRequestValidator : public UObject {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    FSTInteractionValidationResult Validate(const FSTInteractionContext& Context) const;
    virtual FSTInteractionValidationResult Validate_Implementation(const FSTInteractionContext& Context) const;
};
