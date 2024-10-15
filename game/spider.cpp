// This include:
#include "spider.h"

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

Spider::Spider()
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

Spider::~Spider()
{
    delete m_pSprSpriteBody;
    m_pSprSpriteBody = 0;

    delete m_sAnimations.m_pASprSpdrSleep;
    m_sAnimations.m_pASprSpdrSleep = 0;

    delete m_sAnimations.m_pASprSpdrWalk;
    m_sAnimations.m_pASprSpdrWalk = 0;

    delete m_sAnimations.m_pASprSpdrAttack;
    m_sAnimations.m_pASprSpdrAttack = 0;
}

bool
Spider::Initialise(Renderer& renderer)
{
    if (!SetBodySprites(renderer))
    {
        LogManager::GetInstance().Log("Spider Sprites failed to initialise!");
        return false;
    }

    m_fScale = 1.0f;
    m_fScaleChangeRate = 0.125f;
    m_fScaleMin = 0.92f;
    m_fScaleMax = 1.08f;
    m_fAnimateScale = 0.435f;
    m_fHitBoxRange = 50.0f;
    m_fAttackRangeMax = (m_sAnimations.m_pASprSpdrAttack->GetWidth() / 2.0f);
    m_fGroundY = ((sm_fBoundaryHeight / 7.0f) * 5.0f);
    m_bAlive = true;

    m_vPosition.x = sm_fSegmentWidth * 4;
    m_vPosition.y = m_fGroundY - 85.0f;

    return true;
}

void
Spider::Process(float deltaTime, InputSystem& inputSystem)
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

            if (!m_sAnimations.m_pASprSpdrWalk->IsAnimating())
            {
                m_sAnimations.m_pASprSpdrWalk->Animate();
                m_sAnimations.m_pASprSpdrWalk->SetLooping(true);
            }
        }
        else
        {
            m_sAnimations.m_pASprSpdrWalk->Inanimate();
            m_sAnimations.m_pASprSpdrWalk->SetLooping(false);
        }
    }
    else
    {
        m_fExecutionTime = 0.0f;
        m_sAnimations.m_pASprSpdrSleep->Animate();
        m_sAnimations.m_pASprSpdrSleep->SetLooping(true);
    }

    ProcessAction();

    m_vPosition += (m_velocityPos + m_velocityBody) * deltaTime;

    m_sAnimations.m_pASprSpdrSleep->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprSpdrSleep->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprSpdrWalk->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprSpdrWalk->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprSpdrAttack->SetX((int)m_vPosition.x);
    m_sAnimations.m_pASprSpdrAttack->SetY((int)m_vPosition.y);

    m_sAnimations.m_pASprSpdrSleep->Process(deltaTime);
    m_sAnimations.m_pASprSpdrWalk->Process(deltaTime);
    m_sAnimations.m_pASprSpdrAttack->Process(deltaTime);
}

void
Spider::Draw(Renderer& renderer)
{
    if (m_bAlive)
    {
        if (m_bPlayerInRange)
        {
            if (m_bRun)
            {
                m_sAnimations.m_pASprSpdrWalk->Draw(renderer, m_bFlipHorizontally, false);
            }
            else if (m_bAttack)
            {
                if (m_sAnimations.m_pASprSpdrAttack->IsAnimating())
                {
                    m_sAnimations.m_pASprSpdrAttack->Draw(renderer, m_bFlipHorizontally, false);
                }
                else
                {
                    m_bAttack = false;
                    m_bIsAnimating = false;
                }
            }
            else
            {
                m_sAnimations.m_pASprSpdrWalk->Draw(renderer, m_bFlipHorizontally, false);
            }
        }
        else
        {
            m_sAnimations.m_pASprSpdrSleep->Draw(renderer, m_bFlipHorizontally, false);
        }
    }
}

bool
Spider::SetBodySprites(Renderer& renderer)
{
    m_sAnimations.m_pASprSpdrSleep = renderer.CreateAnimatedSprite("..\\Sprites\\spider\\anim8spdrsleep.png");

    if (!(m_sAnimations.m_pASprSpdrSleep))
    {
        LogManager::GetInstance().Log("Spider sleep animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSpdrSleep->SetupFrames(56, 34);
        m_sAnimations.m_pASprSpdrSleep->SetFrameDuration(0.1f);
        m_sAnimations.m_pASprSpdrSleep->SetScale(2.5f);
    }

    m_sAnimations.m_pASprSpdrAttack = renderer.CreateAnimatedSprite("..\\Sprites\\spider\\anim8spdrattack.png");

    if (!(m_sAnimations.m_pASprSpdrAttack))
    {
        LogManager::GetInstance().Log("Spider attack animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSpdrAttack->SetupFrames(56, 34);
        m_sAnimations.m_pASprSpdrAttack->SetFrameDuration(0.15f);
        m_sAnimations.m_pASprSpdrAttack->SetScale(2.5f);
    }

    m_sAnimations.m_pASprSpdrWalk = renderer.CreateAnimatedSprite("..\\Sprites\\spider\\anim8spdrwalk.png");

    if (!(m_sAnimations.m_pASprSpdrWalk))
    {
        LogManager::GetInstance().Log("Skeleton run animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSpdrWalk->SetupFrames(56, 34);
        m_sAnimations.m_pASprSpdrWalk->SetFrameDuration(0.07f);
        m_sAnimations.m_pASprSpdrWalk->SetScale(2.5f);
    }

    return true;
}

void
Spider::Move(void)
{
    if (m_bPlayerInRange)
    {
        if (m_pEntCharacter->GetPosition().x < (m_vPosition.x - m_fAttackRangeMax) &&
            m_pEntCharacter->GetPosition().x < m_vPosition.x)
        {
            m_iFacingDirection = 1;
            m_bFlipHorizontally = false;
            m_bIsAnimating = true;
            m_velocityBody.x = -250.0f;
        }
        else if (m_pEntCharacter->GetPosition().x > (m_vPosition.x + m_fAttackRangeMax) &&
            m_pEntCharacter->GetPosition().x > m_vPosition.x)
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_bIsAnimating = true;
            m_velocityBody.x = 250.0f;
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
Spider::Action()
{
    if (m_fDistToPlayer < m_fAttackRangeMax && m_fDistToPlayer >= 0.0f)
    {
        if (!m_bRun)
        {
            m_sAnimations.m_pASprSpdrWalk->SetLooping(false);
            m_sAnimations.m_pASprSpdrWalk->Inanimate();
        }

        if (m_sAnimations.m_pASprSpdrSleep->IsAnimating())
        {
            m_sAnimations.m_pASprSpdrSleep->Inanimate();
            m_sAnimations.m_pASprSpdrSleep->SetLooping(false);
        }

        if (!m_sAnimations.m_pASprSpdrAttack->IsAnimating() && !m_bAttack)
        {
            m_sAnimations.m_pASprSpdrAttack->Animate();
            m_bAttack = true;
        }
    }
    else
    {
        m_bRun = true;
    }
}

void
Spider::ProcessAction()
{

}

Vector2&
Spider::GetPosition()
{
    return m_vStandingPos;
}

void
Spider::SetCharacter(Character& character)
{
    m_pEntCharacter = &character;
}

void
Spider::CheckPlayerDist()
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
        m_sAnimations.m_pASprSpdrSleep->Animate();
        m_sAnimations.m_pASprSpdrSleep->SetLooping(true);
    }
}

int
Spider::GetBodyWidth()
{
    return (int)(m_fHitBoxRange * 2);
}

void
Spider::ShiftX(float amount)
{
    m_vPosition.x += amount;
    m_vStandingPos.x += amount;
}

void
Spider::SetNumWalkableSegs(int amount)
{
    m_iNumWalkableSegs = amount;
}

void
Spider::ComputeBounds(float width, float height)
{
    // Set boundaries
    m_vBoundaryLow.x = (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryLow.y = ((sm_fBoundaryHeight / 7.0f) * 4.0f);

    m_vBoundaryHigh.x = width - (m_pSprSpriteBody->GetWidth() / 2.0f);
    m_vBoundaryHigh.y = height;
}

//void
//Spider::DebugDraw()
//{
//
//}