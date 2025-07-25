#pragma once

#include <string>

#include "raylib.h"

struct DeviceButton {
    rlRectangle position;
    std::string displayText;
};
