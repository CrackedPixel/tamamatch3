#pragma once

struct InputController {
    bool IsButtonDown = false;
    bool IsButtonUp = false;
    bool IsButtonSelect = false;
    bool IsButtonBack = false;

    void EnableButtonDown() {
        IsButtonDown = true;
    }

    void EnableButtonUp() {
        IsButtonUp = true;
    }

    void EnableButtonSelect() {
        IsButtonSelect = true;
    }

    void EnableButtonBack() {
        IsButtonBack = true;
    }

    void OnHandleInput() {
        IsButtonDown = false;
        IsButtonUp = false;
        IsButtonSelect = false;
        IsButtonBack = false;
    }
};
