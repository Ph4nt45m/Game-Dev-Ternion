// This include:
#include "Warrior.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "inputsystem.h"
#include "../imgui/imgui.h"
#include "animatedsprite.h"
#include "projectile.h"
#include "healthbar.h"
#include "game.h"
#include "MyContactListener.h"
#include "sceneManager.h"

// Library includes:
#include <cassert>
#include <cstdio>

//Box2D world

Warrior::Warrior(b2World* world)
    : m_pSprSpriteHead(0)
    , m_pSprSpriteLegLeft(0)
    , m_pSprSpriteLegRight(0)
    , m_pSprSpriteShadow(0)
    , m_pSprWeapon(0)
    , m_pEntArrow(0)
    , m_pASprWeapAttack(0)
    , m_iCharacterType(0)
    , m_pHealthbar(0)
    , m_fAngleOfAttack(0.0f)
    , m_bDefined(false)
    , m_fHeadBodyOffset(0.0f)
    , m_fLengthFootToBody(0.0f)
    , m_fLegBodyOffset(0.0f)
    , m_fElapsedTime(0.0f)
    , m_fStepTimer(0.0f)
    , m_fStepDuration(0.0f)
    , m_pBody(nullptr)
    , m_pWorld(world)
    , m_pSPAttackBody(nullptr) // Changes made by Karl
    , m_jumpTimer(0.0f)
    , m_sActions{ 0, 0, 0, 0, 0 }
    , m_fOffset(0.0f)
    , m_fPlayerWidth(0.0f) // Changes made by Karl
    , m_fPlayerHeight(0.0f)
    , m_fAttackWidth(0.0f)
    , m_fAttackHeight(0.0f)
{

}

Warrior::~Warrior()
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

    delete m_pHealthbar;
    m_pHealthbar = 0;

    delete m_sActions.m_pASpriteIdle;
    m_sActions.m_pASpriteIdle = 0;

    delete m_sActions.m_pASpriteRun;
    m_sActions.m_pASpriteRun = 0;

    delete m_sActions.m_pASpriteJump;
    m_sActions.m_pASpriteJump = 0;

    delete m_sActions.m_pASpriteAttack;
    m_sActions.m_pASpriteAttack = 0;

    delete m_sActions.m_pASpriteDeath;
    m_sActions.m_pASpriteDeath = nullptr;

    delete m_pStaticDeath;
    m_pStaticDeath = nullptr;
    // Changes made by Karl
    if (m_pBody != nullptr)
    {
        m_pWorld->DestroyBody(m_pBody);
        m_pBody = nullptr;
    }
    if (m_pSPAttackBody != nullptr)
    {
        m_pWorld->DestroyBody(m_pSPAttackBody);
        m_pSPAttackBody = nullptr;
    }
}

bool Warrior::Initialise(Renderer& renderer)
{
    m_iFacingDirection = 1;
    m_fScale = 1.0f; // Changes made by Karl
    m_bAlive = true;

    Entity::SetWindowBoundaries(renderer);
    // Changes made by Karl
    m_vPosition.x = 100.0f;  // Position in pixels
    m_vPosition.y = 0.0f; // Position in pixels
    m_fPlayerWidth = 56.0f; // Changes made by Karl
    m_fPlayerHeight = 120.0f;
    m_fOffset = 82.0f; // Y offset in pixels

    return true;
}

void Warrior::Process(float deltaTime, InputSystem& inputSystem)
{
    if (!m_bAlive)
    {
        if (!m_bAnimateDeath)
        {
            m_sActions.m_pASpriteDeath->Animate();
            m_bAnimateDeath = true;
            SceneManager::GetInstance().ChangeScene(6);
        }
        else if (m_sActions.m_pASpriteDeath->IsAnimating())
        {
            m_sActions.m_pASpriteDeath->Process(deltaTime);
        }
        
        return;
    }

    // Handle user input for movement
    HandleInput(deltaTime, inputSystem);
    ProcessActions(deltaTime); // Changes made by Karl

    // Get the current Box2D body position (in meters) and convert it back to pixels for rendering
    b2Vec2 bodyPosition = m_pBody->GetPosition();

    // Update the character's position in pixels (scaling from meters)
    m_vPosition.x = bodyPosition.x * SCALE;
    m_vPosition.y = bodyPosition.y * SCALE;

    // Set facing orientation based on mouse cursor position
    /*m_vCursor.SetPosition(inputSystem.GetMousePosition());
    if (m_vPosition.x < m_vCursor.x)
    {
        m_iFacingDirection = 1;
        m_bFlipHorizontally = false;
    }
    else if (m_vPosition.x > m_vCursor.x)
    {
        m_iFacingDirection = -1;
        m_bFlipHorizontally = true;
    }*/
    
    // Set the sprite's position to match the Box2D body position
    m_sActions.m_pASpriteIdle->SetX((int)m_vPosition.x);
    m_sActions.m_pASpriteIdle->SetY((int)m_vPosition.y - m_fOffset);
    m_sActions.m_pASpriteRun->SetX((int)m_vPosition.x);
    m_sActions.m_pASpriteRun->SetY((int)m_vPosition.y - m_fOffset);
    m_sActions.m_pASpriteJump->SetX((int)m_vPosition.x);
    m_sActions.m_pASpriteJump->SetY((int)m_vPosition.y - m_fOffset);
    m_sActions.m_pASpriteAttack->SetX((int)m_vPosition.x); // Changes made by Karl
    m_sActions.m_pASpriteAttack->SetY((int)m_vPosition.y - m_fOffset);

    m_sActions.m_pASpriteIdle->Process(deltaTime);
    m_sActions.m_pASpriteRun->Process(deltaTime);
    m_sActions.m_pASpriteJump->Process(deltaTime);
    m_sActions.m_pASpriteAttack->Process(deltaTime); // Changes made by Karl

    // Scale the sprite based on camera distance or other factors (you can adjust the scaling factor)
    //m_pSprSpriteBody->SetScale(m_fScale);

    // Process health bar updates (ensure health bar reflects player's current status)
    if (m_pHealthbar)
    {
        m_pHealthbar->Process(deltaTime, inputSystem);
    }

    if (getPlayerHealthbar()->GetCurrentHealth() <= 0)
    {
        m_bAlive = false;
    }
}

void Warrior::DrawWithCam(Renderer& renderer, Camera& camera)
{
    if (m_bAlive)
    {
        // Get the camera offset
        Vector2* cameraOffset = camera.GetOffset();

        // Adjust the character position based on the camera offset
        int adjustedX = m_sActions.m_pASpriteIdle->GetX() - cameraOffset->x;

        // Update sprite position
        m_sActions.m_pASpriteIdle->SetX(adjustedX);
        m_sActions.m_pASpriteRun->SetX(adjustedX);
        m_sActions.m_pASpriteJump->SetX(adjustedX);
        m_sActions.m_pASpriteAttack->SetX(adjustedX); // Changes made by Karl

        // Draw character sprite with adjusted position
        if (m_bMovingX || !m_bJumping || !m_bDoubleJump || m_bSlash) // Changes made by Karl - Reversed logic for Kyle's toggle in contact listener
        {
            if (!m_bJumping || !m_bDoubleJump) // Changes made by Karl - Reversed logic for Kyle's toggle in contact listener
            {
                m_sActions.m_pASpriteJump->Draw(renderer, m_bFlipHorizontally, false);
            }
            else if (m_bSlash)
            {
                m_sActions.m_pASpriteAttack->Draw(renderer, m_bFlipHorizontally, false);
            }
            else if (m_bMovingX)
            {
                m_sActions.m_pASpriteRun->Draw(renderer, m_bFlipHorizontally, false);
            }
        }
        else
        {
            m_sActions.m_pASpriteIdle->Draw(renderer, m_bFlipHorizontally, false);
        }

        //// Draw weapon
        //if (m_pASprWeapAttack->IsAnimating())
        //{
        //    m_pASprWeapAttack->SetX(adjustedX);
        //    m_pASprWeapAttack->SetY(adjustedY - 15);
        //    m_pASprWeapAttack->Draw(renderer, m_bFlipHorizontally, false);
        //}
        //else
        //{
        //    m_pSprWeapon->SetX(adjustedX);
        //    m_pSprWeapon->SetY(adjustedY - 15);
        //    m_pSprWeapon->Draw(renderer, m_bFlipHorizontally, true);
        //}

        // If the character has a health bar, draw it adjusted as well
        if (m_pHealthbar)
        {
            m_pHealthbar->Draw(renderer);
        }
    }
    else if (m_sActions.m_pASpriteDeath->IsAnimating())
    {
        m_sActions.m_pASpriteDeath->Draw(renderer, false, false);
    }
    else
    {
        m_pStaticDeath->Draw(renderer, false, true);
    }
}
// Changes made by Karl
void
Warrior::HandleInput(float deltaTime, InputSystem& inputSystem)
{
    b2Vec2 velocity = m_pBody->GetLinearVelocity();

    // Toggle godmode
    if (inputSystem.GetKeyState(SDL_SCANCODE_L) == BS_PRESSED)
    {
        m_bGodmode = !m_bGodmode;
    }

    // Move right when pressing D
    if (inputSystem.GetKeyState(SDL_SCANCODE_D) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_D) == BS_HELD)
    {
        if (!m_bSlash)
        {
            velocity.x = 0.5f;  // Set a fixed speed to move right

            if (!m_bMovingX)
            {
                if (m_sActions.m_pASpriteIdle->IsAnimating())
                {
                    m_sActions.m_pASpriteIdle->Inanimate();
                    m_sActions.m_pASpriteIdle->SetLooping(false);
                }

                if (!m_sActions.m_pASpriteRun->IsAnimating())
                {
                    m_sActions.m_pASpriteRun->Animate();
                    m_sActions.m_pASpriteRun->SetLooping(true);
                }

                m_bMovingX = true;
                m_iFacingDirection = 1;
                m_bFlipHorizontally = false;
            }
        } // Changes made by Karl
    }
    else if (inputSystem.GetKeyState(SDL_SCANCODE_A) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_A) == BS_HELD)
    {
        if (!m_bSlash)
        {
            velocity.x = -0.5f;  // Set a fixed speed to move left

            if (!m_bMovingX)
            {
                if (m_sActions.m_pASpriteIdle->IsAnimating())
                {
                    m_sActions.m_pASpriteIdle->Inanimate();
                    m_sActions.m_pASpriteIdle->SetLooping(false);
                }

                if (!m_sActions.m_pASpriteRun->IsAnimating())
                {
                    m_sActions.m_pASpriteRun->Animate();
                    m_sActions.m_pASpriteRun->SetLooping(true);
                }

                m_bMovingX = true;
                m_iFacingDirection = -1;
                m_bFlipHorizontally = true;
            }
        } // Changes made by Karl
    }
    else
    {
        velocity.x = 0.0f;  // No horizontal movement

        if (m_bMovingX)
        {
            if (m_sActions.m_pASpriteRun->IsAnimating())
            {
                m_sActions.m_pASpriteRun->Inanimate();
                m_sActions.m_pASpriteRun->SetLooping(false);
            }

            if (!m_sActions.m_pASpriteIdle->IsAnimating())
            {
                m_sActions.m_pASpriteIdle->Animate();
                m_sActions.m_pASpriteIdle->SetLooping(true);
            }

            m_bMovingX = false;
        }
    }
    // Changes made by Karl
    // Attack logic
    if (inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT) == BS_PRESSED)
    {   // Allow attack only when on the ground
        if (m_bJumping && m_bDoubleJump && !m_bSlash) // Changes made by Karl - Reversed logic for Kyle's toggle in contact listener
        {   // Changes made by Karl
            m_bSlash = true;
            SceneManager::GetInstance().GetSounds()->loadSound("bounce", "..\\Sprites\\sounds\\soundEffects\\Flame Arrow-SoundBible.com-618067908.wav");
            SceneManager::GetInstance().GetSounds()->playSound("bounce", 0, SceneManager::GetInstance().getsoundEffectsVolume());

            // Disable all other animations
            if (m_sActions.m_pASpriteIdle->IsAnimating())
            {
                m_sActions.m_pASpriteIdle->Inanimate();
            }

            if (m_sActions.m_pASpriteRun)
            {
                m_sActions.m_pASpriteRun->Inanimate();
                velocity.x = 0.0f; // Changes made by Karl
            }

            // Enable attack animation
            if (!m_sActions.m_pASpriteAttack->IsAnimating())
            {
                m_sActions.m_pASpriteAttack->Animate();
                // Set sound effects here
            }
        }
    }

    if (!m_sActions.m_pASpriteAttack->IsAnimating())
    {
        m_bSlash = false;
    }

    // Jumping logic
    if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE) == BS_PRESSED)
    {
        if (m_bJumping && !m_bSlash) // Changes made by Karl - Reversed logic for Kyle's toggle in contact listener
        {
            // First jump
            velocity.y = -2.0f;  // Apply upward force
            m_bJumping = false;   // Character is now jumping // Changes made by Karl - Reversed logic for Kyle's toggle in contact listener
            
            if (m_sActions.m_pASpriteIdle->IsAnimating())
            {
                m_sActions.m_pASpriteIdle->Inanimate();
                m_sActions.m_pASpriteIdle->SetLooping(false);
            }

            if (m_sActions.m_pASpriteRun->IsAnimating())
            {
                m_sActions.m_pASpriteRun->Inanimate();
                m_sActions.m_pASpriteRun->SetLooping(false);
            }

            if (!m_sActions.m_pASpriteJump->IsAnimating())
            {
                m_sActions.m_pASpriteJump->Animate();
            }
        }
        else if (!m_bJumping && m_bDoubleJump) {
            // Double jump
            velocity.y = -2.0f;  // Apply upward force
            m_bDoubleJump = false;     // Double jump has been used // Changes made by Karl - Reversed logic for Kyle's toggle in contact listener
            m_sActions.m_pASpriteJump->Restart();
            m_sActions.m_pASpriteJump->Animate();
        }
    }

    // If the player is jumping, update the jump timer
    if (!m_bJumping || !m_bDoubleJump) { // Changes made by Karl - Reversed logic for Kyle's toggle in contact listener
        m_jumpTimer += deltaTime;
        if (m_jumpTimer >= (!m_bDoubleJump ? 1.2f : 0.8f)) {
            m_jumpTimer = 0.0f;  // Reset the timer
            // Changes made by Karl - Disable jump animation, enable animation based on current input
            if (m_sActions.m_pASpriteJump->IsAnimating())
            {
                m_sActions.m_pASpriteJump->Inanimate();
            }

            if (!m_bMovingX)
            {
                m_sActions.m_pASpriteIdle->Animate();
                m_sActions.m_pASpriteIdle->SetLooping(true);
            }
            else if (!m_sActions.m_pASpriteRun->IsAnimating())
            {
                m_sActions.m_pASpriteRun->Animate();
            }
        }
    }

    m_pBody->SetLinearVelocity(velocity);

    // Get the body's position in Box2D
    b2Vec2 bodyPos = m_pBody->GetPosition();
    // Update sprite position based on Box2D body position, applying the scale factor
    float spriteX = bodyPos.x * SCALE;  // Convert from Box2D meters to pixels
    float spriteY = bodyPos.y * SCALE;  // Convert from Box2D meters to pixels


    // Apply the calculated position to the sprite
    m_sActions.m_pASpriteIdle->SetX(static_cast<int>(spriteX));
    m_sActions.m_pASpriteIdle->SetY(static_cast<int>(spriteY));
    m_sActions.m_pASpriteRun->SetX(static_cast<int>(spriteX));
    m_sActions.m_pASpriteRun->SetY(static_cast<int>(spriteY));
    m_sActions.m_pASpriteJump->SetX(static_cast<int>(spriteX));
    m_sActions.m_pASpriteJump->SetY(static_cast<int>(spriteY) - m_fOffset);
}
// Changes made by Karl
void
Warrior::ProcessActions(float deltaTime)
{
    // Check if attack 
    if (m_bSlash)
    {
        m_fElapsedTime += deltaTime;

        if (m_fElapsedTime >= 0.7f)
        {
            if (m_pSPAttackBody == nullptr)
            {
                CreateSPAttack();
            }
        }
    }

    // Check if attack animation has finished
    if (!m_bSlash)
    {
        if (m_pSPAttackBody != nullptr)
        {
            DeleteSPAttack();
            m_fElapsedTime = 0.0f;
        }
    }
}

bool
Warrior::SetBodySprites(Renderer& renderer)
{   // Changes made by Karl
    m_sActions.m_pASpriteIdle = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\warrior\\anim8waridle.png");

    if (!m_sActions.m_pASpriteIdle)
    {
        LogManager::GetInstance().Log("Player idle failed to initialise!");
        return false;
    }
    else
    {
        m_sActions.m_pASpriteIdle->SetupFrames(515, 286);
        m_sActions.m_pASpriteIdle->SetFrameDuration(0.15f);
        m_sActions.m_pASpriteIdle->Animate();
        m_sActions.m_pASpriteIdle->SetLooping(true);
    }

    m_sActions.m_pASpriteRun = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\warrior\\anim8warrun.png");

    if (!m_sActions.m_pASpriteRun)
    {
        LogManager::GetInstance().Log("Player run failed to initialise!");
        return false;
    }
    else
    {
        m_sActions.m_pASpriteRun->SetupFrames(515, 286);
        m_sActions.m_pASpriteRun->SetFrameDuration(0.1f);
    }

    m_sActions.m_pASpriteJump = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\warrior\\anim8warjump.png");

    if (!m_sActions.m_pASpriteJump)
    {
        LogManager::GetInstance().Log("Player jump failed to initialise!");
        return false;
    }
    else
    {
        m_sActions.m_pASpriteJump->SetupFrames(515, 286);
        m_sActions.m_pASpriteJump->SetFrameDuration(0.03f);
    }
    // Changes made by Karl
    m_sActions.m_pASpriteAttack = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\warrior\\anim8warattack.png");

    if (!m_sActions.m_pASpriteAttack)
    {
        LogManager::GetInstance().Log("Player attack failed to initialise!");
        return false;
    }
    else
    {
        m_sActions.m_pASpriteAttack->SetupFrames(515, 286);
        m_sActions.m_pASpriteAttack->SetFrameDuration(0.07f);
        m_fAttackWidth = 515 / SCALE;
        m_fAttackHeight = 286 / SCALE;
    }

    m_sActions.m_pASpriteDeath = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\warrior\\anim8wardeath.png");

    if (!m_sActions.m_pASpriteDeath)
    {
        LogManager::GetInstance().Log("Player death failed to initialise!");
        return false;
    }
    else
    {
        m_sActions.m_pASpriteDeath->SetupFrames(515, 286);
        m_sActions.m_pASpriteDeath->SetFrameDuration(0.15f);
        m_sActions.m_pASpriteDeath->SetX(sm_fBoundaryWidth * 0.5f);
        m_sActions.m_pASpriteDeath->SetY(sm_fBoundaryHeight * 0.59f);
    }

    m_pStaticDeath = renderer.CreateSprite("..\\Sprites\\characters\\warrior\\staticdeath.png");

    if (!m_pStaticDeath)
    {
        LogManager::GetInstance().Log("Player static death failed to initialise!");
        return false;
    }
    else
    {
        m_pStaticDeath->SetX(sm_fBoundaryWidth * 0.5f);
        m_pStaticDeath->SetY(sm_fBoundaryHeight * 0.59f);
    }

    return true;
}

b2Vec2
Warrior::GetPosition()
{
    return m_pBody->GetPosition();
}

Vector2&
Warrior::GetVelocityBody()
{
    return m_velocityBody;
}
// Changes made by Karl - Start
int
Warrior::GetCharactertype()
{
    return m_iCharacterType;
}

void
Warrior::SetCharacterType(Renderer& renderer, int type)
{
    m_iCharacterType = type;

    DefineCharacter(renderer);
}

void
Warrior::DefineCharacter(Renderer& renderer)
{
    if (!SetBodySprites(renderer))
    {
        LogManager::GetInstance().Log("Player animations failed to initialise!");
    }

    // Initialize healthbar
    m_pHealthbar = new Healthbar(renderer, 200.0f);

    // Box2D Body Initialization (Changes made by Rauen)

    // Create the Box2D body definition
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(m_vPosition.x / SCALE, m_vPosition.y / SCALE);  // Convert from pixels to meters

    // Create the Box2D body in the world
    m_pBody = m_pWorld->CreateBody(&bodyDef);

    // Define the character's shape as a box (in meters)
    b2PolygonShape characterBox; // Changes made by Karl
    float boxWidth = (m_fPlayerWidth / 2.0f) / SCALE;   // Convert pixel width to meters (half-width for Box2D)
    float boxHeight = (m_fPlayerHeight / 2.0f) / SCALE; // Convert pixel height to meters (half-height for Box2D)
    characterBox.SetAsBox(boxWidth, boxHeight);

    // Create a fixture for the body (set density, friction, etc.)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &characterBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    // Attach the fixture to the body
    m_pBody->CreateFixture(&fixtureDef);

    // Set user data for collision detection
    userData* warriorData = new userData{ PLAYER, static_cast<void*>(this) };
    m_pBody->SetUserData(static_cast<void*>(warriorData));
}

bool
Warrior::IsDefined()
{
    return m_bDefined;
}

void
Warrior::SetDefined(bool define)
{
    m_bDefined = define;
} // Changes made by Karl - End
// Changes made by Karl - Start
void
Warrior::CreateSPAttack()
{
    // Define the body
    b2BodyDef SpecialBodyDef;
    SpecialBodyDef.type = b2_staticBody;
    if (m_iFacingDirection == -1)
    {
        SpecialBodyDef.position.Set((m_pBody->GetPosition().x - 0.5f), m_pBody->GetPosition().y);
    }
    else
    {
        SpecialBodyDef.position.Set((m_pBody->GetPosition().x + 0.5f), m_pBody->GetPosition().y);
    }

    // Create the body in the world
    m_pSPAttackBody = m_pWorld->CreateBody(&SpecialBodyDef);

    // Define the shape of the body (box shape in this example)
    b2PolygonShape staticBox;
    float specialWidth = ((m_fPlayerWidth * 2.8f)) / SCALE;
    float specialHeight = ((m_fPlayerHeight * 1.5f)) / SCALE;

    staticBox.SetAsBox(specialWidth, specialHeight);

    // Define the fixture (physical properties)
    b2FixtureDef SlashfixtureDef;
    SlashfixtureDef.shape = &staticBox;
    SlashfixtureDef.density = 0.0f;
    SlashfixtureDef.friction = 0.0f;
    SlashfixtureDef.isSensor = true;

    // Attach the fixture to the body
    m_pSPAttackBody->CreateFixture(&SlashfixtureDef);
    m_pSPAttackBody->SetActive(true);

    // Set user data to identify this body as a Golem
    userData* attackData = new userData{ PLAYER_SP_ATTACK, static_cast<void*>(this) };
    m_pSPAttackBody->SetUserData(static_cast<void*>(attackData));
}

void
Warrior::DeleteSPAttack()
{
    if (m_pSPAttackBody != nullptr)
    {
        m_pWorld->DestroyBody(m_pSPAttackBody);
        m_pSPAttackBody = nullptr;
    }
}
// Changes made by Karl - End
Vector2&
Warrior::GetProjectilePos()
{
    return m_pEntArrow->GetPosition();
}

float
Warrior::GetProjWidth()
{
    return m_pEntArrow->GetWidth();
}

float
Warrior::GetProjHeight()
{
    return m_pEntArrow->GetHeight();
}

bool
Warrior::IsProjAlive()
{
    return m_pEntArrow->IsAlive();
}

void
Warrior::SetProjAlive(bool alive)
{
    m_pEntArrow->SetAlive(alive);
}

void Warrior::Draw(Renderer& renderer, Camera& camera)
{}

Healthbar* Warrior::getPlayerHealthbar()
{
    return m_pHealthbar;
}
void Warrior::DeleteBody()
{
    if (m_pBody != nullptr)
    {
        m_pWorld->DestroyBody(m_pBody);
        m_pBody = nullptr;
    }
}

void
Warrior::SetAlive(bool alive)
{
    m_bAlive = alive;
}

bool
Warrior::IsGodmode()
{
    return m_bGodmode;
}

void Warrior::setCharacterPos()
{
    m_vPosition.x = 110.0f;  // Position in pixels
    m_vPosition.y = 0.0f; // Position in pixels
    m_fPlayerWidth = 56.0f; // Changes made by Karl
    m_fPlayerHeight = 120.0f;
    m_fOffset = 82.0f; // Y offset in pixels
    if (m_pBody != nullptr)
    {
        b2Vec2 newPosition(m_vPosition.x / SCALE, m_vPosition.y);
        float currentAngle = m_pBody->GetAngle();

        m_pBody->SetTransform(newPosition, currentAngle);
    }

}

//void
//Warriorr::DebugDraw()
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
