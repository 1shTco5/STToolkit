#include "STGraphEditorAppMode.h"

#include "STGraphDetailsTabFactory.h"
#include "STGraphPrimaryTabFactory.h"

FSTGraphEditorAppMode::FSTGraphEditorAppMode(const TSharedRef<FSTGraphEditorApp>& InApp)
    : FApplicationMode(TEXT("STGraphEditor.GraphMode")) {
    ApplicationModeTabFactories.RegisterFactory(MakeShared<FSTGraphPrimaryTabFactory>(InApp));
    ApplicationModeTabFactories.RegisterFactory(MakeShared<FSTGraphDetailsTabFactory>(InApp));

    TabLayout = FTabManager::NewLayout(TEXT("STGraphEditor_Layout_v3"))
                    ->AddArea(FTabManager::NewPrimaryArea()
                                  ->SetOrientation(Orient_Horizontal)
                                  ->Split(FTabManager::NewStack()->SetSizeCoefficient(0.75f)->AddTab(
                                      FSTGraphPrimaryTabFactory::TabId, ETabState::OpenedTab))
                                  ->Split(FTabManager::NewStack()->SetSizeCoefficient(0.25f)->AddTab(
                                      FSTGraphDetailsTabFactory::TabId, ETabState::OpenedTab)));
}
