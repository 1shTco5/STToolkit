#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "DialogRuntimeGraph.h"
#include "DialogAsset.generated.h"

UCLASS(BlueprintType)
class DIALOGASSETEDITORRUNTIME_API UDialogAsset : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString DialogName = TEXT("Enter dialog name here");

    UPROPERTY()
    UDialogRuntimeGraph* Graph = nullptr;

private:
    std::function<void()> m_OnPreSaveListener = nullptr;

public:
    virtual void PreSave(FObjectPreSaveContext SaveContext) override;
    
public:
    void SetPreSaveListener(std::function<void()> OnPreSaveListener) { m_OnPreSaveListener = OnPreSaveListener; }
};