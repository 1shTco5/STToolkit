#include "Asset/STDialogueAssetFactory.h"

#include "STDialogueAsset.h"
#include "Graph/STDialogueSchema.h"

USTDialogueAssetFactory::USTDialogueAssetFactory() {
    SupportedClass = USTDialogueAsset::StaticClass();
    bCreateNew = true;
}

TSubclassOf<UEdGraphSchema> USTDialogueAssetFactory::GetGraphSchemaClass() const {
    return USTDialogueSchema::StaticClass();
}