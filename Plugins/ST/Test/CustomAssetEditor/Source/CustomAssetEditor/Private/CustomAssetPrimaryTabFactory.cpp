#include "CustomAssetPrimaryTabFactory.h"
#include "CustomAssetEditorApp.h"
#include "CustomAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

CustomAssetPrimaryTabFactory::CustomAssetPrimaryTabFactory(TSharedPtr<class CustomAssetEditorApp> App) :
    FWorkflowTabFactory(FName("CustomAssetPrimaryTab"), App) {
    m_App = App;
}

TSharedRef<SWidget> CustomAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    TSharedPtr<CustomAssetEditorApp> App = m_App.Pin();

    return SNew(SVerticalBox)
        + SVerticalBox::Slot().FillHeight(1.0f).HAlign(HAlign_Fill)
        [
            SNew(SGraphEditor).IsEditable(true).GraphToEdit(App->GetWorkingGraph())
        ];
}

FText CustomAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const {
    return FText::FromString(TEXT("A primary view for doing primary things"));
}