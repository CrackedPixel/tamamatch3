#include "tamapetai.hpp"

#include "game.hpp"

void TamaPetAI::OnInitialize() {
    m_lastFrameCounter = 0;
    m_currentTimer = 0.0f;
}

void TamaPetAI::OnUpdate(float deltaTime) {
    m_currentTimer += deltaTime * m_game->m_gameData.gameSpeed;

    if (static_cast<int>(m_currentTimer / 0.5f) > m_lastFrameCounter) {
        m_lastFrameCounter = static_cast<int>(m_currentTimer / 0.5f);
        ProcessAI();
    }

    if (m_petPosition.x < m_petTarget.x) {
        m_petDirection = -1;

        m_petPosition.x += deltaTime * m_petSpeed;
        if (m_petPosition.x > m_petTarget.x) {
            m_petPosition.x = m_petTarget.x;
        }
    }

    if (m_petPosition.x > m_petTarget.x) {
        m_petDirection = 1;

        m_petPosition.x -= deltaTime * m_petSpeed;
        if (m_petPosition.x < m_petTarget.x) {
            m_petPosition.x = m_petTarget.x;
        }
    }


    if (m_petPosition.y < m_petTarget.y) {
        m_petPosition.y += deltaTime * m_petSpeed;
        if (m_petPosition.y > m_petTarget.y) {
            m_petPosition.y = m_petTarget.y;
        }
    }

    if (m_petPosition.y > m_petTarget.y) {
        m_petPosition.y -= deltaTime * m_petSpeed;
        if (m_petPosition.y < m_petTarget.y) {
            m_petPosition.y = m_petTarget.y;
        }
    }
}

bool TamaPetAI::OnHandleInput(Vector2 mousePosition) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return false;
    }

    rlRectangle PetBoundsGlobal = m_petBounds;
    PetBoundsGlobal.x += 184;
    PetBoundsGlobal.y += 174;
    PetBoundsGlobal.width += 184;
    PetBoundsGlobal.height += 174;

    if (CheckCollisionPointRec(mousePosition, PetBoundsGlobal)) {
        Vector2 newTarget = mousePosition;
        newTarget.x -= 184;
        newTarget.y -= 174;
        SetNewPetTarget(newTarget);

        return true;
    }

    return false;
}

void TamaPetAI::OnRender() {
    Pet& petData = m_game->m_gameData.GetCurrentPet();
    Texture& petTexture = petData.stage == PET_STAGES::EGG
                ? m_game->m_resourceManager.GetTexture("textures/tadpole0.png")
                : m_game->m_resourceManager.GetTexture("textures/tadpole1.png")
    ;
                // : petData.stage == PET_STAGES::NEWBORN
                //     ? m_game->m_resourceManager.GetTexture("textures/tadpole1.png")

    Texture& faceTexture = m_game->m_resourceManager.GetTexture("textures/faces.png");

    DrawTexturePro(
       petTexture,
       { m_animationStep * (petTexture.width * 0.25f), GetOffsetFromState(), m_petDirection * m_game->m_gameData.GetCurrentPetWidth(), 64 },
       { m_petPosition.x, m_petPosition.y, petTexture.width * 0.25f, 64.0f},
       { 0.0f, 0.0f },
       0.0f,
       m_game->m_gameData.PetTintList[petData.petTint] // TBD
    );

    if (m_petDirection == -1) {
        DrawTexturePro(
            faceTexture,
            m_game->m_gameData.GetCurrentFace(),
            { m_petPosition.x + m_game->m_gameData.GetCurrentPetWidth() - 42, m_petPosition.y, 64, 64},
            { 0.0f, 0.0f },
            0.0f,
            WHITE
            );
    } else {
        DrawTexturePro(
            faceTexture,
            m_game->m_gameData.GetCurrentFace(),
            { m_petPosition.x, m_petPosition.y, 64, 64},
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
    }
}

float TamaPetAI::GetOffsetFromState() {
    return 0.0f;
}

// void OnInitialize() {
//     attributes[PET_ATTRIBUTES::HUNGER] = 0.0f;
//     attributes[PET_ATTRIBUTES::HAPPINESS] = 1.0f;
//     attributes[PET_ATTRIBUTES::BOREDOM] = 0.0f;
//     attributes[PET_ATTRIBUTES::HYGIENE] = 1.0f;
//     attributes[PET_ATTRIBUTES::TANKHYGIENE] = 1.0f;
//     attributes[PET_ATTRIBUTES::HEALTH] = 1.0f;
//     attributes[PET_ATTRIBUTES::GROWTH] = 0.0f;
// }

void TamaPetAI::ProcessAI() {
    m_animationStep += 1;

    if (m_animationStep > MAX_ANIMATION_STEPS) {
        m_animationStep = 0;
    }

    if (m_lastFrameCounter % 10 == 0) {
        ProcessMovement();
    }
}

void TamaPetAI::ProcessMovement() {
    // if (GetRandomValue(0, 1) == 1) {
    //     m_petTarget.x = m_petPosition.x + GetRandomValue(64, 256);
    // } else {
    //     m_petTarget.x = m_petPosition.x - GetRandomValue(64, 256);
    // }

    if (GetRandomValue(0, 10) == 1) {
        m_petTarget.x = m_petPosition.x;
        m_petTarget.y = m_petPosition.y;

        return;
    }

    SetNewPetTarget(
        GetRandomValue(m_petBounds.x, m_petBounds.width),
        GetRandomValue(m_petBounds.y, m_petBounds.height)
    );
}

void TamaPetAI::SetNewPetTarget(int newX, int newY) {
    m_petTarget.x = newX;
    m_petTarget.y = newY;

    if (m_petTarget.x < m_petBounds.x) {
        m_petTarget.x = m_petBounds.x;
    }

    if (m_petTarget.y < m_petBounds.y) {
        m_petTarget.y = m_petBounds.y;
    }

    if (m_petTarget.x > m_petBounds.width - m_game->m_gameData.GetCurrentPetWidth()) {
        m_petTarget.x = m_petBounds.width - m_game->m_gameData.GetCurrentPetWidth();
    }

    if (m_petTarget.y > m_petBounds.height) {
        m_petTarget.y = m_petBounds.height;
    }
}

void TamaPetAI::SetNewPetTarget(Vector2 destination) {
    SetNewPetTarget(destination.x, destination.y);
}

