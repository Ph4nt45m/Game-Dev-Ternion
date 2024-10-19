// This include:
#include "spider.h"

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

Spider::Spider(b2World* world)
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
    , m_pBite(nullptr)
{

}

Spider::~Spider()
{
    delete m_pSprSpriteBody;
    m_pSprSpriteBody = 0;

    delete m_sAnimations.m_pASprSpiderIdle;
    m_sAnimations.m_pASprSpiderIdle = 0;

    delete m_sAnimations.m_pASprSpiderWalk;
    m_sAnimations.m_pASprSpiderWalk = 0;

    delete m_sAnimations.m_pASprSpiderAttack;
    m_sAnimations.m_pASprSpiderAttack = 0;
}

bool
Spider::Initialise(Renderer& renderer)
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
    m_fAttackRangeMax = 80 / SCALE;
    m_bAlive = true;


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
        dynamicBox.SetAsBox(48 / SCALE, 32 / SCALE);

        // Define the fixture (physical properties)
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        //fixtureDef.isSensor = true;

        fixtureDef.filter.categoryBits = SPIDER;
        fixtureDef.filter.maskBits = PLAYER;
        // Attach the fixture to the body
        m_pBody->CreateFixture(&fixtureDef);

        // Set user data to identify this body as a Spider

        userData* spiderData = new userData{ SPIDER, static_cast<void*>(this) };
        m_pBody->SetUserData(static_cast<void*>(spiderData));

        m_sAnimations.m_pASprSpiderIdle->SetX((int)m_pBody->GetPosition().x * SCALE);
        m_sAnimations.m_pASprSpiderIdle->SetY((int)m_pBody->GetPosition().y * SCALE);

    }

    return true;
}

void
Spider::Process(float deltaTime, InputSystem& inputSystem)
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

            if (!m_sAnimations.m_pASprSpiderWalk->IsAnimating())
            {
                m_sAnimations.m_pASprSpiderWalk->Animate();
                m_sAnimations.m_pASprSpiderWalk->SetLooping(true);
            }
        }
        else
        {
            m_sAnimations.m_pASprSpiderWalk->Inanimate();
            m_sAnimations.m_pASprSpiderWalk->SetLooping(false);
            m_sAnimations.m_pASprSpiderIdle->Animate();
            m_sAnimations.m_pASprSpiderIdle->SetLooping(true);
        }
    }
    else
    {
        m_fExecutionTime = 0.0f;
        m_sAnimations.m_pASprSpiderIdle->Animate();
        m_sAnimations.m_pASprSpiderIdle->SetLooping(true);
    }


    m_sAnimations.m_pASprSpiderIdle->Process(deltaTime);
    m_sAnimations.m_pASprSpiderWalk->Process(deltaTime);
    m_sAnimations.m_pASprSpiderAttack->Process(deltaTime);

    m_sAnimations.m_pASprSpiderIdle->SetX(m_pBody->GetPosition().x * SCALE);
    m_sAnimations.m_pASprSpiderIdle->SetY(m_pBody->GetPosition().y * SCALE);
}

void
Spider::Draw(Renderer& renderer, Camera& camera)
{
    if (m_bAlive)
    {

        Vector2* cameraOffset = camera.GetOffset();

        int spiderX = (int)(m_pBody->GetPosition().x * SCALE - cameraOffset->x);
        int spiderY = (int)(m_pBody->GetPosition().y * SCALE - cameraOffset->y);

        if (m_bPlayerInRange)
        {
            if (m_bRun)
            {
                m_sAnimations.m_pASprSpiderWalk->SetX(spiderX);
                m_sAnimations.m_pASprSpiderWalk->SetY(spiderY);
                m_sAnimations.m_pASprSpiderWalk->Draw(renderer, m_bFlipHorizontally, false);
            }
            else if (m_bAttack)
            {
                if (m_sAnimations.m_pASprSpiderAttack->IsAnimating())
                {
                    m_sAnimations.m_pASprSpiderAttack->SetX(spiderX);
                    m_sAnimations.m_pASprSpiderAttack->SetY(spiderY);
                    m_sAnimations.m_pASprSpiderAttack->Draw(renderer, m_bFlipHorizontally, false);
                }
                else
                {
                    m_bAttack = false;
                    m_bIsAnimating = false;
                }
            }
            else
            {
                m_sAnimations.m_pASprSpiderIdle->SetX(spiderX);
                m_sAnimations.m_pASprSpiderIdle->SetY(spiderY);
                m_sAnimations.m_pASprSpiderIdle->Draw(renderer, m_bFlipHorizontally, false);
            }
        }
        else
        {
            m_sAnimations.m_pASprSpiderIdle->SetX(spiderX);
            m_sAnimations.m_pASprSpiderIdle->SetY(spiderY);
            m_sAnimations.m_pASprSpiderIdle->Draw(renderer, m_bFlipHorizontally, false);
        }
    }
}

bool
Spider::SetBodySprites(Renderer& renderer)
{
    m_sAnimations.m_pASprSpiderIdle = renderer.CreateAnimatedSprite("..\\Sprites\\spider\\anim8spidersleep.png");

    if (!(m_sAnimations.m_pASprSpiderIdle))
    {
        LogManager::GetInstance().Log("Spider sleep animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSpiderIdle->SetupFrames(56, 34);
        m_sAnimations.m_pASprSpiderIdle->SetFrameDuration(0.07f);
        m_sAnimations.m_pASprSpiderIdle->SetScale(2.5f);
    }

    m_sAnimations.m_pASprSpiderAttack = renderer.CreateAnimatedSprite("..\\Sprites\\spider\\anim8spiderattack.png");

    if (!(m_sAnimations.m_pASprSpiderAttack))
    {
        LogManager::GetInstance().Log("Spider bite animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSpiderAttack->SetupFrames(56, 34);
        m_sAnimations.m_pASprSpiderAttack->SetFrameDuration(0.15f);
        m_sAnimations.m_pASprSpiderAttack->SetScale(2.5f);
    }

    m_sAnimations.m_pASprSpiderWalk = renderer.CreateAnimatedSprite("..\\Sprites\\spider\\anim8spiderwalk.png");

    if (!(m_sAnimations.m_pASprSpiderWalk))
    {
        LogManager::GetInstance().Log("Spider walk animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprSpiderWalk->SetupFrames(56, 34);
        m_sAnimations.m_pASprSpiderWalk->SetFrameDuration(0.1f);
        m_sAnimations.m_pASprSpiderWalk->SetScale(2.5f);
    }

    return true;
}

void
Spider::Move(void)
{
    if (m_bPlayerInRange)
    {
        b2Vec2 velocity = m_pBody->GetLinearVelocity();

        if (m_pEntCharacter->GetPosition().x < (m_pBody->GetPosition().x - m_fAttackRangeMax) &&
            m_pEntCharacter->GetPosition().x < m_pBody->GetPosition().x)
        {
            m_iFacingDirection = 1;
            m_bFlipHorizontally = false;
            m_bIsAnimating = true;
            velocity.x = -0.1f;
        }
        else if (m_pEntCharacter->GetPosition().x > (m_pBody->GetPosition().x + m_fAttackRangeMax) &&
            m_pEntCharacter->GetPosition().x > m_pBody->GetPosition().x)
        {
            m_iFacingDirection = -1;
            m_bFlipHorizontally = true;
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
Spider::Action()
{
    if (m_fDistToPlayer < m_fAttackRangeMax && m_fDistToPlayer >= 0.0f)
    {
        if (!m_bRun)
        {
            m_sAnimations.m_pASprSpiderWalk->SetLooping(false);
            m_sAnimations.m_pASprSpiderWalk->Inanimate();
        }

        if (m_sAnimations.m_pASprSpiderIdle->IsAnimating())
        {
            m_sAnimations.m_pASprSpiderIdle->Inanimate();
            m_sAnimations.m_pASprSpiderIdle->SetLooping(false);
        }

        if (!m_sAnimations.m_pASprSpiderAttack->IsAnimating() && !m_bAttack)
        {
            m_sAnimations.m_pASprSpiderAttack->Animate();
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
    if (m_bAttack)
    {
        if (m_pBite == nullptr)
        {
            CreateSlash();
        }

    }
    else
    {
        if (m_pBite != nullptr)
        {
            DeleteSlash();
        }
    }
}

void Spider::CreateSlash()
{
    // Define the body
    b2BodyDef BitebodyDef;
    BitebodyDef.type = b2_staticBody;
    if (m_iFacingDirection == -1)
    {
        BitebodyDef.position.Set((m_pBody->GetPosition().x + 0.1f), m_pBody->GetPosition().y);
    }
    else
    {
        BitebodyDef.position.Set((m_pBody->GetPosition().x - 0.1f), m_pBody->GetPosition().y);
    }

    // Create the body in the world
    m_pBite = m_pWorld->CreateBody(&BitebodyDef);

    // Define the shape of the body (box shape in this example)
    b2PolygonShape staticBox;

    staticBox.SetAsBox(30 / SCALE, 32 / SCALE);

    // Define the fixture (physical properties)
    b2FixtureDef BitefixtureDef;
    BitefixtureDef.shape = &staticBox;
    BitefixtureDef.density = 0.0f;
    BitefixtureDef.friction = 0.0f;
    BitefixtureDef.isSensor = true;

    // Attach the fixture to the body
    m_pBite->CreateFixture(&BitefixtureDef);

    // Set user data to identify this body as a spider

    userData* biteData = new userData{ SPIDER_BITE, static_cast<void*>(this) };
    m_pBite->SetUserData(static_cast<void*>(biteData));


}

void Spider::DeleteSlash()
{
    if (m_pBite)
    {
        m_pWorld->DestroyBody(m_pBite);
        m_pBite = nullptr;
    }
}

void
Spider::CheckPlayerDist()
{
    // Get player position using Box2D body - in meters
    b2Vec2 playerPosition = m_pEntCharacter->GetPosition();

    // Get Golem's current position - in meters
    b2Vec2 spiderPosition = m_pBody->GetPosition();

    // Calculate the differences in the x and y coordinates
    m_fDistToPlayer = abs(playerPosition.x - spiderPosition.x);
}

int
Spider::GetBodyWidth()
{
    return (int)(m_fHitBoxRange * 2);
}

void Spider::SetCamera(Camera* camera)
{
    m_pCamera = camera;
    IsCameraSet = true;
}

void Spider::SetPlayer(Player* player)
{
    m_pEntCharacter = player;
}

//void
//Spider::DebugDraw()
//{
//
//}