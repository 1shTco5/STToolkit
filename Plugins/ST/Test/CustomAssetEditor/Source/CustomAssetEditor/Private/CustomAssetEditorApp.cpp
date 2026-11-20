#include "CustomAssetEditorApp.h"

#include "CustomAssetAppMode.h"
#include "CustomAsset.h"

void CustomAssetEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) {
    FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void CustomAssetEditorApp::InitEditor(const EToolkitMode::Type Mode,
                                      const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject) {
    TArray<UObject*> ObjectsToEdit;
    ObjectsToEdit.Add(InObject);

    m_WorkingAsset = Cast<UCustomAsset>(InObject);

    InitAssetEditor(
        Mode,
        InitToolkitHost,
        TEXT("CustomAssetEditor"),
        FTabManager::FLayout::NullLayout,
        true,
        true,
        ObjectsToEdit);

    AddApplicationMode(TEXT("CustomAssetAppMode"), MakeShareable(new CustomAssetAppMode(SharedThis(this))));

    SetCurrentMode(TEXT("CustomAssetAppMode"));
}