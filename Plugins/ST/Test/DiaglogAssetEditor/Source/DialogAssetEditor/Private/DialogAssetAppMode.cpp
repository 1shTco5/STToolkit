#include "DialogAssetAppMode.h"
#include "DialogAssetEditorApp.h"
#include "DialogAssetPrimaryTabFactory.h"
#include "DialogAssetPropertyTabFactory.h"

DialogAssetAppMode::DialogAssetAppMode(TSharedPtr<DialogAssetEditorApp> App) : FApplicationMode(
    TEXT("DialogAssetAppMode")) {
    m_App = App;
    m_Tabs.RegisterFactory(MakeShareable(new DialogAssetPrimaryTabFactory(App)));
    m_Tabs.RegisterFactory(MakeShareable(new DialogAssetPropertyTabFactory(App)));

    TabLayout = FTabManager::NewLayout("DialogAssetAppMode_Layout_v2")->AddArea(
        FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)->Split(
            FTabManager::NewStack()->SetSizeCoefficient(0.75)->AddTab(
                FName(TEXT("DialogAssetPrimaryTab")), ETabState::OpenedTab))->Split(
            FTabManager::NewStack()->SetSizeCoefficient(0.25)->AddTab(
                FName(TEXT("DialogAssetPropertyTab")), ETabState::OpenedTab))
        );
}

void DialogAssetAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) {
    TSharedPtr<DialogAssetEditorApp> App = m_App.Pin();
    App->PushTabFactories(m_Tabs);
}

void DialogAssetAppMode::PreDeactivateMode() {
    FApplicationMode::PreDeactivateMode();
}

void DialogAssetAppMode::PostActivateMode() {
    FApplicationMode::PostActivateMode();
}