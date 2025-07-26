#include "tamapetai.hpp"

#include "game.hpp"

void TamaPetAI::OnInitialize() {
    m_lastFrameCounter = 0;
    m_currentTimer = 0.0f;
}

void TamaPetAI::OnUpdate(float deltaTime) {
    Pet& petData = m_game->m_gameData.GetCurrentPet();

    if (petData.state == PET_STATES::DED) {
        m_animationStep = 0;
        return;
    }

    petData.attributes[PET_ATTRIBUTES::GROWTH] += deltaTime * m_game->m_gameData.gameSpeed;
    m_currentTimer += deltaTime * m_game->m_gameData.gameSpeed;

    if (static_cast<int>(m_currentTimer / 0.5f) > m_lastFrameCounter) {
        m_lastFrameCounter = static_cast<int>(m_currentTimer / 0.5f);
        ProcessAI();
    }

    ProcessMovement(deltaTime);
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
        : petData.stage == PET_STAGES::NEWBORN
            ? m_game->m_resourceManager.GetTexture("textures/tadpole1.png")
            : petData.stage == PET_STAGES::TODDLER
                ? m_game->m_resourceManager.GetTexture("textures/tadpole2.png")
                    : petData.stage == PET_STAGES::ADOLESCENT
                    ? m_game->m_resourceManager.GetTexture("textures/tadpole3.png")
                        : m_game->m_resourceManager.GetTexture("textures/tadpole4.png");
                // : petData.stage == PET_STAGES::NEWBORN
                //     ? m_game->m_resourceManager.GetTexture("textures/tadpole1.png")

    Texture& faceTexture = m_game->m_resourceManager.GetTexture("textures/faces.png");

    if (petData.stage == PET_STAGES::EGG) {
        DrawTexturePro(
            petTexture,
            { (m_animationStep % MAX_ANIMATION_STEPS) * m_game->m_gameData.GetCurrentPetWidth(), GetOffsetFromState(), m_petDirection * m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { 0.0f, 0.0f },
            0.0f,
            WHITE
        );
        return;
    }

    if (petData.state == PET_STATES::DED) {
        DrawTexturePro(
           petTexture,
           { (m_animationStep % MAX_ANIMATION_STEPS) * m_game->m_gameData.GetCurrentPetWidth(), GetOffsetFromState(), m_petDirection * m_game->m_gameData.GetCurrentPetWidth(), -m_game->m_gameData.GetCurrentPetHeight() },
           { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
           { 0.0f, 0.0f },
           0.0f,
           m_game->m_gameData.PetTintList[petData.petTint]
        );
    } else {
        DrawTexturePro(
            petTexture,
            { (m_animationStep % MAX_ANIMATION_STEPS) * m_game->m_gameData.GetCurrentPetWidth(), GetOffsetFromState(), m_petDirection * m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { m_petPosition.x, m_petPosition.y, m_game->m_gameData.GetCurrentPetWidth(), m_game->m_gameData.GetCurrentPetHeight() },
            { 0.0f, 0.0f },
            0.0f,
            m_game->m_gameData.PetTintList[petData.petTint]
        );
    }

    if (petData.stage == PET_STAGES::EGG) {
        return;
    }

    if (m_petDirection == -1) {
        if (petData.state == PET_STATES::DED) {
            DrawTexturePro(
                faceTexture,
                {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, -m_game->m_gameData.GetCurrentFace().height},
                { m_petPosition.x + m_game->m_gameData.GetCurrentPetWidth() - 42, m_petPosition.y, 64, 64 },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        } else {
            DrawTexturePro(
                faceTexture,
                {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
                { m_petPosition.x + m_game->m_gameData.GetCurrentPetWidth() - 42, m_petPosition.y, 64, 64 },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }
    } else {
        if (petData.state == PET_STATES::DED) {
            DrawTexturePro(
                faceTexture,
                {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, -m_game->m_gameData.GetCurrentFace().height},
                { m_petPosition.x, m_petPosition.y, 64, 64},
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        } else {
            DrawTexturePro(
                faceTexture,
                {m_game->m_gameData.GetCurrentFace().x, m_game->m_gameData.GetCurrentFace().y + (m_animationStep == 6 ? 64 : 0), m_game->m_gameData.GetCurrentFace().width, m_game->m_gameData.GetCurrentFace().height},
                { m_petPosition.x, m_petPosition.y, 64, 64},
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        }
    }
}

float TamaPetAI::GetOffsetFromState() {
    return 0.0f;
}

void TamaPetAI::ProcessAI() {
    m_animationStep += 1;

    if (m_animationStep > (MAX_ANIMATION_STEPS * 2)) {
        m_animationStep = 0;
    }

    if (m_lastFrameCounter % 10 == 0) {
        AIMovement();
    }

    ProcessAttributes();
}

void TamaPetAI::ProcessMovement(float deltaTime) {
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

void TamaPetAI::ProcessAttributes() {
    Pet& petData = m_game->m_gameData.GetCurrentPet();
    if (petData.stage == PET_STAGES::EGG) {
        if (petData.attributes[PET_ATTRIBUTES::GROWTH] >= 60.0f) {
            petData.state = PET_STATES::EVOLVE;
        }

        return;
    }

    constexpr const float multiplySpeed = 0.0001f;
    petData.attributes[PET_ATTRIBUTES::HUNGER] += GetRandomValue(10, 20) * multiplySpeed;
    petData.attributes[PET_ATTRIBUTES::HAPPINESS] -= GetRandomValue(10, 26) * multiplySpeed;
    petData.attributes[PET_ATTRIBUTES::BOREDOM] += GetRandomValue(15, 40) * multiplySpeed;
    petData.attributes[PET_ATTRIBUTES::HYGIENE] -= GetRandomValue(15, 50) * multiplySpeed;
    petData.attributes[PET_ATTRIBUTES::TANKHYGIENE] -= GetRandomValue(10, 26) * multiplySpeed;
    petData.attributes[PET_ATTRIBUTES::ILLNESS] -= GetRandomValue(5, 15) * multiplySpeed;

    ClampRange(petData.attributes[PET_ATTRIBUTES::HUNGER]);
    ClampRange(petData.attributes[PET_ATTRIBUTES::HAPPINESS]);
    ClampRange(petData.attributes[PET_ATTRIBUTES::BOREDOM]);
    ClampRange(petData.attributes[PET_ATTRIBUTES::HYGIENE]);
    ClampRange(petData.attributes[PET_ATTRIBUTES::TANKHYGIENE]);
    ClampRange(petData.attributes[PET_ATTRIBUTES::ILLNESS]);

    float healthChange = 0.0f;
    // petData.attributes[PET_ATTRIBUTES::HP] = 1.0f;
    if (petData.attributes[PET_ATTRIBUTES::HUNGER] >= 0.25f) {
        healthChange -= 0.05f * (petData.attributes[PET_ATTRIBUTES::HUNGER] * 0.25f);
    } else {
        healthChange += 0.05f;
    }

    if (petData.attributes[PET_ATTRIBUTES::HAPPINESS] <= 0.75f) {
        healthChange -= 0.05f * ((1 - petData.attributes[PET_ATTRIBUTES::HAPPINESS]) * 0.25f);
    } else {
        healthChange += 0.05f;
    }

    if (petData.attributes[PET_ATTRIBUTES::HYGIENE] <= 0.75f) {
        healthChange -= 0.05f * ((1 - petData.attributes[PET_ATTRIBUTES::HYGIENE]) * 0.25f);
    } else {
        healthChange += 0.05f;
    }

    if (petData.attributes[PET_ATTRIBUTES::TANKHYGIENE] <= 0.75f) {
        healthChange -= 0.5f * 0.05f * ((1 - petData.attributes[PET_ATTRIBUTES::TANKHYGIENE]) * 0.25f);
    } else {
        healthChange += 0.5f * 0.05f;
    }

    if (petData.attributes[PET_ATTRIBUTES::ILLNESS] <= 0.75f) {
        healthChange -= 0.05f * ((1 - petData.attributes[PET_ATTRIBUTES::ILLNESS]) * 0.25f);
    } else {
        healthChange += 0.05f;
    }

    petData.attributes[PET_ATTRIBUTES::HP] += 0.1f * healthChange;

    if (petData.attributes[PET_ATTRIBUTES::HP] <= 0.0f) {
        petData.state = PET_STATES::DED;

        return;
    }

    if (petData.state == PET_STATES::HEALTHY) {
        if (petData.attributes[PET_ATTRIBUTES::GROWTH] >= 600.0f) {
            petData.state = PET_STATES::EVOLVE;
            return;
        }

        if (petData.attributes[PET_ATTRIBUTES::HUNGER] > 0.85f) {
            petData.state = PET_STATES::HUNGRY;
            return;
        }

        if (petData.attributes[PET_ATTRIBUTES::HUNGER] > 0.45f) {
            petData.state = PET_STATES::HUNGRY;
            return;
        }

        if (petData.attributes[PET_ATTRIBUTES::BOREDOM] > 0.35f) {
            petData.state = PET_STATES::ANGRY;
            return;
        }

        if (petData.attributes[PET_ATTRIBUTES::HAPPINESS] < 0.65f) {
            petData.state = PET_STATES::SAD;
            return;
        }

        if (petData.attributes[PET_ATTRIBUTES::HAPPINESS] < 0.65f) {
            petData.state = PET_STATES::SICK;
            return;
        }
    }
}

void TamaPetAI::AIMovement() {
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

void TamaPetAI::ClampRange(float &value) {
    if (value < 0.0f) {
        value = 0.0f;
    }

    if (value > 1.0f) {
        value = 1.0f;
    }
}

