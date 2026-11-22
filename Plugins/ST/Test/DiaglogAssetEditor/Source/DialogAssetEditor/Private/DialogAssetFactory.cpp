#include "DialogAssetFactory.h"
#include "DialogAsset.h"

UDialogAssetFactory::UDialogAssetFactory(const FObjectInitializer& ObjectInitializer) {
    SupportedClass = UDialogAsset::StaticClass();
}

UObject* UDialogAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                               UObject* Context, FFeedbackContext* Warn) {
    UDialogAsset* Asset = NewObject<UDialogAsset>(InParent, InName, Flags);
    return Asset;
}

bool UDialogAssetFactory::CanCreateNew() const {
    return true;
}