#include "DialogAssetPrimaryTabFactory.h"
#include "DialogAssetEditorApp.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"

DialogAssetPrimaryTabFactory::DialogAssetPrimaryTabFactory(TSharedPtr<class DialogAssetEditorApp> App) :
    FWorkflowTabFactory(FName("DialogAssetPrimaryTab"), App) {
    m_App = App;
}

TSharedRef<SWidget> DialogAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    TSharedPtr<DialogAssetEditorApp> App = m_App.Pin();

    SGraphEditor::FGraphEditorEvents GraphEvents;
    GraphEvents.OnSelectionChanged.BindRaw(App.Get(), &DialogAssetEditorApp::OnGraphSelectionChanged);

    TSharedPtr<SGraphEditor> GraphEditor =
        SNew(SGraphEditor).IsEditable(true).GraphEvents(GraphEvents).GraphToEdit(App->GetWorkingGraph());
    App->SetWorkingGraphUI(GraphEditor);

    return SNew(SVerticalBox)
        + SVerticalBox::Slot().FillHeight(1.0f).HAlign(HAlign_Fill)
        [
            GraphEditor.ToSharedRef()
        ];
}

FText DialogAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const {
    return FText::FromString(TEXT("A primary view for doing primary things"));
}