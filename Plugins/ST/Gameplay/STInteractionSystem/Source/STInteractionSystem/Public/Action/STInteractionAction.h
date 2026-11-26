#pragma once

#include "UObject/Object.h"
#include "STInteractionContext.h"
#include "STInteractionAction.generated.h"

UENUM(BlueprintType)
enum class ESTInteractionExecutionPolicy : uint8 { ServerOnly, OwningClientOnly, ServerAndOwningClient };

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class STINTERACTIONSYSTEM_API USTInteractionAction : public UObject {
    GENERATED_BODY()
    friend class USTInteractorComponent;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
    ESTInteractionExecutionPolicy ExecutionPolicy = ESTInteractionExecutionPolicy::ServerAndOwningClient;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
    bool bWaitForCompletion = false;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Complete();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Fail();

protected:
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void ReceiveInitialize(const FSTInteractionContext& Context);
    virtual void ReceiveInitialize_Implementation(const FSTInteractionContext& Context);

    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void ReceiveStart(const FSTInteractionContext& Context);
    virtual void ReceiveStart_Implementation(const FSTInteractionContext& Context);

    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void ReceiveStop(const FSTInteractionContext& Context);
    virtual void ReceiveStop_Implementation(const FSTInteractionContext& Context);

    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void ReceiveShutdown(const FSTInteractionContext& Context);
    virtual void ReceiveShutdown_Implementation(const FSTInteractionContext& Context);
};
