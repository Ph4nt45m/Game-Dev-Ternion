// This include:
#include "golem.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "../imgui/imgui.h"
#include "character.h"
#include "projectile.h"

// Library includes:
#include <cassert>
#include <cstdio>

Golem::Golem()
    : m_pEntCharacter(0)
    , m_pSprSpriteBody(0)
    , m_pEntProjectile(0)
    , m_sAnimations{ 0, 0, 0, 0, 0 }
    , m_iNumSegments(0)
    , m_iNumWalkableSegs(0)
    , m_fAnimateScale(0.0f)
    , m_fExecutionTime(0.0f)
    , m_iAttackType(0)
    , m_fDistToPlayer(0.0f)
    , m_fHitBoxRange(0.0f)
    , m_fSlashRangeMax(0.0f)
    , m_fSlamRangeMax(0.0f)
    , m_fThrowRangeMax(0.0f)
    , m_fGroundY(0.0f)
    , m_bPlayerInRange(false)
    , m_bSpotted(false)
    , m_bEngage(false)
    , m_bIsAnimating(false)
    , m_bSlam(false)
    , m_bWalk(false)
{

}

Golem::~Golem()
{
    delete m_pSprSpriteBody;
    m_pSprSpriteBody = 0;

    delete m_pEntProjectile;
    m_pEntProjectile = 0;

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
    m_fGroundY = ((sm_fBoundaryHeight / 7.0f) * 5.0f);
    m_iAttackType = 2;
    m_bAlive = true;

    m_vPosition.x = sm_fSegmentWidth * (sm_iNumSegments - 3);
    m_vPosition.y = m_fGroundY - 85.0f;
    m_vStartingPos.x = sm_fSegmentWidth * (sm_iNumSegments - 3);
    m_vStartingPos.y = m_fGroundY - 85.0f;

    m_pEntProjectile = new Projectile();

    if (!m_pEntProjectile->Initialise(renderer))
    {
        LogManager::GetInstance().Log("Golem Projectile failed to initialise!");
        return false;
    }
    else
    {
        m_pEntProjectile->SetProjectileSprite(renderer, "..\\Sprites\\golem\\ball.png");
        m_pEntProjectile->SetGroundY(m_fGroundY + 32);
        m_pEntProjectile->SetTimeToTarget(1.5f);
    }

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

        if (!m_bWalk && (((int)m_fExecutionTime % 5) == 0))
        {
            Action();
        }
        else
        {
            if (m_bWalk)
            {
                Move(m_iAttackType);
                
                if (!m_sAnimations.m_pASprGolemWalk->IsAnimating())
                {
                    m_sAnimations.m_pASprGolemWalk->Animate();
                    m_sAnimations.m_pASprGolemWalk->SetLooping(true);
                }
            }
            else
            {
                m_sAnimations.m_pASprGolemWalk->Inanimate();
                m_sAnimations.m_pASprGolemWalk->SetLooping(false);
            }
        }
    }
    else
    {
        m_fExecutionTime = 0.0f;
    }

    ProcessAction();

    m_vPosition += (m_velocityPos + m_velocityBody) * deltaTime;

    m_pSprSpriteBody->SetX((int)m_vPosition.x);
    m_pSprSpriteBody->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprGolemWalk->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprGolemWalk->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprGolemJump->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprGolemJump->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprGolemSlam->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprGolemSlam->SetY((int)(m_fGroundY + 32));

    m_sAnimations.m_pASprGolemSlash->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprGolemSlash->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprGolemThrow->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprGolemThrow->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprGolemWalk->Process(deltaTime);
    m_sAnimations.m_pASprGolemJump->Process(deltaTime);
    m_sAnimations.m_pASprGolemSlam->Process(deltaTime);
    m_sAnimations.m_pASprGolemSlash->Process(deltaTime);
    m_sAnimations.m_pASprGolemThrow->Process(deltaTime);
    m_pEntProjectile->Process(deltaTime, inputSystem);
}

void
Golem::Draw(Renderer& renderer)
{
    if (m_bAlive)
    {
        if (m_bPlayerInRange)
        {
            if (m_bWalk)
            {
                m_sAnimations.m_pASprGolemWalk->Draw(renderer, m_bFlipHorizontally, false);
            }
            else if (m_bSlash)
            {
                if (m_sAnimations.m_pASprGolemSlash->IsAnimating())
                {
                    printf("Slash\n");
                    m_sAnimations.m_pASprGolemSlash->Draw(renderer, m_bFlipHorizontally, false);
                }
                else
                {
                    m_bSlash = false;
                    m_bIsAnimating = false;

                    if (m_fDistToPlayer < m_fSlamRangeMax)
                    {
                        m_iAttackType = 1;
                    }
                    else if (m_fDistToPlayer >= m_fSlamRangeMax)
                    {
                        m_iAttackType = 2;
                    }
                }
            }
            else if (m_bSlam)
            {
                if (m_sAnimations.m_pASprGolemJump->IsAnimating())
                {
                    printf("Jump\n");
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
                        printf("Slam\n");
                        m_sAnimations.m_pASprGolemSlam->Draw(renderer, m_bFlipHorizontally, false);
                        m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, true);
                    }
                    else
                    {
                        m_bSlam = false;
                        m_bIsAnimating = false;
                        
                        if (m_fDistToPlayer < m_fSlashRangeMax)
                        {
                            m_iAttackType = 0;
                        }
                        else if (m_fDistToPlayer >= m_fSlamRangeMax)
                        {
                            m_iAttackType = 2;
                        }
                    }
                }
            }
            else if (m_bProjectile)
            {
                if (m_sAnimations.m_pASprGolemThrow->IsAnimating())
                {
                    printf("Throw\n");
                    m_sAnimations.m_pASprGolemThrow->Draw(renderer, m_bFlipHorizontally, false);
                }
                else
                {
                    printf("Projectile\n");
                    m_pEntProjectile->SetStartPos(m_vPosition.x + ((m_bFlipHorizontally ? -m_fHitBoxRange : m_fHitBoxRange)), m_vPosition.y);
                    m_pEntProjectile->SetTargetPos(m_pEntCharacter->GetPosition().x, m_pEntCharacter->GetPosition().y);
                    m_pEntProjectile->Shoot();
                    m_bShoot = false;
                    m_bProjectile = false;
                    m_bIsAnimating = false;
                    m_iAttackType = 1;
                    
                    /*if (m_fDistToPlayer < m_fSlashRangeMax)
                    {
                        m_iAttackType = 0;
                    }
                    else if (m_fDistToPlayer < m_fSlamRangeMax && m_fDistToPlayer >= m_fSlashRangeMax)
                    {
                        m_iAttackType = 1;
                    }*/
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

        if (!m_bShoot && m_bProjectile && m_iAttackType == 2)
        {
            m_iAttackType = 1;
        }

        m_pEntProjectile->Draw(renderer);
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
        m_sAnimations.m_pASprGolemJump->SetFrameDuration(0.1f);
    }

    m_sAnimations.m_pASprGolemSlam = renderer.CreateAnimatedSprite("..\\Sprites\\golem\\anim8golemslam.png");

    if (!(m_sAnimations.m_pASprGolemSlam))
    {
        LogManager::GetInstance().Log("Golem Slam animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprGolemSlam->SetupFrames(708, 159);
        m_sAnimations.m_pASprGolemSlam->SetFrameDuration(0.15f);
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
Golem::Move(int attackType)
{
    if (m_bPlayerInRange)
    {
        switch (attackType)
        {
        case 0:
            if (m_pEntCharacter->GetPosition().x < (m_vPosition.x - m_fSlashRangeMax) &&
                m_pEntCharacter->GetPosition().x > m_vPosition.x)
            {
                printf("Moving Left Slash\n");
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_bIsAnimating = true;
                m_velocityBody.x = -150.0f;
            }
            else if (m_pEntCharacter->GetPosition().x > (m_vPosition.x + m_fSlashRangeMax) &&
                m_pEntCharacter->GetPosition().x < m_vPosition.x)
            {
                printf("Moving Right Slash\n");
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
                m_bIsAnimating = true;
                m_velocityBody.x = 150.0f;
            }
            else
            {
                m_velocityBody.x = 0.0f;
                m_bIsAnimating = false;
                m_bWalk = false;
            }
            break;
        case 1:
            if (m_pEntCharacter->GetPosition().x < (m_vPosition.x - m_fSlamRangeMax) &&
                m_pEntCharacter->GetPosition().x < m_vPosition.x)
            {
                printf("Moving Left Slam\n");
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_bIsAnimating = true;
                m_velocityBody.x = -150.0f;
            }
            else if (m_pEntCharacter->GetPosition().x > (m_vPosition.x + m_fSlamRangeMax) &&
                m_pEntCharacter->GetPosition().x < m_vPosition.x)
            {
                printf("Moving Right Slam\n");
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
                m_bIsAnimating = true;
                m_velocityBody.x = 150.0f;
            }
            else
            {
                m_velocityBody.x = 0.0f;
                m_bIsAnimating = false;
                m_bWalk = false;
            }
            break;
        case 2:
            if (m_pEntCharacter->GetPosition().x < (m_vPosition.x - m_fThrowRangeMax) && m_pEntCharacter->GetPosition().x < m_vPosition.x)
            {
                printf("Moving Left Throw\n");
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_bIsAnimating = true;
                m_velocityBody.x = -150.0f;
            }
            else if (m_pEntCharacter->GetPosition().x > (m_vPosition.x + m_fThrowRangeMax) && m_pEntCharacter->GetPosition().x > m_vPosition.x)
            {
                printf("Moving Right Throw\n");
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
                m_bIsAnimating = true;
                m_velocityBody.x = 150.0f;
            }
            else
            {
                m_velocityBody.x = 0.0f;
                m_bIsAnimating = false;
                m_bWalk = false;
            }
            break;
        case 3:
            if ((int)m_vPosition.x < (int)m_vStartingPos.x)
            {
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
                m_bIsAnimating = true;
                m_velocityBody.x = 150.0f;
            }
            else if ((int)m_vPosition.x > (int)m_vStartingPos.x)
            {
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_bIsAnimating = true;
                m_velocityBody.x = -150.0f;
            }
            else
            {
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_velocityBody.x = 0.0f;
            }
            break;
        }
    }
    else
    {
        if ((int)m_vPosition.x < (int)m_vStartingPos.x)
        {
            m_iFacingDirection = 1;
            m_bFlipHorizontally = false;
            m_bIsAnimating = true;
            m_bWalk = true;
            m_velocityBody.x = 150.0f;
        }
        else if ((int)m_vPosition.x > (int)m_vStartingPos.x)
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_bIsAnimating = true;
            m_bWalk = true;
            m_velocityBody.x = -150.0f;
        }
        else
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_bIsAnimating = false;
            m_bWalk = false;
            m_velocityBody.x = 0.0f;
        }
    }
}

void
Golem::Action()
{
    if (m_iAttackType == 2)
    {
        if (m_fDistToPlayer <= m_fThrowRangeMax && m_fDistToPlayer >= m_fSlamRangeMax && !m_bIsAnimating)
        {
            if (!m_bWalk)
            {
                m_sAnimations.m_pASprGolemWalk->SetLooping(false);
                m_sAnimations.m_pASprGolemWalk->Inanimate();
            }

            if (!m_sAnimations.m_pASprGolemThrow->IsAnimating() && !m_bProjectile)
            {
                m_sAnimations.m_pASprGolemThrow->Animate();
                m_bShoot = true;
                m_bProjectile = true;
                m_bIsAnimating = true;
            }
        }
        else
        {
            m_bWalk = true;
        }
    }
    else if (m_iAttackType == 1)
    {
        if (m_fDistToPlayer < m_fSlamRangeMax && m_fDistToPlayer >= 0.0f && !m_bIsAnimating)
        {
            if (!m_bWalk)
            {
                m_sAnimations.m_pASprGolemWalk->SetLooping(false);
                m_sAnimations.m_pASprGolemWalk->Inanimate();
            }

            if (!m_sAnimations.m_pASprGolemJump->IsAnimating() && !m_bSlam)
            {
                m_sAnimations.m_pASprGolemJump->Animate();
                m_bSlam = true;
                m_bJumping = true;
                m_bIsAnimating = true;
            }
        }
        else
        {
            m_bWalk = true;
        }
    }
    else if (m_iAttackType == 0)
    {
        if (m_fDistToPlayer < m_fSlashRangeMax && m_fDistToPlayer >= 0.0f && !m_bIsAnimating)
        {
            if (!m_bWalk)
            {
                m_sAnimations.m_pASprGolemWalk->SetLooping(false);
                m_sAnimations.m_pASprGolemWalk->Inanimate();
            }

            if (!m_sAnimations.m_pASprGolemSlash->IsAnimating() && !m_bSlash)
            {
                m_sAnimations.m_pASprGolemSlash->Animate();
                m_iAttackType = 0;
                m_bSlash = true;
                m_bIsAnimating = true;
            }
        }
        else
        {
            m_bWalk = true;
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
        /*m_velocityDirection.x = m_vProjectileEndPos.x - m_vProjectileStartPos.x;
        m_velocityDirection.y = m_vProjectileEndPos.y - m_vProjectileStartPos.y;

        float length = m_velocityDirection.Length();

        if (length > 0.0f)
        {
            m_velocityDirection.x /= length;
            m_velocityDirection.y /= length;
        }

        m_velocityProjectile.x = m_velocityDirection.x * 175.0f;
        m_velocityProjectile.y = m_velocityDirection.y * 175.0f;*/
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
    else if (m_fDistToPlayer > (sm_fSegmentWidth * 2.5f) && m_bPlayerInRange)
    {
        m_bPlayerInRange = false;
        m_bSpotted = false;

        if ((int)m_vPosition.x != (int)m_vStartingPos.x)
        {
            m_bWalk = true;
            Move(3);
        }
    }
}

bool
Golem::IsAnimating()
{
    if (m_sAnimations.m_pASprGolemWalk->IsAnimating())
    {
        return true;
    }
    
    if (m_sAnimations.m_pASprGolemSlash->IsAnimating())
    {
        return true;
    }

    if (m_sAnimations.m_pASprGolemJump->IsAnimating())
    {
        return true;
    }

    if (m_sAnimations.m_pASprGolemSlam->IsAnimating())
    {
        return true;
    }

    if (m_sAnimations.m_pASprGolemThrow->IsAnimating())
    {
        return true;
    }

    return false;
}

int
Golem::GetBodyWidth()
{
    return (int)(m_fHitBoxRange * 2);
}

void
Golem::ShiftX(float amount)
{
    m_vPosition.x += amount;
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
