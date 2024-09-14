// This include:
#include "golem.h"

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

Golem::Golem()
    : m_pEntCharacter(0)
    , m_pSprSpriteBody(0)
    , m_pSprGolemProjectile(0)
    , m_sAnimations{ 0, 0, 0, 0, 0 }
    , m_iNumSegments(0)
    , m_iNumWalkableSegs(0)
    , m_fAnimateScale(0.0f)
    , m_fExecutionTime(0.0f)
    , m_iPrevAttackType(0)
    , m_fDistToPlayer(0.0f)
    , m_fHitBoxRange(0.0f)
    , m_fSlashRangeMax(0.0f)
    , m_fSlamRangeMax(0.0f)
    , m_fThrowRangeMax(0.0f)
    , m_bPlayerInRange(false)
    , m_bSpotted(false)
    , m_bIsAnimating(false)
    , m_bSlam(false)
{

}

Golem::~Golem()
{
    delete m_pSprSpriteBody;
    m_pSprSpriteBody = 0;

    delete m_pSprGolemProjectile;
    m_pSprGolemProjectile = 0;

    delete m_sAnimations.m_pASprGolemWalk;
    m_sAnimations.m_pASprGolemWalk = 0;

    delete m_sAnimations.m_pASprGolemSlash;
    m_sAnimations.m_pASprGolemSlash = 0;

    delete m_sAnimations.m_pASprGolemJump;
    m_sAnimations.m_pASprGolemJump = 0;

    delete m_sAnimations.m_pASprGolemSlam;
    m_sAnimations.m_pASprGolemSlam = 0;

    delete m_sAnimations.m_pASprGolemThrow;
    m_sAnimations.m_pASprGolemThrow = 0;
}

bool
Golem::Initialise(Renderer& renderer)
{
    if (!SetBodySprites(renderer))
    {
        LogManager::GetInstance().Log("Golem Sprites failed to initialise!");
        return false;
    }

    m_iFacingDirection = -1;
    m_bFlipHorizontally = true;
    m_fScale = 1.0f;
    m_fScaleChangeRate = 0.125f;
    m_fScaleMin = 0.92f;
    m_fScaleMax = 1.08f;
    m_fAnimateScale = 0.435f;
    m_fHitBoxRange = 50.0f;
    m_fSlashRangeMax = (m_sAnimations.m_pASprGolemSlash->GetWidth() / 2.0f) - 15;
    m_fSlamRangeMax = (m_sAnimations.m_pASprGolemSlam->GetWidth() / 2.0f);
    m_fThrowRangeMax = (sm_fSegmentWidth * 2.0f);
    m_bAlive = true;

    m_vPosition.x = sm_fSegmentWidth * (sm_iNumSegments - 3);
    m_vPosition.y = ((sm_fBoundaryHeight / 7.0f) * 4.0f) + ((sm_fBoundaryHeight / 7.0f) - 85.0f);
    m_vStartingPos.x = sm_fSegmentWidth * (sm_iNumSegments - 3);
    m_vStartingPos.y = ((sm_fBoundaryHeight / 7.0f) * 4.0f) + ((sm_fBoundaryHeight / 7.0f) - 85.0f);

	return true;
}

void
Golem::Process(float deltaTime, InputSystem& inputSystem)
{
    GetInputs(inputSystem);
    HandleInput(deltaTime);
    CheckPlayerDist();

    if (m_bPlayerInRange)
    {
        m_fExecutionTime += deltaTime;

        if (m_fExecutionTime > 60.0f)
        {
            m_fExecutionTime -= 60.0f;
        }

        if (m_bSpotted)
        {
            Action();
        }
    }
    else
    {
        m_fExecutionTime = 0.0f;
    }

    ProcessAction();

    m_vPosition += (m_velocityPos + m_velocityBody) * deltaTime;

    if (m_bProjectile)
    {
        m_vProjectilePos += m_velocityProjectile * deltaTime;

        if (m_vProjectileStartPos.x > m_vProjectileEndPos.x)
        {
            if (m_vProjectilePos.x > m_vProjectileEndPos.x)
            {
                m_pSprGolemProjectile->SetX((int)m_vProjectilePos.x);
                m_pSprGolemProjectile->SetY((int)m_vProjectilePos.y);
            }
            else
            {
                m_bProjectile = false;
            }
        }
        else if (m_vProjectileStartPos.x < m_vProjectileEndPos.x)
        {
            if (m_vProjectilePos.x < m_vProjectileEndPos.x)
            {
                m_pSprGolemProjectile->SetX((int)m_vProjectilePos.x);
                m_pSprGolemProjectile->SetY((int)m_vProjectilePos.y);
            }
            else
            {
                m_bProjectile = false;
            }
        }
    }

    m_pSprSpriteBody->SetX((int)m_vPosition.x);
    m_pSprSpriteBody->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprGolemWalk->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprGolemWalk->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprGolemWalk->Process(deltaTime);
}

void
Golem::Draw(Renderer& renderer)
{
    if (m_bAlive)
    {
        if (m_bPlayerInRange)
        {
            if (m_fDistToPlayer > m_fHitBoxRange)
            {
                if (m_sAnimations.m_pASprGolemWalk->IsAnimating())
                {
                    m_sAnimations.m_pASprGolemWalk->Draw(renderer, m_bFlipHorizontally, false);
                }
                else if (m_bSlash)
                {
                    if (m_sAnimations.m_pASprGolemSlash->IsAnimating())
                    {
                        m_sAnimations.m_pASprGolemSlash->Draw(renderer, m_bFlipHorizontally, false);
                    }
                    else
                    {
                        m_bSlash = false;
                    }
                }
                else if (m_bSlam)
                {
                    if (m_sAnimations.m_pASprGolemJump->IsAnimating())
                    {
                        m_sAnimations.m_pASprGolemJump->Draw(renderer, m_bFlipHorizontally, false);
                    }
                    else
                    {
                        if (m_bJumping)
                        {
                            m_sAnimations.m_pASprGolemSlam->Animate();
                            m_bJumping = false;
                        }
                        else if (m_sAnimations.m_pASprGolemSlam->IsAnimating())
                        {
                            m_sAnimations.m_pASprGolemSlam->Draw(renderer, m_bFlipHorizontally, false);
                        }
                        else
                        {
                            m_bSlam = false;
                        }
                    }
                }
                else if (m_bProjectile)
                {
                    if (m_sAnimations.m_pASprGolemThrow->IsAnimating())
                    {
                        m_sAnimations.m_pASprGolemThrow->Draw(renderer, m_bFlipHorizontally, false);
                    }
                    else
                    {
                        if (m_bShoot || m_bProjectile)
                        {
                            m_pSprGolemProjectile->Draw(renderer, m_bFlipHorizontally, false);
                            m_bShoot = false;
                        }
                    }
                }
            }
            else
            {
                m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, true);
            }
        }
        else
        {
            m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, true);
        }
    }
}

void
Golem::GetInputs(InputSystem& inputSystem)
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
Golem::HandleInput(float deltaTime)
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
Golem::SetBodySprites(Renderer& renderer)
{
    m_pSprSpriteBody = renderer.CreateSprite("..\\Sprites\\golem\\golem.png");

    if (!(m_pSprSpriteBody))
    {
        LogManager::GetInstance().Log("Golem failed to initialise!");
        return false;
    }

    m_pSprGolemProjectile = renderer.CreateSprite("..\\Sprites\\golem\\test.png");

    if (!(m_pSprGolemProjectile))
    {
        LogManager::GetInstance().Log("Golem Projectile failed to initialise!");
        return false;
    }

    m_sAnimations.m_pASprGolemWalk = renderer.CreateAnimatedSprite("..\\Sprites\\golem\\anim8golemwalk.png");

    if (!(m_sAnimations.m_pASprGolemWalk))
    {
        LogManager::GetInstance().Log("Golem Walk animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprGolemWalk->SetupFrames(391, 391);
        m_sAnimations.m_pASprGolemWalk->SetFrameDuration(0.07f);
    }

    m_sAnimations.m_pASprGolemSlash = renderer.CreateAnimatedSprite("..\\Sprites\\golem\\anim8golemslash.png");

    if (!(m_sAnimations.m_pASprGolemSlash))
    {
        LogManager::GetInstance().Log("Golem Slash animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprGolemSlash->SetupFrames(391, 391);
        m_sAnimations.m_pASprGolemSlash->SetFrameDuration(0.15f);
    }

    m_sAnimations.m_pASprGolemJump = renderer.CreateAnimatedSprite("..\\Sprites\\golem\\anim8golemjump.png");

    if (!(m_sAnimations.m_pASprGolemJump))
    {
        LogManager::GetInstance().Log("Golem Jump animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprGolemJump->SetupFrames(391, 391);
        m_sAnimations.m_pASprGolemJump->SetFrameDuration(0.15f);
    }

    m_sAnimations.m_pASprGolemSlam = renderer.CreateAnimatedSprite("..\\Sprites\\golem\\anim8golemslam.png");

    if (!(m_sAnimations.m_pASprGolemSlam))
    {
        LogManager::GetInstance().Log("Golem Slam animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprGolemSlam->SetupFrames(391, 391);
        m_sAnimations.m_pASprGolemSlam->SetFrameDuration(0.07f);
    }

    m_sAnimations.m_pASprGolemThrow = renderer.CreateAnimatedSprite("..\\Sprites\\golem\\anim8golemthrow.png");

    if (!(m_sAnimations.m_pASprGolemThrow))
    {
        LogManager::GetInstance().Log("Golem Throw animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprGolemThrow->SetupFrames(391, 391);
        m_sAnimations.m_pASprGolemThrow->SetFrameDuration(0.15f);
    }

    return true;
}

void
Golem::Move()
{
    if (m_bPlayerInRange)
    {
        if (m_pEntCharacter->GetPosition().x < (m_vPosition.x - m_fHitBoxRange))
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_velocityBody.x = -150.0f;
        }
        else if (m_pEntCharacter->GetPosition().x > (m_vPosition.x + m_fHitBoxRange))
        {
            m_iFacingDirection = 1;
            m_bFlipHorizontally = false;
            m_velocityBody.x = 150.0f;
        }
        else
        {
            m_velocityBody.x = 0.0f;
        }
    }
    else
    {
        if ((int)m_vPosition.x < (int)m_vStartingPos.x)
        {
            m_iFacingDirection = 1;
            m_bFlipHorizontally = false;
            m_velocityBody.x = 150.0f;
        }
        else if ((int)m_vPosition.x > (int)m_vStartingPos.x)
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_velocityBody.x = -150.0f;
        }
        else
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_velocityBody.x = 0.0f;
        }
    }
}

void
Golem::Action()
{
    /*if (m_fDistToPlayer > m_fHitBoxRange)
    {
        Move();
        m_sAnimations.m_pASprGolemWalk->Animate();
        m_sAnimations.m_pASprGolemWalk->SetLooping(true);
    }
    else
    {
        m_sAnimations.m_pASprGolemWalk->SetLooping(false);
    }*/

    if (m_fDistToPlayer > m_fThrowRangeMax && !m_bIsAnimating)
    {
        Move();
        m_sAnimations.m_pASprGolemWalk->Animate();
        m_sAnimations.m_pASprGolemWalk->SetLooping(true);
    }
    else
    {
        m_sAnimations.m_pASprGolemWalk->SetLooping(false);
        m_sAnimations.m_pASprGolemWalk->Inanimate();
        IsAnimating();

        if (m_fDistToPlayer >= m_fSlamRangeMax && m_fDistToPlayer <= m_fThrowRangeMax && !m_bIsAnimating && m_iPrevAttackType != 2)
        {
            m_sAnimations.m_pASprGolemThrow->Animate();
            m_iPrevAttackType = 2;
            m_bShoot = true;
            m_bProjectile = true;

            if (m_vProjectileStartPos.x == 0.0f && m_vProjectileEndPos.x == 0.0f)
            {
                m_vProjectilePos.x = m_vPosition.x +
                    (m_fHitBoxRange * (m_pEntCharacter->GetPosition().x > m_vPosition.x ? 1.0f : -1.0f));
                m_vProjectilePos.y = m_vPosition.y;
                m_vProjectileStartPos.x = m_vPosition.x +
                    (m_fHitBoxRange * (m_pEntCharacter->GetPosition().x > m_vPosition.x ? 1.0f : -1.0f));
                m_vProjectileStartPos.y = m_vPosition.y;
                m_vProjectileEndPos.x = m_pEntCharacter->GetPosition().x +
                    ((m_pEntCharacter->GetBodyWidth() / 2) * (m_pEntCharacter->GetPosition().x > m_vPosition.x ? 1.0f : -1.0f));
                m_vProjectileEndPos.y = m_pEntCharacter->GetPosition().y;
            }
        }
        else if (m_fDistToPlayer >= m_fSlashRangeMax && m_fDistToPlayer < m_fSlamRangeMax && !m_bIsAnimating && m_iPrevAttackType != 1)
        {
            m_sAnimations.m_pASprGolemJump->Animate();
            m_iPrevAttackType = 1;
            m_bSlam = true;
            m_bJumping = true;
        }
        else if (m_fDistToPlayer >= 0.0f && m_fDistToPlayer < m_fSlashRangeMax && !m_bIsAnimating && m_iPrevAttackType != 0)
        {
            m_sAnimations.m_pASprGolemSlash->Animate();
            m_iPrevAttackType = 0;
            m_bSlash = true;
        }
    }
}

void
Golem::ProcessAction()
{
    if (m_bSlash)
    {
        if (m_fDistToPlayer >= 0.0f && m_fDistToPlayer < m_fSlashRangeMax)
        {
            // Apply slash damage
        }
    }

    if (m_bSlam)
    {
        if (m_sAnimations.m_pASprGolemSlam->IsAnimating())
        {
            if (m_fDistToPlayer >= m_fSlashRangeMax && m_fDistToPlayer < m_fSlamRangeMax)
            {
                // Apply damage
            }
        }
    }

    if (m_bProjectile)
    {
        m_velocityDirection.x = m_vProjectileEndPos.x - m_vProjectileStartPos.x;
        m_velocityDirection.y = m_vProjectileEndPos.y - m_vProjectileStartPos.y;

        float length = m_velocityDirection.Length();

        if (length > 0.0f)
        {
            m_velocityDirection.x /= length;
            m_velocityDirection.y /= length;
        }

        m_velocityProjectile.x = m_velocityDirection.x * 175.0f;
        m_velocityProjectile.y = m_velocityDirection.y * 175.0f;
    }
}

void
Golem::SetTerrainMoving(bool moving)
{
    sm_bTerrainMoving = moving;
}

bool
Golem::IsTerrainMoving()
{
    return sm_bTerrainMoving;
}

Vector2&
Golem::GetPosition()
{
    return m_vStandingPos;
}

void
Golem::SetCharacter(Character& character)
{
    m_pEntCharacter = &character;
}

void
Golem::CheckPlayerDist()
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
    else
    {
        m_bPlayerInRange = false;
        m_bSpotted = false;
    }
}

void
Golem::IsAnimating()
{
    if (!m_sAnimations.m_pASprGolemWalk->IsAnimating())
    {
        m_bIsAnimating = false;
    }
    
    if (!m_sAnimations.m_pASprGolemSlash->IsAnimating())
    {
        m_bIsAnimating = false;
    }

    if (!m_sAnimations.m_pASprGolemJump->IsAnimating())
    {
        m_bIsAnimating = false;
    }

    if (!m_sAnimations.m_pASprGolemSlam->IsAnimating())
    {
        m_bIsAnimating = false;
    }

    if (!m_sAnimations.m_pASprGolemThrow->IsAnimating())
    {
        m_bIsAnimating = false;
    }
}

int
Golem::GetBodyWidth()
{
    return m_pSprSpriteBody->GetWidth();
}

void
Golem::ShiftX(float amount)
{
    m_vPosition.x += amount;
    m_vFeetPos.x += amount;
    m_vStandingPos.x += amount;
}

void
Golem::SetNumSegments(int amount)
{
    m_iNumSegments = amount;
}

void
Golem::SetNumWalkableSegs(int amount)
{
    m_iNumWalkableSegs = amount;
}

void
Golem::ComputeBounds(float width, float height)
{
    // Set boundaries
    m_vBoundaryLow.x = (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryLow.y = ((sm_fBoundaryHeight / 7.0f) * 4.0f);

    m_vBoundaryHigh.x = width - (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryHigh.y = height;
}

//void
//Golem::DebugDraw()
//{
//
//}
