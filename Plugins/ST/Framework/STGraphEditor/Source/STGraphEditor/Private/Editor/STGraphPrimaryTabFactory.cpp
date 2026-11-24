#include "STGraphPrimaryTabFactory.h"

#include "GraphEditor.h"
#include "STGraphEditorApp.h"
#include "Widgets/SNullWidget.h"

const FName FSTGraphPrimaryTabFactory::TabId(TEXT("STGraphEditor.PrimaryTab"));

FSTGraphPrimaryTabFactory::FSTGraphPrimaryTabFactory(const TSharedRef<FSTGraphEditorApp>& InApp)
    : FWorkflowTabFactory(TabId, InApp) {
    TabLabel = FText::FromString(TEXT("STGraph"));
    bIsSingleton = true;
}

#pragma region FWorkflowTabFactory Interface
TSharedRef<SWidget> FSTGraphPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const {
    const TSharedPtr<FSTGraphEditorApp> App = StaticCastSharedPtr<FSTGraphEditorApp>(HostingApp.Pin());
    UEdGraph* Graph = App ? App->GetEditorGraph() : nullptr;
    if (!Graph) {
        return SNullWidget::NullWidget;
    }

    const bool bEditable = App->GetOpenMethod() == EAssetOpenMethod::Edit;
    SGraphEditor::FGraphEditorEvents GraphEvents;
    GraphEvents.OnSelectionChanged.BindSP(App.ToSharedRef(), &FSTGraphEditorApp::OnGraphSelectionChanged);
    const TSharedRef<SGraphEditor> GraphEditor = SNew(SGraphEditor)
                                                     .GraphToEdit(Graph)
                                                     .GraphEvents(GraphEvents)
                                                     .IsEditable(bEditable)
                                                     .DisplayAsReadOnly(!bEditable);
    App->OnGraphSelectionChanged(GraphEditor->GetSelectedNodes());
    return GraphEditor;
}
#pragma endregion
