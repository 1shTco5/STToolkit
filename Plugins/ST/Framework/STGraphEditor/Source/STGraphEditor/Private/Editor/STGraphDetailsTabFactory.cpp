#include "STGraphDetailsTabFactory.h"

#include "DetailsViewArgs.h"
#include "IDetailsView.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "STGraphEditorApp.h"
#include "Widgets/SNullWidget.h"

const FName FSTGraphDetailsTabFactory::TabId(TEXT("STGraphEditor.DetailsTab"));

FSTGraphDetailsTabFactory::FSTGraphDetailsTabFactory(const TSharedRef<FSTGraphEditorApp>& InApp)
    : FWorkflowTabFactory(TabId, InApp) {
    TabLabel = FText::FromString(TEXT("Details"));
    bIsSingleton = true;
}

#pragma region FWorkflowTabFactory Interface
TSharedRef<SWidget> FSTGraphDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    const TSharedPtr<FSTGraphEditorApp> App = StaticCastSharedPtr<FSTGraphEditorApp>(HostingApp.Pin());
    if (!App) {
        return SNullWidget::NullWidget;
    }

    FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.bHideSelectionTip = true;
    const TSharedRef<IDetailsView> DetailsView = PropertyEditor.CreateDetailView(DetailsViewArgs);
    const bool bEditable = App->GetOpenMethod() == EAssetOpenMethod::Edit;
    DetailsView->SetIsPropertyEditingEnabledDelegate(
        FIsPropertyEditingEnabled::CreateLambda([bEditable] { return bEditable; }));
    App->SetDetailsView(DetailsView);
    return DetailsView;
}
#pragma endregion
