#include "Asset/STGraphAssetFactory.h"

#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphSchema.h"
#include "STGraphAsset.h"

#pragma region UFactory Interface
UObject* USTGraphAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                                UObject* Context, FFeedbackContext* Warn) {
    const UClass* AssetClass = GetSupportedClass();
    if (!AssetClass || !InClass || !InClass->IsChildOf(AssetClass) ||
        !InClass->IsChildOf(USTGraphAsset::StaticClass())) {
        return nullptr;
    }

    const TSubclassOf<UEdGraphSchema> SchemaClass = GetGraphSchemaClass();
    if (!SchemaClass) {
        return nullptr;
    }

    USTGraphAsset* Asset = NewObject<USTGraphAsset>(InParent, InClass, InName, Flags);
    UEdGraph* Graph = NewObject<UEdGraph>(Asset, TEXT("EditorGraph"));
    Graph->Schema = SchemaClass;
    Asset->EditorGraph = Graph;
    Graph->GetSchema()->CreateDefaultNodesForGraph(*Graph);
    return Asset;
}
#pragma endregion
