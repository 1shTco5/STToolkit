#include "DialogAsset.h"

#include "UObject/ObjectSaveContext.h"

void UDialogAsset::PreSave(FObjectPreSaveContext SaveContext) {
    if (m_OnPreSaveListener) {
        m_OnPreSaveListener();
    }
}