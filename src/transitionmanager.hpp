#pragma once

#include "raylib.h"

enum struct FADE_STATES {
    NONE,
    FADE_IN,
    FADE_OUT,
    HOLD,
};

struct TransitionManager {
    void OnUpdate(float deltaTime) {
        switch (m_fadeState) {
            case FADE_STATES::NONE: return;
            case FADE_STATES::HOLD: return;

            case FADE_STATES::FADE_IN: {
                m_currentAlpha -= FADE_SPEED * deltaTime;

                if (m_currentAlpha < 0.0f) {
                    m_currentAlpha = 0.0f;
                    m_fadeState = FADE_STATES::NONE;
                }
            } break;

            case FADE_STATES::FADE_OUT: {
                m_currentAlpha += FADE_SPEED * deltaTime;

                if (m_currentAlpha > 1.0f) {
                    m_currentAlpha = 1.0f;
                    m_fadeState = FADE_STATES::HOLD;
                }
            } break;
        }
    }

    void OnRender() {
        if (m_fadeState == FADE_STATES::NONE) {
            return;
        }

        DrawRectangle(0, 0, 640, 480, ColorAlpha(BLACK, m_currentAlpha));
    }

    void FadeOut() {
        m_fadeState = FADE_STATES::FADE_OUT;
    }

    void FadeIn() {
        m_currentAlpha = 1.0f;
        m_fadeState = FADE_STATES::FADE_IN;
    }

    bool IsDoneFade() {
        return m_fadeState == FADE_STATES::NONE || m_fadeState == FADE_STATES::HOLD;
    }

private:
    FADE_STATES m_fadeState = FADE_STATES::FADE_IN;
    float m_currentAlpha;
    static constexpr const float FADE_SPEED = 0.5f * 5.0f;
};
