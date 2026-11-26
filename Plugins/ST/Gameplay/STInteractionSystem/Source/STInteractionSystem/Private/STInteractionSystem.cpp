// Copyright Epic Games, Inc. All Rights Reserved.

#include "STInteractionSystem.h"

#define LOCTEXT_NAMESPACE "FSTInteractionSystemModule"

#pragma region IModuleInterface
void FSTInteractionSystemModule::StartupModule() {
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin
    // file per-module
}

void FSTInteractionSystemModule::ShutdownModule() {
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
}

#pragma endregion

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSTInteractionSystemModule, STInteractionSystem)
