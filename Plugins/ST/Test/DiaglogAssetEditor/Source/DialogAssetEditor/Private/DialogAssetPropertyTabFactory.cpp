#include "DialogAssetPropertyTabFactory.h"
#include "DialogAssetEditorApp.h"
#include "DialogAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

DialogAssetPropertyTabFactory::DialogAssetPropertyTabFactory(TSharedPtr<class DialogAssetEditorApp> App) :
    FWorkflowTabFactory(FName("DialogAssetPropertyTab"), App) {
    m_App = App;
}

TSharedRef<SWidget> DialogAssetPropertyTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    TSharedPtr<DialogAssetEditorApp> App = m_App.Pin();
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

    TSharedPtr<IDetailsView> SelectedNodeDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
    SelectedNodeDetailsView->SetObject(nullptr);
    App->SetSelectedNodeDetailsView(SelectedNodeDetailsView);

    return SNew(SVerticalBox)
        + SVerticalBox::Slot().FillHeight(1.f).HAlign(HAlign_Fill)
        [
            DetailsView.ToSharedRef()
        ]
        + SVerticalBox::Slot().FillHeight(1.f).HAlign(HAlign_Fill)
        [
            SelectedNodeDetailsView.ToSharedRef()
        ];
}

FText DialogAssetPropertyTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const {
    return FText::FromString(TEXT("A property view for current asset"));
}