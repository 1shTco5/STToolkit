#include "CustomAssetPropertyTabFactory.h"
#include "CustomAssetEditorApp.h"
#include "CustomAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

CustomAssetPropertyTabFactory::CustomAssetPropertyTabFactory(TSharedPtr<class CustomAssetEditorApp> App) :
    FWorkflowTabFactory(FName("CustomAssetPropertyTab"), App) {
    m_App = App;
}

TSharedRef<SWidget> CustomAssetPropertyTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    TSharedPtr<CustomAssetEditorApp> App = m_App.Pin();
    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
        TEXT("PropertyEditor"));

    FDetailsViewArgs DetailsViewArgs;
    {
        DetailsViewArgs.bAllowSearch = false;
        DetailsViewArgs.bHideSelectionTip = true;
        DetailsViewArgs.bLockable = false;
        DetailsViewArgs.bSearchInitialKeyFocus = true;
        DetailsViewArgs.bUpdatesFromSelection = false;
        DetailsViewArgs.NotifyHook = nullptr;
        DetailsViewArgs.bShowOptions = true;
        DetailsViewArgs.bShowModifiedPropertiesOption = false;
        DetailsViewArgs.bShowScrollBar = false;
    }

    TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    DetailsView->SetObject(App->GetWorkingAsset());

    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        .HAlign(HAlign_Fill)
        [
            DetailsView.ToSharedRef()
        ];
}

FText CustomAssetPropertyTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const {
    return FText::FromString(TEXT("A property view for current asset"));
}