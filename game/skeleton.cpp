// This include:
#include "skeleton.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "../imgui/imgui.h"
#include "player.h"
#include "MyContactListener.h"

// Library includes:
#include <cassert>
#include <cstdio>

Skeleton::Skeleton(b2World* world)
    : m_pEntCharacter(0)
    , m_pSprSpriteBody(0)
    , m_sAnimations{ 0, 0, 0 }
    , m_fAnimateScale(0.0f)
    , m_fExecutionTime(0.0f)
    , m_fDistToPlayer(0.0f)
    , m_fHitBoxRange(0.0f)
    , m_fAttackRangeMax(0.0f)
    , m_bPlayerInRange(false)
    , m_bSpotted(false)
    , m_bEngage(false)
    , m_bIsAnimating(false)
    , m_bAttack(false)
    , m_bRun(false)
    , m_pWorld(world)
    , m_pSlash(nullptr)
    , healthBar(nullptr)
{

}

Skeleton::~Skeleton()
{
    delete m_pSprSpriteBody;
    m_pSprSpriteBody = 0;

    delete m_sAnimations.m_pASprSkelIdle;
    m_sAnimations.m_pASprSkelIdle = 0;

    delete m_sAnimations.m_pASprSkelWalk;
    m_sAnimations.m_pASprSkelWalk = 0;

    delete m_sAnimations.m_pASprSkelAttack;
    m_sAnimations.m_pASprSkelAttack = 0;

    delete healthBar;
    healthBar = nullptr;
}

bool
Skeleton::Initialise(Renderer& renderer)
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
    m_fAttackRangeMax = 96 / SCALE;
    m_bAlive = true;

    healthBar = new Healthbar(renderer, 20.0f);
    if (m_pWorld)
    {
        m_vPosition.x = 2000.0f;
        m_vPosition.y = 500.0f;
        // Define the body
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(m_vPosition.x / SCALE, m_vPosition.y / SCALE);

        // Create the body in the world
        m_pBody = m_pWorld->CreateBody(&bodyDef);

        // Define the shape of the body (box shape in this example)
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(30 / SCALE, 32 / SCALE);

        // Define the fixture (physical properties)
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        //fixtureDef.isSensor = true;

        fixtureDef.filter.categoryBits = SKELETON;
        fixtureDef.filter.maskBits = PLAYER;

        // Attach the fixture to the body
        m_pBody->CreateFixture(&fixtureDef);

        // Set user data to identify this body as a Golem

        userData* skeletonData = new userData{ SKELETON, static_cast<void*>(this) };
        m_pBody->SetUserData(static_cast<void*>(skeletonData));

        m_sAnimations.m_pASprSkelIdle->SetX((int)m_pBody->GetPosition().x * SCALE);
        m_sAnimations.m_pASprSkelIdle->SetY((int)m_pBody->GetPosition().y * SCALE);

    }

    return true;
}

void
Skeleton::Process(float deltaTime, InputSystem& inputSystem)
{
    CheckPlayerDist();
    ProcessAction();

    b2Vec2 mushPosition = m_pBody->GetPosition();

    // Convert the Golem's Box2D position to pixels (if needed)
    mushPosition.x *= SCALE;
    mushPosition.y *= SCALE;

    // Check if the Golem is inside the camera view
    Vector2* cameraOffset = m_pCamera->GetOffset();
    float cameraX = cameraOffset->x;
    float cameraY = cameraOffset->y;

    // Assuming your camera has width and height fields
    float cameraWidth = m_pCamera->GetWidth();
    float cameraHeight = m_pCamera->GetHeight();

    bool inCameraView = (mushPosition.x > cameraX && mushPosition.x < cameraX + cameraWidth) &&
        (mushPosition.y > cameraY && mushPosition.y < cameraY + cameraHeight);

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
            m_sAnimations.m_pASprSkelIdle->Animate();
            m_sAnimations.m_pASprSkelIdle->SetLooping(true);
        }
    }
    else
    {
        m_fExecutionTime = 0.0f;
        m_sAnimations.m_pASprSkelIdle->Animate();
        m_sAnimations.m_pASprSkelIdle->SetLooping(true);
    }


    m_sAnimations.m_pASprSkelIdle->Process(deltaTime);
    m_sAnimations.m_pASprSkelWalk->Process(deltaTime);
    m_sAnimations.m_pASprSkelAttack->Process(deltaTime);

    m_sAnimations.m_pASprSkelIdle->SetX(m_pBody->GetPosition().x * SCALE);
    m_sAnimations.m_pASprSkelIdle->SetY(m_pBody->GetPosition().y * SCALE);
}

void
Skeleton::Draw(Renderer& renderer, Camera& camera)
{
    if (m_bAlive)
    {

        Vector2* cameraOffset = camera.GetOffset();

        int mushX = (int)(m_pBody->GetPosition().x * SCALE - cameraOffset->x);
        int mushY = (int)(m_pBody->GetPosition().y * SCALE - cameraOffset->y);

        if (m_bPlayerInRange)
        {
            if (m_bRun)
            {
                m_sAnimations.m_pASprSkelWalk->SetX(mushX);
                m_sAnimations.m_pASprSkelWalk->SetY(mushY);
                m_sAnimations.m_pASprSkelWalk->Draw(renderer, m_bFlipHorizontally, false);
            }
            else if (m_bAttack)
            {
                if (m_sAnimations.m_pASprSkelAttack->IsAnimating())
                {
                    m_sAnimations.m_pASprSkelAttack->SetX(mushX);
                    m_sAnimations.m_pASprSkelAttack->SetY(mushY);
                    m_sAnimations.m_pASprSkelAttack->Draw(renderer, m_bFlipHorizontally, false);
                }
                else
                {
                    m_bAttack = false;
                    m_bIsAnimating = false;
                }
            }
            else
            {
                m_sAnimations.m_pASprSkelIdle->SetX(mushX);
                m_sAnimations.m_pASprSkelIdle->SetY(mushY);
                m_sAnimations.m_pASprSkelIdle->Draw(renderer, m_bFlipHorizontally, false);
            }
        }
        else
        {
            m_sAnimations.m_pASprSkelIdle->SetX(mushX);
            m_sAnimations.m_pASprSkelIdle->SetY(mushY);
            m_sAnimations.m_pASprSkelIdle->Draw(renderer, m_bFlipHorizontally, false);
        }
    }
}

bool
Skeleton::SetBodySprites(Renderer& renderer)
{
    m_sAnimations.m_pASprSkelIdle = renderer.CreateAnimatedSprite("..\\Sprites\\skeleton\\anim8skelidle.png");

    if (!(m_sAnimations.m_pASprSkelIdle))
    {
        LogManager::GetInstance().Log("Skeleton idle animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSkelIdle->SetupFrames(96, 64);
        m_sAnimations.m_pASprSkelIdle->SetFrameDuration(0.07f);
        m_sAnimations.m_pASprSkelIdle->SetScale(2.5f);
    }

    m_sAnimations.m_pASprSkelAttack = renderer.CreateAnimatedSprite("..\\Sprites\\skeleton\\anim8skelattack.png");

    if (!(m_sAnimations.m_pASprSkelAttack))
    {
        LogManager::GetInstance().Log("Skeleton attack animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSkelAttack->SetupFrames(96, 64);
        m_sAnimations.m_pASprSkelAttack->SetFrameDuration(0.15f);
        m_sAnimations.m_pASprSkelAttack->SetScale(2.5f);
    }

    m_sAnimations.m_pASprSkelWalk = renderer.CreateAnimatedSprite("..\\Sprites\\skeleton\\anim8skelwalk.png");

    if (!(m_sAnimations.m_pASprSkelWalk))
    {
        LogManager::GetInstance().Log("Skeleton walk animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSkelWalk->SetupFrames(96, 64);
        m_sAnimations.m_pASprSkelWalk->SetFrameDuration(0.1f);
        m_sAnimations.m_pASprSkelWalk->SetScale(2.5f);
    }

    return true;
}

void
Skeleton::Move(void)
{
    if (m_bPlayerInRange)
    {
        b2Vec2 velocity = m_pBody->GetLinearVelocity();

        if (m_pEntCharacter->GetPosition().x < (m_pBody->GetPosition().x - m_fAttackRangeMax) &&
            m_pEntCharacter->GetPosition().x < m_pBody->GetPosition().x)
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
            m_bIsAnimating = true;
            velocity.x = -0.1f;
        }
        else if (m_pEntCharacter->GetPosition().x > (m_pBody->GetPosition().x + m_fAttackRangeMax) &&
            m_pEntCharacter->GetPosition().x > m_pBody->GetPosition().x)
        {
            m_iFacingDirection = 1;
            m_bFlipHorizontally = false;
            m_bIsAnimating = true;
            velocity.x = 0.1f;
        }
        else
        {
            m_velocityBody.x = 0.0f;
            m_bIsAnimating = false;
            m_bRun = false;
            m_fExecutionTime = 0.0f;
        }
        m_pBody->SetLinearVelocity(velocity);
    }
}

void
Skeleton::Action()
{
    if (m_fDistToPlayer < m_fAttackRangeMax && m_fDistToPlayer >= 0.0f)
    {
        if (!m_bRun)
        {
            m_sAnimations.m_pASprSkelWalk->SetLooping(false);
            m_sAnimations.m_pASprSkelWalk->Inanimate();
        }

        if (m_sAnimations.m_pASprSkelIdle->IsAnimating())
        {
            m_sAnimations.m_pASprSkelIdle->Inanimate();
            m_sAnimations.m_pASprSkelIdle->SetLooping(false);
        }

        if (!m_sAnimations.m_pASprSkelAttack->IsAnimating() && !m_bAttack)
        {
            m_sAnimations.m_pASprSkelAttack->Animate();
            m_bAttack = true;
        }
    }
    else
    {
        m_bRun = true;
    }
}

void
Skeleton::ProcessAction()
{
    if (m_bAttack)
    {
        if (m_pSlash == nullptr)
        {
            CreateSlash();
        }

    }
    else
    {
        if (m_pSlash != nullptr)
        {
            DeleteSlash();
        }
    }
}

void Skeleton::CreateSlash()
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
    m_pSlash = m_pWorld->CreateBody(&SlashbodyDef);

    // Define the shape of the body (box shape in this example)
    b2PolygonShape staticBox;

    staticBox.SetAsBox(48 / SCALE, 32 / SCALE);

    // Define the fixture (physical properties)
    b2FixtureDef SlashfixtureDef;
    SlashfixtureDef.shape = &staticBox;
    SlashfixtureDef.density = 0.0f;
    SlashfixtureDef.friction = 0.0f;
    SlashfixtureDef.isSensor = true;

    // Attach the fixture to the body
    m_pSlash->CreateFixture(&SlashfixtureDef);

    // Set user data to identify this body as a Golem

    userData* attackData = new userData{ SKELETON_ATTACK, static_cast<void*>(this) };
    m_pSlash->SetUserData(static_cast<void*>(attackData));

}

void Skeleton::DeleteSlash()
{
    if (m_pSlash)
    {
        m_pWorld->DestroyBody(m_pSlash);
        m_pSlash = nullptr;
    }
}

void
Skeleton::CheckPlayerDist()
{
    // Get player position using Box2D body - in meters
    b2Vec2 playerPosition = m_pEntCharacter->GetPosition();

    // Get Golem's current position - in meters
    b2Vec2 skelPosition = m_pBody->GetPosition();

    // Calculate the differences in the x and y coordinates
    m_fDistToPlayer = abs(playerPosition.x - skelPosition.x);
}

int
Skeleton::GetBodyWidth()
{
    return (int)(m_fHitBoxRange * 2);
}

void Skeleton::SetCamera(Camera* camera)
{
    m_pCamera = camera;
    IsCameraSet = true;
}

void Skeleton::SetPlayer(Player* player)
{
    m_pEntCharacter = player;
}
//Changes made by Kyle
Healthbar* Skeleton::getEnemyHealth()
{
    return healthBar;
}


//void
//Skeleton::DebugDraw()
//{
//
//}