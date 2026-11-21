#include "CustomAssetAppMode.h"
#include "CustomAssetEditorApp.h"
#include "CustomAssetPrimaryTabFactory.h"
#include "CustomAssetPropertyTabFactory.h"

CustomAssetAppMode::CustomAssetAppMode(TSharedPtr<CustomAssetEditorApp> App) : FApplicationMode(
    TEXT("CustomAssetAppMode")) {
    m_App = App;
    m_Tabs.RegisterFactory(MakeShareable(new CustomAssetPrimaryTabFactory(App)));
    m_Tabs.RegisterFactory(MakeShareable(new CustomAssetPropertyTabFactory(App)));

    TabLayout = FTabManager::NewLayout("CustomAssetAppMode_Layout_v2")->AddArea(
        FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)->Split(
            FTabManager::NewStack()->SetSizeCoefficient(0.75)->AddTab(
                FName(TEXT("CustomAssetPrimaryTab")), ETabState::OpenedTab))->Split(
            FTabManager::NewStack()->SetSizeCoefficient(0.25)->AddTab(
                FName(TEXT("CustomAssetPropertyTab")), ETabState::OpenedTab))
        );
}

void CustomAssetAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) {
    TSharedPtr<CustomAssetEditorApp> App = m_App.Pin();
    App->PushTabFactories(m_Tabs);
}

void CustomAssetAppMode::PreDeactivateMode() {
    FApplicationMode::PreDeactivateMode();
}

void CustomAssetAppMode::PostActivateMode() {
    FApplicationMode::PostActivateMode();
}