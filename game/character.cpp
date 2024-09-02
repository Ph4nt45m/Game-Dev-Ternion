// This include:
// This include:
#include "character.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "inputsystem.h"
#include "../imgui/imgui.h"

// Library includes:
#include <cassert>
#include <cstdio>

// Static members:
float Character::sm_fBoundaryWidth = 0.0f;
float Character::sm_fBoundaryHeight = 0.0f;

Character::Character()
    : m_sMotionKeyStates{ BS_NEUTRAL, BS_NEUTRAL, BS_NEUTRAL, BS_NEUTRAL, BS_NEUTRAL }
    , m_sKeyboardMotions{ MOTION_NONE, MOTION_NONE, MOTION_NONE }
    , m_pSprSpriteHead(0)
    , m_pSprSpriteBody(0)
    , m_pSprSpriteLegLeft(0)
    , m_pSprSpriteLegRight(0)
    , m_pSprSpriteShadow(0)
    , m_bAlive(false)
    , m_bMovingX(false)
    , m_bMovingY(false)
    , m_bJumping(false)
    , m_iFacingDirection(0)
    , m_fTotalHeight(0.0f)
    , m_fHeadBodyOffset(0.0f)
    , m_fLengthFootToBody(0.0f)
    , m_fLegBodyOffset(0.0f)
    , m_fStepTimer(0.0f)
    , m_fStepDuration(0.0f)
{

}

Character::~Character()
{
    delete m_pSprSpriteHead;
    m_pSprSpriteHead = 0;

    delete m_pSprSpriteBody;
    m_pSprSpriteBody = 0;

    delete m_pSprSpriteLegLeft;
    m_pSprSpriteLegLeft = 0;

    delete m_pSprSpriteLegRight;
    m_pSprSpriteLegRight = 0;

    delete m_pSprSpriteShadow;
    m_pSprSpriteShadow = 0;
}

bool Character::Initialise(Renderer& renderer)
{
    m_pSprSpriteHead = renderer.CreateSprite("..\\Sprites\\head.png");
    m_pSprSpriteBody = renderer.CreateSprite("..\\Sprites\\body.png");
    m_pSprSpriteLegLeft = renderer.CreateSprite("..\\Sprites\\leg.png");
    m_pSprSpriteLegRight = renderer.CreateSprite("..\\Sprites\\leg.png");
    m_pSprSpriteShadow = renderer.CreateSprite("..\\Sprites\\floorshadow.png");
    m_pSprSpriteBody->SetAngle(180.0f);
    m_pSprSpriteHead->SetAngle(180.0f);
    m_pSprSpriteLegLeft->SetAngle(180.0f);
    m_pSprSpriteLegRight->SetAngle(180.0f);
    m_pSprSpriteShadow->SetAngle(180.0f);
    m_iFacingDirection = 1;
    m_fTotalHeight = 120.0f;
    m_fHeadBodyOffset = 14.0f;
    m_fLegBodyOffset = 15.0f;
    m_fStepTimer = 0.0f;
    m_fStepDuration = 0.5f;
    m_bAlive = true;

    const int SCREEN_WIDTH = renderer.GetWidth();
    const int SCREEN_HEIGHT = renderer.GetHeight();

    sm_fBoundaryWidth = static_cast<float>(SCREEN_WIDTH);
    sm_fBoundaryHeight = static_cast<float>(SCREEN_HEIGHT);

    m_vPosition.x = SCREEN_WIDTH / 2.0f;
    //m_position.y = SCREEN_HEIGHT - (m_pSpriteBody->GetHeight() / 2.0f) - ((m_pSpriteLegLeft->GetHeight() + 1.0f) / 2.0f) - 1.0f;
    m_vPosition.y = ((SCREEN_HEIGHT / 3.0f) * 2.0f) + ((SCREEN_HEIGHT / 3.0f) / 2.0f);

    m_fLengthFootToBody = ((float)m_pSprSpriteLegLeft->GetHeight() + 1.0f) / 2.0f;
    /*m_position.x = (float)m_pSpriteBody->GetWidth() / 2.0f;
    m_position.y = (float)SCREEN_HEIGHT - (((float)m_pSpriteBody->GetHeight() / 2.0f)) - m_pLengthFootToBody;*/

    m_vFeetPos.x = SCREEN_WIDTH / 2.0f;
    //m_feetPos.y = SCREEN_HEIGHT - 1.0f;
    m_vFeetPos.y = (((SCREEN_HEIGHT / 3.0f) * 2.0f) + ((float)m_pSprSpriteBody->GetHeight() / 2.0f) +
        m_fLengthFootToBody) + ((SCREEN_HEIGHT / 3.0f) / 2.0f);

    m_vStandingPos.x = SCREEN_WIDTH / 2.0f;
    //m_standingPos.y = SCREEN_HEIGHT - 1.0f;
    m_vStandingPos.y = (((SCREEN_HEIGHT / 3.0f) * 2.0f) + ((float)m_pSprSpriteBody->GetHeight() / 2.0f) +
        m_fLengthFootToBody) + ((SCREEN_HEIGHT / 3.0f) / 2.0f);

    ComputeBounds(SCREEN_WIDTH, SCREEN_HEIGHT);

    return true;
}

void Character::Process(float deltaTime, InputSystem& inputSystem)
{
    GetInputs(inputSystem);
    HandleInput(deltaTime);

    // Checks for boundary collisions
    if (m_vStandingPos.x > (m_vBoundaryHigh.x))
    {
        m_vPosition.x = m_vBoundaryHigh.x - 0.1f;
        m_velocityBody.x = 0.0f;
        m_vFeetPos.x = m_vBoundaryHigh.x - 0.1f;
        m_vStandingPos.x = m_vBoundaryHigh.x - 0.1f;
        m_velocityPos.x = 0.0f;
    }
    else if (m_vStandingPos.x < (m_vBoundaryLow.x))
    {
        m_vPosition.x = m_vBoundaryLow.x + 0.1f;
        m_velocityBody.x = 0.0f;
        m_vFeetPos.x = m_vBoundaryLow.x + 0.1f;
        m_vStandingPos.x = m_vBoundaryLow.x + 0.1f;
        m_velocityPos.x = 0.0f;
    }

    if (m_vStandingPos.y > (m_vBoundaryHigh.y))
    {
        //m_position.y = m_boundaryHigh.y - ((float)m_pSpriteBody->GetHeight() / 2.0f) - m_pLengthFootToBody;
        m_velocityBody.y = 0.0f;
        //m_feetPos.y = m_boundaryHigh.y;
        m_vStandingPos.y = m_vBoundaryHigh.y;
        m_velocityPos.y = 0.0f;
    }
    else if (m_vStandingPos.y < (m_vBoundaryLow.y))
    {
        //m_position.y = m_boundaryLow.y - ((float)m_pSpriteBody->GetHeight() / 2.0f) - m_pLengthFootToBody;
        m_velocityBody.y = 0.0f;
        //m_feetPos.y = m_boundaryLow.y;
        m_vStandingPos.y = m_vBoundaryLow.y;
        m_velocityPos.y = 0.0f;
    }

    m_vPosition += (m_velocityBody + m_velocityJump) * deltaTime;
    m_vFeetPos += (m_velocityBody + m_velocityJump) * deltaTime;
    m_vStandingPos += m_velocityPos * deltaTime;

    // Update position of character's body parts
    m_pSprSpriteBody->SetX((int)m_vPosition.x);
    m_pSprSpriteBody->SetY((int)m_vPosition.y);

    m_pSprSpriteHead->SetX((int)m_vPosition.x);
    m_pSprSpriteHead->SetY((int)m_vPosition.y - (int)(m_pSprSpriteBody->GetHeight() / 2.0f) - (int)m_fHeadBodyOffset);

    m_pSprSpriteShadow->SetX((int)m_vStandingPos.x);
    m_pSprSpriteShadow->SetY((int)m_vStandingPos.y);

    HandleLegs(deltaTime);
}

void Character::Draw(Renderer& renderer)
{
    if (m_bAlive)
    {
        m_pSprSpriteShadow->Draw(renderer, false, false);
        m_pSprSpriteLegLeft->Draw(renderer, false, false);
        m_pSprSpriteLegRight->Draw(renderer, false, false);

        if (m_iFacingDirection > 0)
        {
            m_pSprSpriteBody->Draw(renderer, false, false);
            m_pSprSpriteHead->Draw(renderer, false, false);
        }
        else if (m_iFacingDirection < 0)
        {
            m_pSprSpriteBody->Draw(renderer, true, false);
            m_pSprSpriteHead->Draw(renderer, true, false);
        }
    }
}

void Character::GetInputs(InputSystem& inputSystem)
{
    // Gets movement keys' states
    m_sMotionKeyStates.MoveForward = inputSystem.GetKeyState(SDL_SCANCODE_D);
    m_sMotionKeyStates.MoveBackward = inputSystem.GetKeyState(SDL_SCANCODE_A);
    m_sMotionKeyStates.MoveUp = inputSystem.GetKeyState(SDL_SCANCODE_W);
    m_sMotionKeyStates.MoveDown = inputSystem.GetKeyState(SDL_SCANCODE_S);
    m_sMotionKeyStates.Jump = inputSystem.GetKeyState(SDL_SCANCODE_SPACE);

    if (m_sMotionKeyStates.Jump == BS_PRESSED)
    {
        m_sKeyboardMotions.Heave = MOTION_JUMP;
    }

    if (m_sMotionKeyStates.MoveForward == BS_PRESSED || m_sMotionKeyStates.MoveForward == BS_HELD)
    {
        m_sKeyboardMotions.Surge = MOTION_FORWARD;
    }
    else if (m_sMotionKeyStates.MoveBackward == BS_PRESSED || m_sMotionKeyStates.MoveBackward == BS_HELD)
    {
        m_sKeyboardMotions.Surge = MOTION_BACKWARD;
    }
    else
    {
        m_sKeyboardMotions.Surge = MOTION_NONE;
    }

    if (m_sMotionKeyStates.MoveUp == BS_PRESSED || m_sMotionKeyStates.MoveUp == BS_HELD)
    {
        m_sKeyboardMotions.Sway = MOTION_UP;
    }
    else if (m_sMotionKeyStates.MoveDown == BS_PRESSED || m_sMotionKeyStates.MoveDown == BS_HELD)
    {
        m_sKeyboardMotions.Sway = MOTION_DOWN;
    }
    else
    {
        m_sKeyboardMotions.Sway = MOTION_NONE;
    }
}

void Character::HandleInput(float deltaTime)
{
    // Check for jump input
    if (m_sKeyboardMotions.Heave > 0)
    {
        if (!m_bJumping)
        {
            // Start jump
            m_bJumping = true;
            m_bMovingX = false;
            m_bMovingY = false;
            m_vFeetPos.y -= 1.0f;
            m_velocityJump.y = -400.0f;
        }
        else if (m_bJumping)
        {
            // Start decent
            if ((m_vFeetPos.y <= (m_vStandingPos.y - (m_fTotalHeight * 1.5f))) || (m_velocityJump.y >= 0.0f))
            {
                m_velocityJump.y = 0.0f;
                m_sKeyboardMotions.Heave = MOTION_DECENT;
                return;
            }

            m_velocityJump.y += 600.0f * deltaTime;
        }
    }
    else if (m_sKeyboardMotions.Heave < 0)
    {
        // Feet on ground
        if (m_vFeetPos.y >= m_vStandingPos.y)
        {
            m_vPosition.y = m_vStandingPos.y - ((float)m_pSprSpriteBody->GetHeight() / 2.0f) - m_fLengthFootToBody;
            m_bJumping = false;
            m_sKeyboardMotions.Heave = MOTION_NONE;
            m_vFeetPos.y = m_vStandingPos.y;
            m_velocityJump.y = 0.0f;
            m_velocityBody.y = 0.0f;
            return;
        }

        m_velocityJump.y += 600.0f * deltaTime;
    }

    // Handles forward and backward motions
    if (m_sKeyboardMotions.Surge > 0)
    {
        m_velocityBody.x = 200.0f;
        m_velocityPos.x = 200.0f;
        m_iFacingDirection = 1;
        m_bMovingX = true;
    }
    else if (m_sKeyboardMotions.Surge < 0)
    {
        m_velocityBody.x = -200.0f;
        m_velocityPos.x = -200.0f;
        m_iFacingDirection = -1;
        m_bMovingX = true;
    }
    else if (m_sKeyboardMotions.Surge == 0)
    {
        m_velocityBody.x = 0.0f;
        m_velocityPos.x = 0.0f;

        if (!m_bJumping)
        {
            m_bMovingX = false;
        }
    }

    // Handles up and down motions
    if (m_sKeyboardMotions.Sway > 0)
    {
        if (m_vStandingPos.y > m_vBoundaryLow.y)
        {
            m_velocityBody.y = -200.0f;
            m_velocityPos.y = -200.0f;
        }

        m_bMovingY = true;
    }
    else if (m_sKeyboardMotions.Sway < 0)
    {
        if (m_vStandingPos.y < m_vBoundaryHigh.y)
        {
            m_velocityBody.y = 200.0f;
            m_velocityPos.y = 200.0f;
        }

        m_bMovingY = true;
    }
    else if (m_sKeyboardMotions.Sway == 0)
    {
        m_velocityBody.y = 0.0f;
        m_velocityPos.y = 0.0f;

        if (!m_bJumping)
        {
            m_bMovingY = false;
        }
    }
}

Vector2& Character::GetPosition()
{
    return m_vPosition;
}

void Character::ComputeBounds(int width, int height)
{
    // Set boundaries
    m_vBoundaryLow.x = (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryLow.y = ((sm_fBoundaryHeight / 3.0f) * 2.0f);

    m_vBoundaryHigh.x = width - (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryHigh.y = (float)height;
}

void Character::HandleLegs(float deltaTime)
{
    // Calculate positions of legs at resting state
    int restingLeftLegX = (int)m_vPosition.x - (m_pSprSpriteBody->GetWidth() / 5);
    int restingRightLegX = (int)m_vPosition.x + (m_pSprSpriteBody->GetWidth() / 5);
    int restingLegY = (int)m_vPosition.y + (m_pSprSpriteBody->GetHeight() / 2);

    if ((!m_bMovingX && !m_bMovingY) || m_bJumping)
    {
        // Set positions of legs at resting state
        m_pSprSpriteLegLeft->SetX(restingLeftLegX);
        m_pSprSpriteLegLeft->SetY(restingLegY);

        m_pSprSpriteLegRight->SetX(restingRightLegX);
        m_pSprSpriteLegRight->SetY(restingLegY);

        // Reset step timer for next moving state
        m_fStepTimer = 0.0f;
    }
    else
    {
        // Increments step timer
        m_fStepTimer += deltaTime;

        // Calculates step progress through step cycle (0.0 to 1.0)
        float progress = (float)fmod(m_fStepTimer, m_fStepDuration) / m_fStepDuration;

        // Calculate leg movement positions based on cycle progress
        float halfWidth = (float)m_pSprSpriteBody->GetWidth() / 5.0f;
        float maxStepHeight = m_fLegBodyOffset;

        if (m_iFacingDirection > 0)
        {
            // Clockwise cycle
            float leftLegX = restingLeftLegX + halfWidth * sinf(progress * PI);
            float leftLegY = restingLegY - maxStepHeight * sinf(progress * PI);

            float rightLegX = restingRightLegX - 2 * halfWidth * progress;
            float rightLegY = (float)restingLegY;

            // Swap legs' roles halfway through the step cycle
            if (progress >= 0.5f)
            {
                float tempX = leftLegX;
                float tempY = leftLegY;

                leftLegX = rightLegX;
                leftLegY = rightLegY;

                rightLegX = tempX;
                rightLegY = tempY;
            }

            // Set new positions for the legs
            m_pSprSpriteLegLeft->SetX((int)leftLegX);
            m_pSprSpriteLegLeft->SetY((int)leftLegY);

            m_pSprSpriteLegRight->SetX((int)rightLegX);
            m_pSprSpriteLegRight->SetY((int)rightLegY);
        }
        else if (m_iFacingDirection < 0)
        {
            // Anti-clockwise cycle
            float leftLegX = restingLeftLegX + 2 * halfWidth * progress;
            float leftLegY = (float)restingLegY;

            float rightLegX = restingRightLegX - halfWidth * sinf(progress * PI);
            float rightLegY = restingLegY - maxStepHeight * sinf(progress * PI);

            // Swap legs' roles halfway through the step cycle
            if (progress >= 0.5f)
            {
                float tempX = leftLegX;
                float tempY = leftLegY;

                leftLegX = rightLegX;
                leftLegY = rightLegY;

                rightLegX = tempX;
                rightLegY = tempY;
            }

            // Set new positions for the legs
            m_pSprSpriteLegLeft->SetX((int)leftLegX);
            m_pSprSpriteLegLeft->SetY((int)leftLegY);

            m_pSprSpriteLegRight->SetX((int)rightLegX);
            m_pSprSpriteLegRight->SetY((int)rightLegY);
        }
    }
}

//void Character::DebugDraw()
//{
//    ImGui::Text("Character Debug Info");
//    ImGui::InputFloat2("Position", reinterpret_cast<float*>(&m_position));
//    ImGui::InputFloat2("Velocity", reinterpret_cast<float*>(&m_velocity));
//
//    ImGui::Text("Size (%d, %d)", m_pSprite->GetWidth(), m_pSprite->GetHeight());
//    ImGui::Text("Lowerbound (%f, %f)", m_boundaryLow.x, m_boundaryLow.y);
//    ImGui::Text("Upperbound (%f, %f)", m_boundaryHigh.x, m_boundaryHigh.y);
//
//    float spriteScale = m_pSprite->GetScale();
//    ImGui::InputFloat("Scale", &spriteScale, 0.05f);
//    m_pSprite->SetScale(spriteScale);
//    ComputeBounds(static_cast<int>(sm_fBoundaryWidth), static_cast<int>(sm_fBoundaryHeight));
//
//    float colour[4];
//    colour[0] = m_pSprite->GetRedTint();
//    colour[1] = m_pSprite->GetGreenTint();
//    colour[2] = m_pSprite->GetBlueTint();
//    colour[3] = m_pSprite->GetAlpha();
//    ImGui::ColorEdit4("Character colour", colour);
//    m_pSprite->SetRedTint(colour[0]);
//    m_pSprite->SetGreenTint(colour[1]);
//    m_pSprite->SetBlueTint(colour[2]);
//    m_pSprite->SetAlpha(colour[3]);
//}
