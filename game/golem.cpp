// This include:
#include "golem.h"

// Local includes:
#include "game.h"
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "../imgui/imgui.h"
#include "character.h"
#include "player.h" // Changes made by Karl
#include "projectile.h"
#include "collision.h"
#include "MyContactListener.h"


// Library includes:
#include <cassert>
#include <cstdio>
#include <stdio.h>

Golem::Golem(b2World* world)
    : m_pEntCharacter(0)
    , m_pSprSpriteBody(0)
    , m_pEntProjectile(0)
    , m_sAnimations{ 0, 0, 0, 0, 0 }
    , m_fAnimateScale(0.0f)
    , m_fExecutionTime(0.0f)
    , m_iAttackType(0)
    , m_fDistToPlayer(0.0f)
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
    , m_pBody(0)
    , slashBody(nullptr)
    , slashWidth(0)
    , slashHeight(0)
    , m_pWorld(world)
    , m_pcamera(nullptr)
    , IsCameraSet(false)
    , m_pSlamBody(nullptr)
    , m_fSlamWidth(0.0f)
    , m_fSlamHeight(0.0f)
    , m_fSlashHeight(0.0f)
    , m_fSlashWidth(0.0f)
    , m_pSlashBody(nullptr)
    , m_fSlamTime(0.0f)
    , growSize(0.0)
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
    m_fSlamRangeMax = ((m_sAnimations.m_pASprGolemSlam->GetWidth() / 2.0f) / SCALE);
    m_fThrowRangeMax = m_fDistToPlayer;
    m_iAttackType = 0;
    m_bAlive = true;


    m_pEntProjectile = new Projectile();

    if (!m_pEntProjectile->Initialise(renderer))
    {
        LogManager::GetInstance().Log("Golem Projectile failed to initialise!");
        return false;
    }
    else
    {   // Changes made by Karl
        m_pEntProjectile->SetProjectileSprite(renderer, "Sprites\\ball.png"); //DO NOT TOUCH THIS CODE
        m_pEntProjectile->SetTimeToTarget(1.5f);
    }

    //Changes made by Rauen

    // Create the Box2D body for the Golem
    if (m_pWorld)
    {
        m_vPosition.x = 3000.0f;
        m_vPosition.y = 500.0f;
        // Define the body
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(m_vPosition.x / SCALE, m_vPosition.y / SCALE);

        // Create the body in the world
        m_pBody = m_pWorld->CreateBody(&bodyDef);

        // Define the shape of the body (box shape in this example)
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox((m_pSprSpriteBody->GetWidth() / 7.0f) / SCALE, (m_pSprSpriteBody->GetHeight() / 4.0f + 30) / SCALE);

        // Define the fixture (physical properties)
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        fixtureDef.filter.categoryBits = GOLEM;
        fixtureDef.filter.maskBits = PLAYER;

        // Attach the fixture to the body
        m_pBody->CreateFixture(&fixtureDef);

        // Set user data to identify this body as a Golem
        m_pBody->SetUserData((void*)GOLEM);
        m_pSprSpriteBody->SetX((int)m_pBody->GetPosition().x * SCALE);
        m_pSprSpriteBody->SetY((int)m_pBody->GetPosition().y * SCALE);

    }

    return true;
}

void
Golem::Process(float deltaTime, InputSystem& inputSystem)
{
    if (IsCameraSet) {
        b2Vec2 golemPosition = m_pBody->GetPosition();

        // Convert the Golem's Box2D position to pixels (if needed)
        golemPosition.x *= SCALE;
        golemPosition.y *= SCALE;

        // Check if the Golem is inside the camera view
        Vector2* cameraOffset = m_pcamera->GetOffset();
        float cameraX = cameraOffset->x;
        float cameraY = cameraOffset->y;

        // Assuming your camera has width and height fields
        float cameraWidth = m_pcamera->GetWidth();
        float cameraHeight = m_pcamera->GetHeight();

        bool inCameraView = (golemPosition.x > cameraX && golemPosition.x < cameraX + cameraWidth) &&
            (golemPosition.y > cameraY && golemPosition.y < cameraY + cameraHeight);

        if (inCameraView)
        {
            // Golem is in the camera view, enable movement
            m_bPlayerInRange = true;  // Enable movement logic (this can be used for AI decisions)
            m_pBody->SetAwake(true);  // Wake up the body in the physics simulation
        }
        else
        {
            // Golem is outside of camera view, disable movement
            m_bPlayerInRange = false;  // Disable movement logic
            m_pBody->SetAwake(false);  // Sleep the body in the physics simulation
        }

        if (m_bPlayerInRange)
        {
            CheckPlayerDist();
            ProcessAction();

            m_fExecutionTime += deltaTime;
            //potentiol
            if (!m_bWalk && (((int)m_fExecutionTime % 5) == 0))
            {
                Action(deltaTime);

            }
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
        else
        {
            m_fExecutionTime = 0.0f;
        }

        m_sAnimations.m_pASprGolemWalk->Process(deltaTime);
        m_sAnimations.m_pASprGolemJump->Process(deltaTime);
        m_sAnimations.m_pASprGolemSlam->Process(deltaTime);
        m_sAnimations.m_pASprGolemSlash->Process(deltaTime);
        m_sAnimations.m_pASprGolemThrow->Process(deltaTime);
        m_pEntProjectile->Process(deltaTime, inputSystem);
    }
}

void Golem::Draw(Renderer& renderer, Camera& camera)
{
    if (m_bAlive)
    {
        // Get the camera offset
        Vector2* cameraOffset = camera.GetOffset();

        // Adjust Golem position based on camera offset
        int golemX = (int)(m_pBody->GetPosition().x * SCALE - cameraOffset->x);
        int golemY = (int)(m_pBody->GetPosition().y * SCALE - cameraOffset->y);

        if (m_bPlayerInRange)
        {
            if (m_bWalk)
            {
                m_sAnimations.m_pASprGolemWalk->SetX(golemX);
                m_sAnimations.m_pASprGolemWalk->SetY(golemY);
                m_sAnimations.m_pASprGolemWalk->Draw(renderer, m_bFlipHorizontally, false);
            }
            else if (m_bSlash)
            {
                if (m_sAnimations.m_pASprGolemSlash->IsAnimating())
                {
                    m_sAnimations.m_pASprGolemSlash->SetX(golemX);
                    m_sAnimations.m_pASprGolemSlash->SetY(golemY);
                    m_sAnimations.m_pASprGolemSlash->Draw(renderer, m_bFlipHorizontally, false);
                }
                else
                {
                    m_bSlash = false;
                    m_bIsAnimating = false;

                    /*if (m_fDistToPlayer < m_fSlamRangeMax)
                    {
                        m_iAttackType = 1;
                        growSize = 0.0f;
                    }*/
                    /*else if (m_fDistToPlayer >= m_fSlamRangeMax)
                    {
                        m_iAttackType = 2;
                    }*/

                    m_iAttackType = 1;
                }
            }
            else if (m_bSlam)
            {
                if (m_sAnimations.m_pASprGolemJump->IsAnimating())
                {
                    m_sAnimations.m_pASprGolemJump->SetX(golemX);
                    m_sAnimations.m_pASprGolemJump->SetY(golemY);
                    m_sAnimations.m_pASprGolemJump->Draw(renderer, m_bFlipHorizontally, false);

                }
                else
                {
                    if (m_bJumping)
                    {

                        m_sAnimations.m_pASprGolemSlam->SetX(golemX);
                        m_sAnimations.m_pASprGolemSlam->SetY(golemY);

                        if (!m_sAnimations.m_pASprGolemSlam->IsAnimating())
                        {
                            m_bJumping = false;
                            m_sAnimations.m_pASprGolemSlam->Animate();
                        }
                    }
                    else if (m_sAnimations.m_pASprGolemSlam->IsAnimating())
                    {

                        m_sAnimations.m_pASprGolemSlam->SetX(golemX);
                        m_sAnimations.m_pASprGolemSlam->SetY(golemY + 100);
                        m_sAnimations.m_pASprGolemSlam->Draw(renderer, m_bFlipHorizontally, false);

                        m_pSprSpriteBody->SetX(golemX);
                        m_pSprSpriteBody->SetY(golemY);
                        m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, true);

                    }
                    else
                    {

                        m_bSlam = false;
                        m_bIsAnimating = false;

                        /*if (m_fDistToPlayer < m_fSlamRangeMax)
                        {
                            m_iAttackType = 0;
                        }
                        else if (m_fDistToPlayer >= m_fSlamRangeMax)
                        {
                            m_iAttackType = 2;
                        }*/

                        m_iAttackType = 0;
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
                    m_pEntProjectile->SetStartPos(m_vPosition.x, m_vPosition.y);
                    m_pEntProjectile->SetTargetPos(m_pEntCharacter->GetPosition().x, m_pEntCharacter->GetPosition().y);
                    m_pEntProjectile->SetGroundY(m_pEntCharacter->GetPosition().y);
                    m_pEntProjectile->Shoot();
                    m_bShoot = false;
                    m_bProjectile = false;
                    m_bIsAnimating = false;

                    if (m_fDistToPlayer < m_fSlashRangeMax)
                    {
                        m_iAttackType = 0;
                    }
                    else if (m_fDistToPlayer < m_fSlamRangeMax && m_fDistToPlayer >= m_fSlashRangeMax || m_iAttackType == 2)
                    {
                        m_iAttackType = 1;
                    }
                }
            }
            else
            {
                m_pSprSpriteBody->SetX(golemX);
                m_pSprSpriteBody->SetY(golemY);
                m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, true);
            }
        }
        else
        {
            m_pSprSpriteBody->SetX(golemX);
            m_pSprSpriteBody->SetY(golemY);
            m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, true);
        }

        if (!m_bShoot && m_bProjectile && m_iAttackType == 2)
        {
            m_iAttackType = 1;
        }

        m_pEntProjectile->Draw(renderer, camera);
    }
}


bool
Golem::SetBodySprites(Renderer& renderer)
{   //Changes made by Karl - Start
    m_pSprSpriteBody = renderer.CreateSprite("Sprites\\golem\\golem.png");

    if (!(m_pSprSpriteBody))
    {
        LogManager::GetInstance().Log("Golem failed to initialise!");
        return false;
    }

    m_sAnimations.m_pASprGolemWalk = renderer.CreateAnimatedSprite("Sprites\\golem\\anim8golemwalk.png");

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

    m_sAnimations.m_pASprGolemSlash = renderer.CreateAnimatedSprite("Sprites\\golem\\anim8golemslash.png");

    if (!(m_sAnimations.m_pASprGolemSlash))
    {
        LogManager::GetInstance().Log("Golem Slash animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprGolemSlash->SetupFrames(391, 391);
        /*m_fSlashWidth = 391 / SCALE / 8.0f;
        m_fSlashHeight = 391 / SCALE;
        m_fSlashRangeMax = m_fSlashWidth * 3.0f;*/
        m_fSlashWidth = ((350 / 2.0f) / SCALE);
        m_fSlashHeight = ((350 / 2.0f) / SCALE);
        m_fSlashRangeMax = m_fSlashWidth;
        m_sAnimations.m_pASprGolemSlash->SetFrameDuration(0.15f);
    }

    m_sAnimations.m_pASprGolemJump = renderer.CreateAnimatedSprite("Sprites\\golem\\anim8golemjump.png");

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
        m_sAnimations.m_pASprGolemSlam->SetupFrames(708, 71);
        m_fSlamWidth = (708.0f / 2.0f) / SCALE;
        m_fSlamHeight = 71.0f / SCALE;
        m_fSlamRangeMax = m_fSlamWidth / 2;
        m_sAnimations.m_pASprGolemSlam->SetFrameDuration(0.15f);
    }

    m_sAnimations.m_pASprGolemThrow = renderer.CreateAnimatedSprite("Sprites\\golem\\anim8golemthrow.png");
    // Changes made by Karl - End
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

void Golem::Move(int attackType)
{
    if (m_bPlayerInRange)
    {
        b2Vec2 velocity = m_pBody->GetLinearVelocity(); // Get current velocity from Box2D body

        switch (attackType)
        {
        case 0: // Slash Attack
            if (m_pEntCharacter->GetPosition().x < (m_pBody->GetPosition().x - m_fSlashRangeMax) &&
                m_pEntCharacter->GetPosition().x < m_pBody->GetPosition().x)
            {
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_bIsAnimating = true;
                velocity.x = -0.1f;  // Set movement to left
            }
            else if (m_pEntCharacter->GetPosition().x > (m_pBody->GetPosition().x + m_fSlashRangeMax) &&
                m_pEntCharacter->GetPosition().x > m_pBody->GetPosition().x)
            {
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
                m_bIsAnimating = true;
                velocity.x = 0.1f;  // Set movement to right
            }
            else
            {
                velocity.x = 0.0f;  // Stop movement
                m_bIsAnimating = false;
                m_bWalk = false;
                m_fExecutionTime = 0.0f;
            }
            break;

        case 1: // Slam Attack
            if (m_pEntCharacter->GetPosition().x < (m_pBody->GetPosition().x - m_fSlamRangeMax) &&
                m_pEntCharacter->GetPosition().x < m_pBody->GetPosition().x)
            {
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_bIsAnimating = true;
                //m_bSlam = true;
                velocity.x = -0.1f;  // Move left
            }
            else if (m_pEntCharacter->GetPosition().x > (m_pBody->GetPosition().x + m_fSlamRangeMax) &&
                m_pEntCharacter->GetPosition().x > m_pBody->GetPosition().x)
            {
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
                m_bIsAnimating = true;
                velocity.x = 0.1f;  // Move right
            }
            else
            {
                velocity.x = 0.0f;  // Stop movement
                m_bIsAnimating = false;
                m_bWalk = false;
                m_bSlam = true;
                m_fExecutionTime = 0.0f;
            }
            break;

        case 2: // Projectile Attack
            if (m_pEntCharacter->GetPosition().x < (m_pBody->GetPosition().x - m_fThrowRangeMax) &&
                m_pEntCharacter->GetPosition().x < m_pBody->GetPosition().x)
            {
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_bIsAnimating = true;
                m_bShoot = true;
                velocity.x = -0.1f;  // Move left
            }
            else if (m_pEntCharacter->GetPosition().x > (m_pBody->GetPosition().x + m_fThrowRangeMax) &&
                m_pEntCharacter->GetPosition().x > m_pBody->GetPosition().x)
            {
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
                m_bIsAnimating = true;
                velocity.x = 0.1f;  // Move right
            }
            else
            {
                velocity.x = 0.0f;  // Stop movement
                m_bIsAnimating = false;
                m_bWalk = false;
                m_bSlam = true;
                m_fExecutionTime = 0.0f;
            }
            break;

        case 3: // Return to Starting Position
            if ((int)m_pBody->GetPosition().x < (int)m_vStartingPos.x)
            {
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
                m_bIsAnimating = true;
                velocity.x = 0.1f;  // Move right
            }
            else if ((int)m_pBody->GetPosition().x > (int)m_vStartingPos.x)
            {
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                m_bIsAnimating = true;
                velocity.x = -0.1f;  // Move left
            }
            else
            {
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
                velocity.x = 0.0f;  // Stop movement
                m_bWalk = false;
                m_iAttackType = 2;
                m_fExecutionTime = 0.0f;
            }
            break;
        }

        // Apply the updated velocity to the Box2D body
        m_pBody->SetLinearVelocity(velocity);
    }
    else
    {
        // If the player is not in range, return to starting position
        b2Vec2 velocity = m_pBody->GetLinearVelocity();

        if ((int)m_pBody->GetPosition().x < (int)m_vStartingPos.x)
        {
            m_iFacingDirection = 1;
            m_bFlipHorizontally = false;
            m_bIsAnimating = true;
            m_bWalk = true;
            velocity.x = 0.1f;  // Move right
        }
        else if ((int)m_pBody->GetPosition().x > (int)m_vStartingPos.x)
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_bIsAnimating = true;
            m_bWalk = true;
            velocity.x = -0.1f;  // Move left
        }
        else
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_bIsAnimating = false;
            m_bWalk = false;
            velocity.x = 0.0f;  // Stop movement
        }

        // Apply the updated velocity to the Box2D body
        m_pBody->SetLinearVelocity(velocity);
        m_pSprSpriteBody->SetX((int)m_pBody->GetPosition().x * SCALE);
        m_pSprSpriteBody->SetY((int)m_pBody->GetPosition().y * SCALE);
    }
}


void
Golem::Action(float deltaTime)
{
    //throw
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
    }//slam
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
    }//slash
    else if (m_iAttackType == 0)
    {
        if (m_fDistToPlayer <= m_fSlashRangeMax && !m_bIsAnimating)
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

void Golem::ProcessAction()
{
    //slash
    if (m_bSlash)
    {
        if (m_pSlashBody == nullptr)
        {
            CreateSlashBody();
        }
    }
    else
    {
        if (m_pSlashBody != nullptr)
        {
            DeleteSlash();
        }
    }

    //slam
    if (!m_bJumping && m_bSlam)
    {
        if (m_pSlamBody == nullptr)
        {
            CreateSlamBody();
        }
    }
    else if (!m_bJumping && !m_bSlam)
    {
        if (m_pSlamBody != nullptr)
        {
            DeleteSlam();
        }
    }
}

void Golem::CreateSlashBody()
{
    // Define the body
    b2BodyDef SlashbodyDef;
    SlashbodyDef.type = b2_staticBody;
    if (m_iFacingDirection == -1)
    {
        SlashbodyDef.position.Set((m_pBody->GetPosition().x - 0.5f), m_pBody->GetPosition().y);
    }
    else
    {
        SlashbodyDef.position.Set((m_pBody->GetPosition().x + 0.5f), m_pBody->GetPosition().y);
    }

    // Create the body in the world
    m_pSlashBody = m_pWorld->CreateBody(&SlashbodyDef);

    // Define the shape of the body (box shape in this example)
    b2PolygonShape staticBox;
    
    staticBox.SetAsBox(m_fSlashWidth + 0.1f, m_fSlashHeight);

    // Define the fixture (physical properties)
    b2FixtureDef SlashfixtureDef;
    SlashfixtureDef.shape = &staticBox;
    SlashfixtureDef.density = 0.0f;
    SlashfixtureDef.friction = 0.0f;
    SlashfixtureDef.isSensor = true;
    SlashfixtureDef.filter.categoryBits = GOLEM_SLASH;
    SlashfixtureDef.filter.maskBits = PLAYER;

    // Attach the fixture to the body
    m_pSlashBody->CreateFixture(&SlashfixtureDef);

    // Set user data to identify this body as a Golem
    m_pSlashBody->SetUserData((void*)GOLEM_SLASH);

}

void Golem::DeleteSlash()
{
    if (m_pSlashBody != nullptr)
    {
        m_pWorld->DestroyBody(m_pSlashBody);
        m_pSlashBody = nullptr;
    }
}

void Golem::CreateSlamBody()
{
    // Define the body
    b2BodyDef SlambodyDef;
    SlambodyDef.type = b2_staticBody;

    SlambodyDef.position.Set(m_pBody->GetPosition().x, m_pBody->GetPosition().y);


    // Create the body in the world
    m_pSlamBody = m_pWorld->CreateBody(&SlambodyDef);

    // Define the shape of the body (box shape in this example)
    b2PolygonShape staticBox;

    staticBox.SetAsBox(m_fSlamWidth, m_fSlamHeight);

    // Define the fixture (physical properties)
    b2FixtureDef SlamfixtureDef;
    SlamfixtureDef.shape = &staticBox;
    SlamfixtureDef.density = 0.0f;
    SlamfixtureDef.isSensor = true;
    SlamfixtureDef.filter.categoryBits = GOLEM_SLAM;
    SlamfixtureDef.filter.maskBits = PLAYER;

    // Attach the fixture to the body
    m_pSlamBody->CreateFixture(&SlamfixtureDef);

    // Set user data to identify this body as a Golem
    m_pSlamBody->SetUserData((void*)GOLEM_SLAM);

}


void Golem::DeleteSlam()
{
    if (m_pSlamBody != nullptr)
    {
        m_pWorld->DestroyBody(m_pSlamBody);
        m_pSlamBody = nullptr;
    }

}

Vector2&
Golem::GetPosition()
{
    return m_vStandingPos;
}


void Golem::CheckPlayerDist()
{
    // Get player position using Box2D body - in meters
    b2Vec2 playerPosition = m_pEntCharacter->GetPosition();

    // Get Golem's current position - in meters
    b2Vec2 golemPosition = m_pBody->GetPosition();

    if (playerPosition.x < golemPosition.x)
    {
        m_iFacingDirection = -1;
        m_bFlipHorizontally = true;
    }
    else
    {
        m_iFacingDirection = 1;
        m_bFlipHorizontally = false;
    }
    
    // Calculate the differences in the x and y coordinates
    m_fDistToPlayer = abs(playerPosition.x - golemPosition.x);
}

void Golem::SetPlayer(Player* player)
{
    m_pEntCharacter = player;
}

void Golem::SetCamera(Camera* camera)
{
    m_pcamera = camera;
    IsCameraSet = true;
}


//void
//Golem::DebugDraw()
//{
//
//}
