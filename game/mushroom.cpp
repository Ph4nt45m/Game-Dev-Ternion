// This include:
#include "mushroom.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "../imgui/imgui.h"
#include "character.h"

// Library includes:
#include <cassert>
#include <cstdio>

Mushroom::Mushroom()
    : m_pEntCharacter(0)
    , m_pSprSpriteBody(0)
    , m_sAnimations{ 0, 0, 0 }
    , m_iNumSegments(0)
    , m_iNumWalkableSegs(0)
    , m_fAnimateScale(0.0f)
    , m_fExecutionTime(0.0f)
    , m_fDistToPlayer(0.0f)
    , m_fHitBoxRange(0.0f)
    , m_fAttackRangeMax(0.0f)
    , m_fGroundY(0.0f)
    , m_bPlayerInRange(false)
    , m_bSpotted(false)
    , m_bEngage(false)
    , m_bIsAnimating(false)
    , m_bAttack(false)
    , m_bRun(false)
{

}

Mushroom::~Mushroom()
{
    delete m_pSprSpriteBody;
    m_pSprSpriteBody = 0;

    delete m_sAnimations.m_pASprMushIdle;
    m_sAnimations.m_pASprMushIdle = 0;

    delete m_sAnimations.m_pASprSkelWalk;
    m_sAnimations.m_pASprSkelWalk = 0;

    delete m_sAnimations.m_pASprMushAttack;
    m_sAnimations.m_pASprMushAttack = 0;
}

bool
Mushroom::Initialise(Renderer& renderer)
{
    if (!SetBodySprites(renderer))
    {
        LogManager::GetInstance().Log("Mushroom Sprites failed to initialise!");
        return false;
    }

    m_fScale = 1.0f;
    m_fScaleChangeRate = 0.125f;
    m_fScaleMin = 0.92f;
    m_fScaleMax = 1.08f;
    m_fAnimateScale = 0.435f;
    m_fHitBoxRange = 50.0f;
    m_fAttackRangeMax = (m_sAnimations.m_pASprMushAttack->GetWidth() / 2.0f);
    m_fGroundY = ((sm_fBoundaryHeight / 7.0f) * 5.0f);
    m_bAlive = true;

    m_vPosition.x = sm_fSegmentWidth * 4;
    m_vPosition.y = m_fGroundY - 85.0f;

    return true;
}

void
Mushroom::Process(float deltaTime, InputSystem& inputSystem)
{
    GetInputs(inputSystem);
    HandleInput(deltaTime);
    CheckPlayerDist();

    if (m_bPlayerInRange)
    {
        m_fExecutionTime += deltaTime;

        if (!m_bRun && (((int)m_fExecutionTime % 5) == 0))
        {
            Action();
        }

        if (m_bRun)
        {
            Move();

            if (!m_sAnimations.m_pASprSkelWalk->IsAnimating())
            {
                m_sAnimations.m_pASprSkelWalk->Animate();
                m_sAnimations.m_pASprSkelWalk->SetLooping(true);
            }
        }
        else
        {
            m_sAnimations.m_pASprSkelWalk->Inanimate();
            m_sAnimations.m_pASprSkelWalk->SetLooping(false);
            m_sAnimations.m_pASprMushIdle->Animate();
            m_sAnimations.m_pASprMushIdle->SetLooping(true);
        }
    }
    else
    {
        m_fExecutionTime = 0.0f;
        m_sAnimations.m_pASprMushIdle->Animate();
        m_sAnimations.m_pASprMushIdle->SetLooping(true);
    }

    ProcessAction();

    m_vPosition += (m_velocityPos + m_velocityBody) * deltaTime;

    m_sAnimations.m_pASprMushIdle->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprMushIdle->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprSkelWalk->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprSkelWalk->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprMushAttack->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprMushAttack->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprMushIdle->Process(deltaTime);
    m_sAnimations.m_pASprSkelWalk->Process(deltaTime);
    m_sAnimations.m_pASprMushAttack->Process(deltaTime);
}

void
Mushroom::Draw(Renderer& renderer)
{
    if (m_bAlive)
    {
        if (m_bPlayerInRange)
        {
            if (m_bRun)
            {
                m_sAnimations.m_pASprSkelWalk->Draw(renderer, m_bFlipHorizontally, false);
            }
            else if (m_bAttack)
            {
                if (m_sAnimations.m_pASprMushAttack->IsAnimating())
                {
                    m_sAnimations.m_pASprMushAttack->Draw(renderer, m_bFlipHorizontally, false);
                }
                else
                {
                    m_bAttack = false;
                    m_bIsAnimating = false;
                }
            }
            else
            {
                m_sAnimations.m_pASprMushIdle->Draw(renderer, m_bFlipHorizontally, false);
            }
        }
        else
        {
            m_sAnimations.m_pASprMushIdle->Draw(renderer, m_bFlipHorizontally, false);
        }
    }
}

void
Mushroom::GetInputs(InputSystem& inputSystem)
{
    // Gets movement keys' states
    m_sMotionKeyStates.MoveForward = inputSystem.GetKeyState(SDL_SCANCODE_D);
    m_sMotionKeyStates.MoveBackward = inputSystem.GetKeyState(SDL_SCANCODE_A);

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
}

void
Mushroom::HandleInput(float deltaTime)
{
    // Handles left and right shift whilst player is out of range
    if (m_sKeyboardMotions.Surge > 0)
    {
        if (sm_bCameraCentered && sm_bTerrainMoving)
        {
            m_velocityPos.x = -200.0f;
        }
        else
        {
            m_velocityPos.x = 0.0f;
        }

        m_bMovingX = true;
    }
    else if (m_sKeyboardMotions.Surge < 0)
    {
        if (sm_bCameraCentered && sm_bTerrainMoving)
        {
            m_velocityPos.x = 200.0f;
        }
        else
        {
            m_velocityPos.x = 0.0f;
        }

        m_bMovingX = true;
    }
    else if (m_sKeyboardMotions.Surge == 0)
    {
        m_velocityPos.x = 0.0f;

        if (!m_bJumping)
        {
            m_bMovingX = false;
        }
    }
}

bool
Mushroom::SetBodySprites(Renderer& renderer)
{
    m_sAnimations.m_pASprMushIdle = renderer.CreateAnimatedSprite("..\\Sprites\\mushroom\\anim8mushidle.png");

    if (!(m_sAnimations.m_pASprMushIdle))
    {
        LogManager::GetInstance().Log("Mushroom idle animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprMushIdle->SetupFrames(80, 64);
        m_sAnimations.m_pASprMushIdle->SetFrameDuration(0.07f);
        m_sAnimations.m_pASprMushIdle->SetScale(2.5f);
    }

    m_sAnimations.m_pASprMushAttack = renderer.CreateAnimatedSprite("..\\Sprites\\mushroom\\anim8mushattack.png");

    if (!(m_sAnimations.m_pASprMushAttack))
    {
        LogManager::GetInstance().Log("Mushroom attack animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprMushAttack->SetupFrames(80, 64);
        m_sAnimations.m_pASprMushAttack->SetFrameDuration(0.15f);
        m_sAnimations.m_pASprMushAttack->SetScale(2.5f);
    }

    m_sAnimations.m_pASprSkelWalk = renderer.CreateAnimatedSprite("..\\Sprites\\mushroom\\anim8mushrun.png");

    if (!(m_sAnimations.m_pASprSkelWalk))
    {
        LogManager::GetInstance().Log("Mushroom run animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSkelWalk->SetupFrames(80, 64);
        m_sAnimations.m_pASprSkelWalk->SetFrameDuration(0.1f);
        m_sAnimations.m_pASprSkelWalk->SetScale(2.5f);
    }

    return true;
}

void
Mushroom::Move(void)
{
    if (m_bPlayerInRange)
    {
        if (m_pEntCharacter->GetPosition().x < (m_vPosition.x - m_fAttackRangeMax) &&
            m_pEntCharacter->GetPosition().x < m_vPosition.x)
        {
            m_iFacingDirection = 1;
            m_bFlipHorizontally = false;
            m_bIsAnimating = true;
            m_velocityBody.x = -150.0f;
        }
        else if (m_pEntCharacter->GetPosition().x > (m_vPosition.x + m_fAttackRangeMax) &&
            m_pEntCharacter->GetPosition().x > m_vPosition.x)
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_bIsAnimating = true;
            m_velocityBody.x = 150.0f;
        }
        else
        {
            m_velocityBody.x = 0.0f;
            m_bIsAnimating = false;
            m_bRun = false;
            m_fExecutionTime = 0.0f;
        }
    }
}

void
Mushroom::Action()
{
    if (m_fDistToPlayer < m_fAttackRangeMax && m_fDistToPlayer >= 0.0f)
    {
        if (!m_bRun)
        {
            m_sAnimations.m_pASprSkelWalk->SetLooping(false);
            m_sAnimations.m_pASprSkelWalk->Inanimate();
        }

        if (m_sAnimations.m_pASprMushIdle->IsAnimating())
        {
            m_sAnimations.m_pASprMushIdle->Inanimate();
            m_sAnimations.m_pASprMushIdle->SetLooping(false);
        }

        if (!m_sAnimations.m_pASprMushAttack->IsAnimating() && !m_bAttack)
        {
            m_sAnimations.m_pASprMushAttack->Animate();
            m_bAttack = true;
        }
    }
    else
    {
        m_bRun = true;
    }
}

void
Mushroom::ProcessAction()
{
    
}

void
Mushroom::SetTerrainMoving(bool moving)
{
    sm_bTerrainMoving = moving;
}

bool
Mushroom::IsTerrainMoving()
{
    return sm_bTerrainMoving;
}

Vector2&
Mushroom::GetPosition()
{
    return m_vStandingPos;
}

void
Mushroom::SetCharacter(Character& character)
{
    m_pEntCharacter = &character;
}

void
Mushroom::CheckPlayerDist()
{
    m_fDistToPlayer = m_vPosition.x - m_pEntCharacter->GetPosition().x;

    if (m_fDistToPlayer < 0)
    {
        m_fDistToPlayer *= -1.0f;
    }

    if (m_fDistToPlayer <= sm_fSegmentWidth * (m_bSpotted ? 2.5f : 1.6f))
    {
        m_bPlayerInRange = true;

        if (!m_bSpotted)
        {
            m_bSpotted = true;
        }
    }
    else if (m_fDistToPlayer > (sm_fSegmentWidth * 2.5f) && m_bPlayerInRange)
    {
        m_bPlayerInRange = false;
        m_bSpotted = false;
        m_sAnimations.m_pASprMushIdle->Animate();
        m_sAnimations.m_pASprMushIdle->SetLooping(true);
    }
}

int
Mushroom::GetBodyWidth()
{
    return (int)(m_fHitBoxRange * 2);
}

void
Mushroom::ShiftX(float amount)
{
    m_vPosition.x += amount;
    m_vStandingPos.x += amount;
}

void
Mushroom::SetNumSegments(int amount)
{
    m_iNumSegments = amount;
}

void
Mushroom::SetNumWalkableSegs(int amount)
{
    m_iNumWalkableSegs = amount;
}

void
Mushroom::ComputeBounds(float width, float height)
{
    // Set boundaries
    m_vBoundaryLow.x = (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryLow.y = ((sm_fBoundaryHeight / 7.0f) * 4.0f);

    m_vBoundaryHigh.x = width - (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryHigh.y = height;
}

//void
//Mushroom::DebugDraw()
//{
//
//}