// This include:
#include "mushroom.h"

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

Mushroom::Mushroom(b2World* world)
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
    , m_pHeadButt(nullptr)
{

}

Mushroom::~Mushroom()
{
    delete m_pSprSpriteBody;
    m_pSprSpriteBody = 0;

    delete m_sAnimations.m_pASprMushIdle;
    m_sAnimations.m_pASprMushIdle = 0;

    delete m_sAnimations.m_pASprMushWalk;
    m_sAnimations.m_pASprMushWalk = 0;

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
    m_fAttackRangeMax = 80/SCALE;
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
        dynamicBox.SetAsBox(40 / SCALE, 64 / SCALE);

        // Define the fixture (physical properties)
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        //fixtureDef.isSensor = true;

        // Attach the fixture to the body
        m_pBody->CreateFixture(&fixtureDef);

        // Set user data to identify this body as a Golem
        userData* mushData = new userData{ MUSHROOM, static_cast<void*>(this) };
        m_pBody->SetUserData(static_cast<void*>(mushData));

        m_sAnimations.m_pASprMushIdle->SetX((int)m_pBody->GetPosition().x * SCALE);
        m_sAnimations.m_pASprMushIdle->SetY((int)m_pBody->GetPosition().y * SCALE);

    }
    healthBar = new Healthbar(renderer, 20.0f);//Changes made by kyle

    return true;
}

void
Mushroom::Process(float deltaTime, InputSystem& inputSystem)
{
    if (!m_bAlive)
    {
        return;
    }

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

            if (!m_sAnimations.m_pASprMushWalk->IsAnimating())
            {
                m_sAnimations.m_pASprMushWalk->Animate();
                m_sAnimations.m_pASprMushWalk->SetLooping(true);
            }
        }
        else
        {
            m_sAnimations.m_pASprMushWalk->Inanimate();
            m_sAnimations.m_pASprMushWalk->SetLooping(false);
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


    m_sAnimations.m_pASprMushIdle->Process(deltaTime);
    m_sAnimations.m_pASprMushWalk->Process(deltaTime);
    m_sAnimations.m_pASprMushAttack->Process(deltaTime);

    m_sAnimations.m_pASprMushIdle->SetX(m_pBody->GetPosition().x * SCALE);
    m_sAnimations.m_pASprMushIdle->SetY(m_pBody->GetPosition().y * SCALE);
    if (getEnemyHealth()->GetCurrentHealth() <= 0.0f)
    {
        DeleteBody();
        m_bAlive = false;
    }

}

void
Mushroom::Draw(Renderer& renderer, Camera& camera)
{
    if (!m_bAlive)
    {
        return;
    }

    Vector2* cameraOffset = camera.GetOffset();

    int mushX = (int)(m_pBody->GetPosition().x * SCALE - cameraOffset->x);
    int mushY = (int)(m_pBody->GetPosition().y * SCALE - cameraOffset->y);

    if (m_bPlayerInRange)
    {
        if (m_bRun)
        {
            m_sAnimations.m_pASprMushWalk->SetX(mushX);
            m_sAnimations.m_pASprMushWalk->SetY(mushY);
            m_sAnimations.m_pASprMushWalk->Draw(renderer, m_bFlipHorizontally, false);
        }
        else if (m_bAttack)
        {
            if (m_sAnimations.m_pASprMushAttack->IsAnimating())
            {
                m_sAnimations.m_pASprMushAttack->SetX(mushX);
                m_sAnimations.m_pASprMushAttack->SetY(mushY);
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
            m_sAnimations.m_pASprMushIdle->SetX(mushX);
            m_sAnimations.m_pASprMushIdle->SetY(mushY);
            m_sAnimations.m_pASprMushIdle->Draw(renderer, m_bFlipHorizontally, false);
        }
    }
    else
    {
        m_sAnimations.m_pASprMushIdle->SetX(mushX);
        m_sAnimations.m_pASprMushIdle->SetY(mushY);
        m_sAnimations.m_pASprMushIdle->Draw(renderer, m_bFlipHorizontally, false);
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

    m_sAnimations.m_pASprMushWalk = renderer.CreateAnimatedSprite("..\\Sprites\\mushroom\\anim8mushrun.png");

    if (!(m_sAnimations.m_pASprMushWalk))
    {
        LogManager::GetInstance().Log("Mushroom run animation failed to initialise!");
        return false;
    }
    else
    {
        m_sAnimations.m_pASprMushWalk->SetupFrames(80, 64);
        m_sAnimations.m_pASprMushWalk->SetFrameDuration(0.1f);
        m_sAnimations.m_pASprMushWalk->SetScale(2.5f);
    }

    return true;
}

void
Mushroom::Move(void)
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
Mushroom::Action()
{
    if (m_fDistToPlayer < m_fAttackRangeMax && m_fDistToPlayer >= 0.0f)
    {
        if (!m_bRun)
        {
            m_sAnimations.m_pASprMushWalk->SetLooping(false);
            m_sAnimations.m_pASprMushWalk->Inanimate();
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
    if (m_bAttack)
    {
        if (m_pHeadButt == nullptr)
        {
            CreateHeadButt();
        }

    }
    else
    {
        if (m_pHeadButt != nullptr)
        {
            DeleteHeadButt();
        }
    }
}

void Mushroom::DeleteBody()
{
    if (m_pBody != nullptr)
    {
        m_pWorld->DestroyBody(m_pBody);
        m_pBody = nullptr;
    }
}

void Mushroom::CreateHeadButt()
{
    // Define the body
    b2BodyDef HeadbodyDef;
    HeadbodyDef.type = b2_staticBody;
    if (m_iFacingDirection == -1)
    {
        HeadbodyDef.position.Set((m_pBody->GetPosition().x + 0.5f), m_pBody->GetPosition().y);
    }
    else
    {
        HeadbodyDef.position.Set((m_pBody->GetPosition().x - 0.5f), m_pBody->GetPosition().y);
    }

    // Create the body in the world
    m_pHeadButt = m_pWorld->CreateBody(&HeadbodyDef);

    // Define the shape of the body (box shape in this example)
    b2PolygonShape staticBox;

    staticBox.SetAsBox(40/SCALE, 32/SCALE);

    // Define the fixture (physical properties)
    b2FixtureDef HeadfixtureDef;
    HeadfixtureDef.shape = &staticBox;
    HeadfixtureDef.density = 0.0f;
    HeadfixtureDef.friction = 0.0f;
    HeadfixtureDef.isSensor = true;
   
    // Attach the fixture to the body
    m_pHeadButt->CreateFixture(&HeadfixtureDef);

    userData* headData = new userData{ MUSHROOM_HEAD, static_cast<void*>(this) };
    m_pHeadButt->SetUserData(static_cast<void*>(headData));

}

void Mushroom::DeleteHeadButt()
{
    if (m_pHeadButt != nullptr)
    {
        m_pWorld->DestroyBody(m_pHeadButt);
        m_pHeadButt = nullptr;
    }
}

void
Mushroom::CheckPlayerDist()
{
    // Get player position using Box2D body - in meters
    b2Vec2 playerPosition = m_pEntCharacter->GetPosition();

    // Get Golem's current position - in meters
    b2Vec2 mushPosition = m_pBody->GetPosition();

    // Calculate the differences in the x and y coordinates
    m_fDistToPlayer = abs(playerPosition.x - mushPosition.x);
}

int
Mushroom::GetBodyWidth()
{
    return (int)(m_fHitBoxRange * 2);
}

void Mushroom::SetCamera(Camera* camera)
{
    m_pCamera = camera;
    IsCameraSet = true;
}

void Mushroom::SetPlayer(Player* player)
{
    m_pEntCharacter = player;
}

//Changes made by Kyle
Healthbar* Mushroom::getEnemyHealth()
{
    return healthBar;
}

//void
//Mushroom::DebugDraw()
//{
//
//}