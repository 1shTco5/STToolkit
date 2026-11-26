// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSTInteractionSystemModule : public IModuleInterface {
public:
    //~Begin IModuleInterface Interface
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    //~End IModuleInterface Interface
};
