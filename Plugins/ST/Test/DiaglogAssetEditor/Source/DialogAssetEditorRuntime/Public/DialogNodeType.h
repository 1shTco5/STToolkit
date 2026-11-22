#pragma once

#include "CoreMinimal.h"
#include "DialogNodeType.generated.h"

UENUM()
enum class EDialogNodeType {
    Unknown,
    StartNode,
    DialogNode,
    EndNode,
};