//// This include:
//#include "skeleton.h"
//
//// Local includes:
//#include "renderer.h"
//#include "logmanager.h"
//#include "sprite.h"
//#include "animatedsprite.h"
//#include "../imgui/imgui.h"
//#include "character.h"
//
//// Library includes:
//#include <cassert>
//#include <cstdio>
//
//Skeleton::Skeleton()
//    : m_pEntCharacter(0)
//    , m_pSprSpriteBody(0)
//    , m_sAnimations{ 0, 0, 0 }
//    , m_iNumSegments(0)
//    , m_iNumWalkableSegs(0)
//    , m_fAnimateScale(0.0f)
//    , m_fExecutionTime(0.0f)
//    , m_fDistToPlayer(0.0f)
//    , m_fHitBoxRange(0.0f)
//    , m_fAttackRangeMax(0.0f)
//    , m_fGroundY(0.0f)
//    , m_bPlayerInRange(false)
//    , m_bSpotted(false)
//    , m_bEngage(false)
//    , m_bIsAnimating(false)
//    , m_bAttack(false)
//    , m_bRun(false)
//{
//
//}
//
//Skeleton::~Skeleton()
//{
//    delete m_pSprSpriteBody;
//    m_pSprSpriteBody = 0;
//
//    delete m_sAnimations.m_pASprSkelIdle;
//    m_sAnimations.m_pASprSkelIdle = 0;
//
//    delete m_sAnimations.m_pASprSkelWalk;
//    m_sAnimations.m_pASprSkelWalk = 0;
//
//    delete m_sAnimations.m_pASprSkelAttack;
//    m_sAnimations.m_pASprSkelAttack = 0;
//}
//
//bool
//Skeleton::Initialise(Renderer& renderer)
//{
//    if (!SetBodySprites(renderer))
//    {
//        LogManager::GetInstance().Log("Skeleton Sprites failed to initialise!");
//        return false;
//    }
//
//    m_iFacingDirection = -1;
//    m_bFlipHorizontally = true;
//    m_fScale = 1.0f;
//    m_fScaleChangeRate = 0.125f;
//    m_fScaleMin = 0.92f;
//    m_fScaleMax = 1.08f;
//    m_fAnimateScale = 0.435f;
//    m_fHitBoxRange = 50.0f;
//    m_fAttackRangeMax = (m_sAnimations.m_pASprSkelAttack->GetWidth() / 2.0f);
//    m_fGroundY = ((sm_fBoundaryHeight / 7.0f) * 5.0f);
//    m_bAlive = true;
//
//    m_vPosition.x = sm_fSegmentWidth * 4;
//    m_vPosition.y = m_fGroundY - 85.0f;
//
//    return true;
//}
//
//void
//Skeleton::Process(float deltaTime, InputSystem& inputSystem)
//{
//    GetInputs(inputSystem);
//    HandleInput(deltaTime);
//    CheckPlayerDist();
//
//    if (m_bPlayerInRange)
//    {
//        m_fExecutionTime += deltaTime;
//
//        if (!m_bRun && (((int)m_fExecutionTime % 5) == 0))
//        {
//            Action();
//        }
//
//        if (m_bRun)
//        {
//            Move();
//
//            if (!m_sAnimations.m_pASprSkelWalk->IsAnimating())
//            {
//                m_sAnimations.m_pASprSkelWalk->Animate();
//                m_sAnimations.m_pASprSkelWalk->SetLooping(true);
//            }
//        }
//        else
//        {
//            m_sAnimations.m_pASprSkelWalk->Inanimate();
//            m_sAnimations.m_pASprSkelWalk->SetLooping(false);
//            m_sAnimations.m_pASprSkelIdle->Animate();
//            m_sAnimations.m_pASprSkelIdle->SetLooping(true);
//        }
//    }
//    else
//    {
//        m_fExecutionTime = 0.0f;
//        m_sAnimations.m_pASprSkelIdle->Animate();
//        m_sAnimations.m_pASprSkelIdle->SetLooping(true);
//    }
//
//    ProcessAction();
//
//    m_vPosition += (m_velocityPos + m_velocityBody) * deltaTime;
//
//    m_sAnimations.m_pASprSkelIdle->SetX((int)m_vPosition.x);
//    m_sAnimations.m_pASprSkelIdle->SetY((int)m_vPosition.y);
//
//    m_sAnimations.m_pASprSkelWalk->SetX((int)m_vPosition.x);
//    m_sAnimations.m_pASprSkelWalk->SetY((int)m_vPosition.y);
//
//    m_sAnimations.m_pASprSkelAttack->SetX((int)m_vPosition.x);
//    m_sAnimations.m_pASprSkelAttack->SetY((int)m_vPosition.y);
//
//    m_sAnimations.m_pASprSkelIdle->Process(deltaTime);
//    m_sAnimations.m_pASprSkelWalk->Process(deltaTime);
//    m_sAnimations.m_pASprSkelAttack->Process(deltaTime);
//}
//
//void
//Skeleton::Draw(Renderer& renderer)
//{
//    if (m_bAlive)
//    {
//        if (m_bPlayerInRange)
//        {
//            if (m_bRun)
//            {
//                m_sAnimations.m_pASprSkelWalk->Draw(renderer, m_bFlipHorizontally, false);
//            }
//            else if (m_bAttack)
//            {
//                if (m_sAnimations.m_pASprSkelAttack->IsAnimating())
//                {
//                    m_sAnimations.m_pASprSkelAttack->Draw(renderer, m_bFlipHorizontally, false);
//                }
//                else
//                {
//                    m_bAttack = false;
//                    m_bIsAnimating = false;
//                }
//            }
//            else
//            {
//                m_sAnimations.m_pASprSkelIdle->Draw(renderer, m_bFlipHorizontally, false);
//            }
//        }
//        else
//        {
//            m_sAnimations.m_pASprSkelIdle->Draw(renderer, m_bFlipHorizontally, false);
//        }
//    }
//}
//
//void
//Skeleton::GetInputs(InputSystem& inputSystem)
//{
//    // Gets movement keys' states
//    m_sMotionKeyStates.MoveForward = inputSystem.GetKeyState(SDL_SCANCODE_D);
//    m_sMotionKeyStates.MoveBackward = inputSystem.GetKeyState(SDL_SCANCODE_A);
//
//    if (m_sMotionKeyStates.MoveForward == BS_PRESSED || m_sMotionKeyStates.MoveForward == BS_HELD)
//    {
//        m_sKeyboardMotions.Surge = MOTION_FORWARD;
//    }
//    else if (m_sMotionKeyStates.MoveBackward == BS_PRESSED || m_sMotionKeyStates.MoveBackward == BS_HELD)
//    {
//        m_sKeyboardMotions.Surge = MOTION_BACKWARD;
//    }
//    else
//    {
//        m_sKeyboardMotions.Surge = MOTION_NONE;
//    }
//}
//
//void
//Skeleton::HandleInput(float deltaTime)
//{
//    // Handles left and right shift whilst player is out of range
//    if (m_sKeyboardMotions.Surge > 0)
//    {
//        if (sm_bCameraCentered && sm_bTerrainMoving)
//        {
//            m_velocityPos.x = -200.0f;
//        }
//        else
//        {
//            m_velocityPos.x = 0.0f;
//        }
//
//        m_bMovingX = true;
//    }
//    else if (m_sKeyboardMotions.Surge < 0)
//    {
//        if (sm_bCameraCentered && sm_bTerrainMoving)
//        {
//            m_velocityPos.x = 200.0f;
//        }
//        else
//        {
//            m_velocityPos.x = 0.0f;
//        }
//
//        m_bMovingX = true;
//    }
//    else if (m_sKeyboardMotions.Surge == 0)
//    {
//        m_velocityPos.x = 0.0f;
//
//        if (!m_bJumping)
//        {
//            m_bMovingX = false;
//        }
//    }
//}
//
//bool
//Skeleton::SetBodySprites(Renderer& renderer)
//{
//    m_sAnimations.m_pASprSkelIdle = renderer.CreateAnimatedSprite("..\\Sprites\\skeleton\\anim8skelidle.png");
//
//    if (!(m_sAnimations.m_pASprSkelIdle))
//    {
//        LogManager::GetInstance().Log("Skeleton idle animation failed to initialise!");
//        return false;
//    }
//    else
//    {
//        m_sAnimations.m_pASprSkelIdle->SetupFrames(96, 64);
//        m_sAnimations.m_pASprSkelIdle->SetFrameDuration(0.07f);
//        m_sAnimations.m_pASprSkelIdle->SetScale(2.5f);
//    }
//
//    m_sAnimations.m_pASprSkelAttack = renderer.CreateAnimatedSprite("..\\Sprites\\skeleton\\anim8skelattack.png");
//
//    if (!(m_sAnimations.m_pASprSkelAttack))
//    {
//        LogManager::GetInstance().Log("Skeleton attack animation failed to initialise!");
//        return false;
//    }
//    else
//    {
//        m_sAnimations.m_pASprSkelAttack->SetupFrames(96, 64);
//        m_sAnimations.m_pASprSkelAttack->SetFrameDuration(0.15f);
//        m_sAnimations.m_pASprSkelAttack->SetScale(2.5f);
//    }
//
//    m_sAnimations.m_pASprSkelWalk = renderer.CreateAnimatedSprite("..\\Sprites\\skeleton\\anim8skelwalk.png");
//
//    if (!(m_sAnimations.m_pASprSkelWalk))
//    {
//        LogManager::GetInstance().Log("Skeleton run animation failed to initialise!");
//        return false;
//    }
//    else
//    {
//        m_sAnimations.m_pASprSkelWalk->SetupFrames(96, 64);
//        m_sAnimations.m_pASprSkelWalk->SetFrameDuration(0.1f);
//        m_sAnimations.m_pASprSkelWalk->SetScale(2.5f);
//    }
//
//    return true;
//}
//
//void
//Skeleton::Move(void)
//{
//    if (m_bPlayerInRange)
//    {
//        if (m_pEntCharacter->GetPosition().x < (m_vPosition.x - m_fAttackRangeMax) &&
//            m_pEntCharacter->GetPosition().x < m_vPosition.x)
//        {
//            m_iFacingDirection = -1;
//            m_bFlipHorizontally = true;
//            m_bIsAnimating = true;
//            m_velocityBody.x = -150.0f;
//        }
//        else if (m_pEntCharacter->GetPosition().x > (m_vPosition.x + m_fAttackRangeMax) &&
//            m_pEntCharacter->GetPosition().x > m_vPosition.x)
//        {
//            m_iFacingDirection = 1;
//            m_bFlipHorizontally = false;
//            m_bIsAnimating = true;
//            m_velocityBody.x = 150.0f;
//        }
//        else
//        {
//            m_velocityBody.x = 0.0f;
//            m_bIsAnimating = false;
//            m_bRun = false;
//            m_fExecutionTime = 0.0f;
//        }
//    }
//}
//
//void
//Skeleton::Action()
//{
//    if (m_fDistToPlayer < m_fAttackRangeMax && m_fDistToPlayer >= 0.0f)
//    {
//        if (!m_bRun)
//        {
//            m_sAnimations.m_pASprSkelWalk->SetLooping(false);
//            m_sAnimations.m_pASprSkelWalk->Inanimate();
//        }
//
//        if (m_sAnimations.m_pASprSkelIdle->IsAnimating())
//        {
//            m_sAnimations.m_pASprSkelIdle->Inanimate();
//            m_sAnimations.m_pASprSkelIdle->SetLooping(false);
//        }
//
//        if (!m_sAnimations.m_pASprSkelAttack->IsAnimating() && !m_bAttack)
//        {
//            m_sAnimations.m_pASprSkelAttack->Animate();
//            m_bAttack = true;
//        }
//    }
//    else
//    {
//        m_bRun = true;
//    }
//}
//
//void
//Skeleton::ProcessAction()
//{
//
//}
//
//void
//Skeleton::SetTerrainMoving(bool moving)
//{
//    sm_bTerrainMoving = moving;
//}
//
//bool
//Skeleton::IsTerrainMoving()
//{
//    return sm_bTerrainMoving;
//}
//
//Vector2&
//Skeleton::GetPosition()
//{
//    return m_vStandingPos;
//}
//
//void
//Skeleton::SetCharacter(Character& character)
//{
//    m_pEntCharacter = &character;
//}
//
//void
//Skeleton::CheckPlayerDist()
//{
//    m_fDistToPlayer = m_vPosition.x - m_pEntCharacter->GetPosition().x;
//
//    if (m_fDistToPlayer < 0)
//    {
//        m_fDistToPlayer *= -1.0f;
//    }
//
//    if (m_fDistToPlayer <= sm_fSegmentWidth * (m_bSpotted ? 2.5f : 1.6f))
//    {
//        m_bPlayerInRange = true;
//
//        if (!m_bSpotted)
//        {
//            m_bSpotted = true;
//        }
//    }
//    else if (m_fDistToPlayer > (sm_fSegmentWidth * 2.5f) && m_bPlayerInRange)
//    {
//        m_bPlayerInRange = false;
//        m_bSpotted = false;
//        m_sAnimations.m_pASprSkelIdle->Animate();
//        m_sAnimations.m_pASprSkelIdle->SetLooping(true);
//    }
//}
//
//int
//Skeleton::GetBodyWidth()
//{
//    return (int)(m_fHitBoxRange * 2);
//}
//
//void
//Skeleton::ShiftX(float amount)
//{
//    m_vPosition.x += amount;
//    m_vStandingPos.x += amount;
//}
//
//void
//Skeleton::SetNumSegments(int amount)
//{
//    m_iNumSegments = amount;
//}
//
//void
//Skeleton::SetNumWalkableSegs(int amount)
//{
//    m_iNumWalkableSegs = amount;
//}
//
//void
//Skeleton::ComputeBounds(float width, float height)
//{
//    // Set boundaries
//    m_vBoundaryLow.x = (m_pSprSpriteBody->GetWidth() / 2.0f);
//    m_vBoundaryLow.y = ((sm_fBoundaryHeight / 7.0f) * 4.0f);
//
//    m_vBoundaryHigh.x = width - (m_pSprSpriteBody->GetWidth() / 2.0f);
//    m_vBoundaryHigh.y = height;
//}
//
////void
////Skeleton::DebugDraw()
////{
////
////}