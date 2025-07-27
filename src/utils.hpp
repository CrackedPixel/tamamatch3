#pragma once

#include <string>
// #include <stdio.h>
#include "raylib.h"

namespace Utils {
    inline Color ColorFromString(const char* str) {
        int r = 0, g = 0, b = 0;
        sscanf(str, "%d %d %d", &r, &g, &b);

        if (r < 0) r = 0; else if (r > 255) r = 255;
        if (g < 0) g = 0; else if (g > 255) g = 255;
        if (b < 0) b = 0; else if (b > 255) b = 255;

        return { (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
    }

    inline rlRectangle RectFromString(const char* str) {
        int x = 0, y = 0, w = 0, h = 0;
        sscanf(str, "%d %d %d %d", &x, &y, &w, &h);

        return { static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h) };
    }

    inline Vector2 Vector2FromString(const char* str) {
        int x = 0, y = 0;
        sscanf(str, "%d %d", &x, &y);

        return { static_cast<float>(x), static_cast<float>(y) };
    }

    inline void ClampRange(float& value) {
        if (value < 0.0f) {
            value = 0.0f;
        }

        if (value > 1.0f) {
            value = 1.0f;
        }
    }

    inline std::string ReplaceNewlines(const std::string& input) {
        std::string result;
        size_t prev = 0;
        size_t pos = 0;
        while ((pos = input.find("\\n", prev)) != std::string::npos) {
            result += input.substr(prev, pos - prev);
            result += '\n';
            prev = pos + 2;
        }
        result += input.substr(prev);
        return result;
    }

}
