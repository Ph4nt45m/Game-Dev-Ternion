// This include:
#include "character.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "inputsystem.h"
#include "../imgui/imgui.h"
#include "animatedsprite.h"
#include "projectile.h"

// Library includes:
#include <cassert>
#include <cstdio>

Character::Character()
    : m_pSprSpriteHead(0)
    , m_pSprSpriteLegLeft(0)
    , m_pSprSpriteLegRight(0)
    , m_pSprSpriteShadow(0)
    , m_pSprWeapon(0)
    , m_pEntArrow(0)
    , m_pASprWeapAttack(0)
    , m_iWeaponType(0)
    , m_fAngleOfAttack(0.0f)
    , m_bDoubleJump(false)
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

    delete m_pSprWeapon;
    m_pSprWeapon = 0;

    delete m_pEntArrow;
    m_pEntArrow = 0;

    delete m_pASprWeapAttack;
    m_pASprWeapAttack = 0;
}

bool
Character::Initialise(Renderer& renderer)
{
    if (!SetBodySprites(renderer))
    {
        LogManager::GetInstance().Log("Character Sprites failed to initialise!");
        return false;
    }

    m_iFacingDirection = 1;
    m_fScale = 1.0f;
    m_fScaleChangeRate = 0.125f;
    m_fScaleMin = 0.92f;
    m_fScaleMax = 1.08f;
    m_fHeadBodyOffset = 14.0f;
    m_fLegBodyOffset = 15.0f;
    m_fStepTimer = 0.0f;
    m_fStepDuration = 0.5f;
    m_iWeaponType = 1;
    m_bAlive = true;

    Entity::SetWindowBoundaries(renderer);

    m_vPosition.x = sm_fBoundaryWidth / 5.0f;
    m_vPosition.y = ((sm_fBoundaryHeight / 7.0f) * 5.0f);

    m_fLengthFootToBody = ((float)m_pSprSpriteLegLeft->GetHeight() + 1.0f) / 2.0f;

    m_vFeetPos.x = sm_fBoundaryWidth / 5.0f;
    m_vFeetPos.y = (((sm_fBoundaryHeight / 7.0f) * 5.0f) + ((float)m_pSprSpriteBody->GetHeight() / 2.0f) +
        m_fLengthFootToBody);

    m_vStandingPos.x = sm_fBoundaryWidth / 5.0f;
    m_vStandingPos.y = (((sm_fBoundaryHeight / 7.0f) * 5.0f) + ((float)m_pSprSpriteBody->GetHeight() / 2.0f) +
        m_fLengthFootToBody);

    ComputeBounds(sm_fBoundaryWidth, sm_fBoundaryHeight);

    switch (m_iWeaponType)
    {
    case 0:
        m_pSprWeapon = renderer.CreateSprite("..\\Sprites\\weaponsstatic\\sword.png");
        m_pASprWeapAttack = renderer.CreateAnimatedSprite("..\\Sprites\\weaponsanim8\\anim8sword.png");

        if (m_pSprWeapon && m_pASprWeapAttack)
        {
            m_pASprWeapAttack->SetupFrames(213, 150);
            m_pASprWeapAttack->SetFrameDuration(0.02f);
        }
        else
        {
            LogManager::GetInstance().Log("Sword Weapon failed to initialise!");
            return false;
        }

        break;
    case 1:
        m_pSprWeapon = renderer.CreateSprite("..\\Sprites\\weaponsstatic\\bow.png");
        m_pASprWeapAttack = renderer.CreateAnimatedSprite("..\\Sprites\\weaponsanim8\\anim8bow.png");
        m_pEntArrow = new Projectile();

        if (m_pSprWeapon && m_pASprWeapAttack)
        {
            m_pASprWeapAttack->SetupFrames(161, 119);
            m_pASprWeapAttack->SetFrameDuration(0.03f);
        }
        else
        {
            LogManager::GetInstance().Log("Bow Weapon failed to initialise!");
            return false;
        }

        if (!(m_pEntArrow->Initialise(renderer)))
        {
            LogManager::GetInstance().Log("Projectile Arrow failed to initialise!");
            return false;
        }
        else
        {
            m_pEntArrow->SetProjectileSprite(renderer, "..\\Sprites\\characterprojectile\\arrow.png");
            m_pEntArrow->SetGroundY(m_vStandingPos.y);
        }

        break;
    case 2:
        m_pSprWeapon = renderer.CreateSprite("..\\Sprites\\weaponsstatic\\staff.png");
        m_pASprWeapAttack = renderer.CreateAnimatedSprite("..\\Sprites\\weaponsanim8\\anim8staff.png");

        if (m_pSprWeapon && m_pASprWeapAttack)
        {
            m_pASprWeapAttack->SetupFrames(67, 119);
            m_pASprWeapAttack->SetFrameDuration(0.15f);
        }
        else
        {
            LogManager::GetInstance().Log("Staff Weapon failed to initialise!");
            return false;
        }

        break;
    }

    return true;
}

void
Character::Process(float deltaTime, InputSystem& inputSystem)
{
    m_vCursor.SetPosition(inputSystem.GetMousePosition());
    GetInputs(inputSystem);
    HandleInput(deltaTime);

    // Sets facing orientation
    if (m_vPosition.x < m_vCursor.x)
    {
        m_iFacingDirection = 1;
        m_bFlipHorizontally = false;
    }
    else if (m_vPosition.x > m_vCursor.x)
    {
        m_iFacingDirection = -1;
        m_bFlipHorizontally = true;
    }

    // Calculates angle of aim
    m_vRelative.x = m_vCursor.x - m_vPosition.x;
    m_vRelative.y = m_vCursor.y - m_vPosition.y;
    m_fAngleOfAttack = -(float)atan2(m_vRelative.y, m_vRelative.x) * 180.0f / PI;

    if (m_fAngleOfAttack < 0)
    {
        m_fAngleOfAttack += 360.0f;
    }

    if (m_iFacingDirection < 0)
    {
        m_fAngleOfAttack += 180.0f;
    }

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
        m_velocityBody.y = 0.0f;
        m_vStandingPos.y = m_vBoundaryHigh.y;
        m_velocityPos.y = 0.0f;
    }
    else if (m_vStandingPos.y < (m_vBoundaryLow.y))
    {
        m_velocityBody.y = 0.0f;
        m_vStandingPos.y = m_vBoundaryLow.y;
        m_velocityPos.y = 0.0f;
    }

    m_vPosition += (m_velocityBody + m_velocityJump) * deltaTime;
    m_vFeetPos += (m_velocityBody + m_velocityJump) * deltaTime;
    m_vStandingPos += m_velocityPos * deltaTime;

    // Update position of character's parts
    m_pSprSpriteBody->SetX((int)m_vPosition.x);
    m_pSprSpriteBody->SetY((int)m_vPosition.y);

    m_pSprSpriteHead->SetX((int)m_vPosition.x);
    m_pSprSpriteHead->SetY((int)m_vPosition.y - (int)(m_pSprSpriteBody->GetHeight() / 2.0f) - (int)m_fHeadBodyOffset);

    // For future updates
    m_pSprSpriteShadow->SetX((int)m_vStandingPos.x);
    m_pSprSpriteShadow->SetY((int)m_vStandingPos.y);

    // Update position of weapon
    m_pSprWeapon->SetX((int)m_vPosition.x);
    m_pSprWeapon->SetY((int)m_vPosition.y - 15);
    m_pASprWeapAttack->SetX((int)m_vPosition.x);
    m_pASprWeapAttack->SetY((int)m_vPosition.y - 15);

    // Update angle of aim of attack
    m_pSprWeapon->SetAngle(m_fAngleOfAttack);
    m_pASprWeapAttack->SetAngle(m_fAngleOfAttack);

    // Update scale of character based on distance from camera
    m_pSprSpriteBody->SetScale(m_fScale);
    m_pSprSpriteHead->SetScale(m_fScale);
    m_pSprSpriteLegLeft->SetScale(m_fScale);
    m_pSprSpriteLegRight->SetScale(m_fScale);
    m_pSprSpriteShadow->SetScale(m_fScale);
    m_pSprWeapon->SetScale(m_fScale);
    m_pASprWeapAttack->SetScale(m_fScale);

    HandleLegs(deltaTime);
    m_pASprWeapAttack->Process(deltaTime);

    if (m_iWeaponType == 1)
    {
        m_pEntArrow->Process(deltaTime, inputSystem);
    }
}

void
Character::Draw(Renderer& renderer)
{
    if (m_bAlive)
    {
        //m_pSprSpriteShadow->Draw(renderer, false, false); // For future updates
        m_pSprSpriteLegLeft->Draw(renderer, false, false);
        m_pSprSpriteLegRight->Draw(renderer, false, false);

        m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, false);
        m_pSprSpriteHead->Draw(renderer, m_bFlipHorizontally, false);

        if (m_pASprWeapAttack->IsAnimating())
        {
            m_pASprWeapAttack->Draw(renderer, m_bFlipHorizontally, false);
        }
        else
        {
            m_pSprWeapon->Draw(renderer, m_bFlipHorizontally, true);
            m_bShoot = false;
        }

        if (m_iWeaponType == 1)
        {
            m_pEntArrow->Draw(renderer);
        }
    }
}

void
Character::GetInputs(InputSystem& inputSystem)
{
    // Gets movement keys' states
    m_sMotionKeyStates.MoveForward = inputSystem.GetKeyState(SDL_SCANCODE_D);
    m_sMotionKeyStates.MoveBackward = inputSystem.GetKeyState(SDL_SCANCODE_A);
    m_sMotionKeyStates.MoveUp = inputSystem.GetKeyState(SDL_SCANCODE_W); // For future updates
    m_sMotionKeyStates.MoveDown = inputSystem.GetKeyState(SDL_SCANCODE_S); // For future updates
    m_sMotionKeyStates.Jump = inputSystem.GetKeyState(SDL_SCANCODE_SPACE);
    m_sMotionKeyStates.LeftClickAttack = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT);

    if (m_sMotionKeyStates.Jump == BS_PRESSED)
    {
        m_sKeyboardMotions.Heave = MOTION_JUMP;
    }

    if (m_sMotionKeyStates.LeftClickAttack == BS_PRESSED)
    {
        m_sKeyboardMotions.Attack = MOTION_ATTACK;
    }
    else
    {
        m_sKeyboardMotions.Attack = MOTION_NONE;
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

void
Character::HandleInput(float deltaTime)
{
    // Check for jump input
    if (m_sKeyboardMotions.Heave > 0)
    {
        if (!m_bJumping && !m_bDoubleJump)
        {
            // Start jump
            if (!m_bJumping)
            {
                m_bJumping = true;
            }

            m_bMovingX = false;
            m_bMovingY = false;
            //m_vFeetPos.y -= 1.0f;
            m_velocityJump.y = -320.0f;
            m_sKeyboardMotions.Heave = MOTION_DECENT;
        }
        else if (m_bJumping && !m_bDoubleJump)
        {
            // Start double jump
            m_bDoubleJump = true;
            m_velocityJump.y = -320.0f;
            m_sKeyboardMotions.Heave = MOTION_DECENT;
        }
        else
        {
            m_sKeyboardMotions.Heave = MOTION_DECENT;
        }
        //else if (m_bJumping)
        //{
        //    // Start decent
        //    if (m_velocityJump.y >= 0.0f)
        //    {
        //        m_sKeyboardMotions.Heave = MOTION_DECENT;
        //        return;
        //    }

        //    m_velocityJump.y += 500.0f * deltaTime;
        //}
    }
    // Start descent
    else if (m_sKeyboardMotions.Heave < 0)
    {
        // Feet on ground
        if (m_vFeetPos.y >= m_vStandingPos.y)
        {
            m_vPosition.y = m_vStandingPos.y - ((float)m_pSprSpriteBody->GetHeight() / 2.0f) - m_fLengthFootToBody;
            m_bJumping = false;
            m_bDoubleJump = false;
            m_sKeyboardMotions.Heave = MOTION_NONE;
            m_vFeetPos.y = m_vStandingPos.y;
            m_velocityJump.y = 0.0f;
            m_velocityBody.y = 0.0f;
            return;
        }

        m_velocityJump.y += 500.0f * deltaTime;
    }

    // Handles main attack
    if (m_sKeyboardMotions.Attack > 0)
    {
        switch (m_iWeaponType)
        {
        case 0:
            m_pASprWeapAttack->Animate();
            break;
        case 1:
            if (!m_pASprWeapAttack->IsAnimating() && !m_pEntArrow->IsAlive())
            {
                m_pASprWeapAttack->Animate();
                m_pEntArrow->SetStartPos(m_vPosition.x, m_vPosition.y);
                m_pEntArrow->SetTargetPos(m_vCursor.x, m_vCursor.y);
                m_pEntArrow->Shoot();
            }
            break;
        case 2:
            m_pASprWeapAttack->Animate();
            break;
        }
    }

    // Handles forward and backward motions
    if (m_sKeyboardMotions.Surge > 0)
    {
        if (!sm_bCameraCentered)
        {
            m_velocityBody.x = 200.0f;
            m_velocityPos.x = 200.0f;
        }
        else
        {
            m_velocityBody.x = 0.0f;
            m_velocityPos.x = 0.0f;
        }
        
        m_iFacingDirection = 1;
        m_bMovingX = true;
    }
    else if (m_sKeyboardMotions.Surge < 0)
    {
        if (!sm_bCameraCentered)
        {
            m_velocityBody.x = -200.0f;
            m_velocityPos.x = -200.0f;
        }
        else
        {
            m_velocityBody.x = 0.0f;
            m_velocityPos.x = 0.0f;
        }
        
        m_iFacingDirection = -1;
        m_bMovingX = true;
    }
    else if (m_sKeyboardMotions.Surge == 0)
    {
        m_velocityBody.x = 0.0f;
        m_velocityPos.x = 0.0f;

        if (!m_bJumping && !m_bDoubleJump)
        {
            m_bMovingX = false;
        }
    }

    //Handles up and down motions - For future updates
    if (m_sKeyboardMotions.Sway > 0)
    {
        if (m_vStandingPos.y > m_vBoundaryLow.y)
        {
            m_velocityBody.y = -200.0f;
            m_velocityPos.y = -200.0f;
            m_fScale -= m_fScaleChangeRate * deltaTime;

            if (m_fScale < m_fScaleMin)
            {
                m_fScale = m_fScaleMin;
            }
        }

        m_bMovingY = true;
    }
    else if (m_sKeyboardMotions.Sway < 0)
    {
        if (m_vStandingPos.y < m_vBoundaryHigh.y)
        {
            m_velocityBody.y = 200.0f;
            m_velocityPos.y = 200.0f;
            m_fScale += m_fScaleChangeRate * deltaTime;

            if (m_fScale > m_fScaleMax)
            {
                m_fScale = m_fScaleMax;
            }
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

bool
Character::SetBodySprites(Renderer& renderer)
{
    m_pSprSpriteHead = renderer.CreateSprite("..\\Sprites\\characterbase\\head.png");

    if (!(m_pSprSpriteHead))
    {
        LogManager::GetInstance().Log("Character Head failed to initialise!");
        return false;
    }

    m_pSprSpriteBody = renderer.CreateSprite("..\\Sprites\\characterbase\\body.png");

    if (!(m_pSprSpriteBody))
    {
        LogManager::GetInstance().Log("Character Body failed to initialise!");
        return false;
    }

    m_pSprSpriteLegLeft = renderer.CreateSprite("..\\Sprites\\characterbase\\leg.png");

    if (!(m_pSprSpriteLegLeft))
    {
        LogManager::GetInstance().Log("Character Left Leg failed to initialise!");
        return false;
    }

    m_pSprSpriteLegRight = renderer.CreateSprite("..\\Sprites\\characterbase\\leg.png");

    if (!(m_pSprSpriteLegRight))
    {
        LogManager::GetInstance().Log("Character Right Leg failed to initialise!");
        return false;
    }

    m_pSprSpriteShadow = renderer.CreateSprite("..\\Sprites\\characterbase\\floorshadow.png");

    if (!(m_pSprSpriteShadow))
    {
        LogManager::GetInstance().Log("Character Shadow failed to initialise!");
        return false;
    }

    m_pSprSpriteBody->SetAngle(180.0f);
    m_pSprSpriteHead->SetAngle(180.0f);
    m_pSprSpriteLegLeft->SetAngle(180.0f);
    m_pSprSpriteLegRight->SetAngle(180.0f);
    m_pSprSpriteShadow->SetAngle(180.0f);

    return true;
}

Vector2&
Character::GetPosition()
{
    return m_vStandingPos;
}

Vector2&
Character::GetFeetPos()
{
    return m_vFeetPos;
}

Vector2&
Character::GetVelocityBody()
{
    return m_velocityBody;
}

int
Character::GetWeapontype()
{
    return m_iWeaponType;
}

Vector2&
Character::GetProjectilePos()
{
    return m_pEntArrow->GetPosition();
}

float
Character::GetProjWidth()
{
    return m_pEntArrow->GetWidth();
}

float
Character::GetProjHeight()
{
    return m_pEntArrow->GetHeight();
}

bool
Character::IsProjAlive()
{
    return m_pEntArrow->IsAlive();
}

void
Character::SetProjAlive(bool alive)
{
    m_pEntArrow->SetAlive(alive);
}

void
Character::SetCentered(bool centered)
{
    sm_bCameraCentered = centered;
}

bool
Character::IsCentered()
{
    return sm_bCameraCentered;
}

void
Character::SetTerrainMoving(bool moving)
{
    sm_bTerrainMoving = moving;
}

bool
Character::IsTerrainMoving()
{
    return sm_bTerrainMoving;
}

int
Character::GetBodyWidth()
{
    return m_pSprSpriteBody->GetWidth();
}

void
Character::SetNumSegments(int amount)
{
    sm_iNumSegments = amount;
}

void
Character::ShiftX(float amount)
{
    m_vPosition.x += amount;
    m_vFeetPos.x += amount;
    m_vStandingPos.x += amount;
}

void
Character::ShiftY(float amount)
{
    m_vStandingPos.y += amount;
    m_vBoundaryLow.y += amount;
    m_vBoundaryHigh.y += amount;

    if (m_vFeetPos.y < m_vStandingPos.y)
    {
        if (m_velocityJump.y == 0.0f)
        {
            m_bJumping = true;

            if (amount < 90)
            {
                m_bDoubleJump = true;
            }

            m_sKeyboardMotions.Heave = MOTION_DECENT;
        }
    }
}

void
Character::ComputeBounds(float width, float height)
{
    // Set boundaries
    m_vBoundaryLow.x = (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryLow.y = ((sm_fBoundaryHeight / 7.0f) * 5.0f);

    m_vBoundaryHigh.x = width - (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryHigh.y = height;
}

void
Character::HandleLegs(float deltaTime)
{
    // Calculate positions of legs at resting state
    int restingLeftLegX = (int)m_vPosition.x - (m_pSprSpriteBody->GetWidth() / 5);
    int restingRightLegX = (int)m_vPosition.x + (m_pSprSpriteBody->GetWidth() / 5);
    int restingLegY = (int)m_vPosition.y + (m_pSprSpriteBody->GetHeight() / 2);

    if ((!m_bMovingX && !m_bMovingY) || m_bJumping || m_bDoubleJump)
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

        if (m_sKeyboardMotions.Surge > 0)
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
        else if (m_sKeyboardMotions.Surge < 0)
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

//void
//Character::DebugDraw()
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
