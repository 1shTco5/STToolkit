#include "CustomAssetAppMode.h"
#include "CustomAssetEditorApp.h"
#include "CustomAssetTabFactory.h"

CustomAssetAppMode::CustomAssetAppMode(TSharedPtr<CustomAssetEditorApp> App) : FApplicationMode(
    TEXT("CustomAssetAppMode")) {
    m_App = App;
    m_Tabs.RegisterFactory(MakeShareable(new CustomAssetTabFactory(App)));

    TabLayout = FTabManager::NewLayout("CustomAssetAppMode_Layout_v1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
                                         ->Split(
                                             FTabManager::NewStack()->AddTab(
                                                 FName(TEXT("CustomAssetTab")),
                                                 ETabState::OpenedTab
                                                 )
                                             )
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