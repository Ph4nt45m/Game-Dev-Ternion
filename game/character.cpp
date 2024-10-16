// This include:
#include "character.h"

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

// Library includes:
#include <cassert>
#include <cstdio>

//Box2D world
#define SCALE 30.0f

Character::Character(b2World* world)
    : m_pSprSpriteHead(0)
    , m_pSprSpriteLegLeft(0)
    , m_pSprSpriteLegRight(0)
    , m_pSprSpriteShadow(0)
    , m_pSprWeapon(0)
    , m_pEntArrow(0)
    , m_pASprWeapAttack(0)
    , m_iWeaponType(0)
    , m_pHealthbar(0)
    , m_fAngleOfAttack(0.0f)
    , m_bDoubleJump(false)
    , m_fHeadBodyOffset(0.0f)
    , m_fLengthFootToBody(0.0f)
    , m_fLegBodyOffset(0.0f)
    , m_fStepTimer(0.0f)
    , m_fStepDuration(0.0f)
    , m_pWorld(world)
    , m_pBody(nullptr)
    , m_jumpTimer(0.0f)
{

}

Character::~Character()
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

    m_pWorld->DestroyBody(m_pBody);
}

bool Character::Initialise(Renderer& renderer)
{
    if (!SetBodySprites(renderer))
    {
        LogManager::GetInstance().Log("Character Sprites failed to initialise!");
        return false;
    }

    m_iFacingDirection = 1;
    m_fScale = 1.0f;
    m_iWeaponType = 1;
    m_bAlive = true;

    Entity::SetWindowBoundaries(renderer);

    m_vPosition.x = 50.0f;  // Position in pixels
    m_vPosition.y = 500.0f; // Position in pixels

    // Weapon initialization based on type
    switch (m_iWeaponType)
    {
    case 0:
        m_pSprWeapon = renderer.CreateSprite("Sprites\\weaponsstatic\\sword.png");
        m_pASprWeapAttack = renderer.CreateAnimatedSprite("Sprites\\weaponsanim8\\anim8sword.png");
        if (m_pSprWeapon && m_pASprWeapAttack)
        {
            m_pASprWeapAttack->SetupFrames(213, 150);
            m_pASprWeapAttack->SetFrameDuration(0.02f);
        }
        else
        {
            LogManager::GetInstance().Log("Sword Weapon failed to initialise!");
            return false;
        }
        break;
    case 1:
        m_pSprWeapon = renderer.CreateSprite("Sprites\\weaponsstatic\\bow.png");
        m_pASprWeapAttack = renderer.CreateAnimatedSprite("Sprites\\weaponsanim8\\anim8bow.png");
        m_pEntArrow = new Projectile();
        if (m_pSprWeapon && m_pASprWeapAttack)
        {
            m_pASprWeapAttack->SetupFrames(161, 119);
            m_pASprWeapAttack->SetFrameDuration(0.03f);
        }
        else
        {
            LogManager::GetInstance().Log("Bow Weapon failed to initialise!");
            return false;
        }

        if (!m_pEntArrow->Initialise(renderer))
        {
            LogManager::GetInstance().Log("Projectile Arrow failed to initialise!");
            return false;
        }
        else
        {
            m_pEntArrow->SetProjectileSprite(renderer, "Sprites\\characterprojectile\\arrow.png");
            m_pEntArrow->SetGroundY(sm_fBoundaryHeight);
        }
        break;
    case 2:
        m_pSprWeapon = renderer.CreateSprite("Sprites\\weaponsstatic\\staff.png");
        m_pASprWeapAttack = renderer.CreateAnimatedSprite("Sprites\\weaponsanim8\\anim8staff.png");
        if (m_pSprWeapon && m_pASprWeapAttack)
        {
            m_pASprWeapAttack->SetupFrames(67, 119);
            m_pASprWeapAttack->SetFrameDuration(0.15f);
        }
        else
        {
            LogManager::GetInstance().Log("Staff Weapon failed to initialise!");
            return false;
        }
        break;
    }

    // Initialize healthbar
    m_pHealthbar = new Healthbar(renderer, 100.0f);

    // Box2D Body Initialization (Changes made by Rauen)

    // Create the Box2D body definition
    //b2BodyDef bodyDef;
    //bodyDef.type = b2_dynamicBody;
    //bodyDef.position.Set(m_vPosition.x / SCALE, m_vPosition.y / SCALE);  // Convert from pixels to meters

    //// Create the Box2D body in the world
    //m_pBody = m_pWorld->CreateBody(&bodyDef);

    //// Define the character's shape as a box (in meters)
    //b2PolygonShape characterBox;
    //float boxWidth = (m_pSprSpriteBody->GetWidth() / 2.0f) / SCALE;   // Convert pixel width to meters (half-width for Box2D)
    //float boxHeight = (m_pSprSpriteBody->GetHeight() / 2.0f) / SCALE; // Convert pixel height to meters (half-height for Box2D)
    //characterBox.SetAsBox(boxWidth, boxHeight);

    //// Create a fixture for the body (set density, friction, etc.)
    //b2FixtureDef fixtureDef;
    //fixtureDef.shape = &characterBox;
    //fixtureDef.density = 1.0f;
    //fixtureDef.friction = 0.3f;

    //// Attach the fixture to the body
    //m_pBody->CreateFixture(&fixtureDef);

    //// Set user data for collision detection
    //m_pBody->SetUserData((void*)PLAYER);

    return true;
}

void Character::Process(float deltaTime, InputSystem& inputSystem)
{
    // Handle user input for movement
    HandleInput(deltaTime, inputSystem);

    // Get the current Box2D body position (in meters) and convert it back to pixels for rendering
    b2Vec2 bodyPosition = m_pBody->GetPosition();

    // Update the character's position in pixels (scaling from meters)
    m_vPosition.x = bodyPosition.x * SCALE;
    m_vPosition.y = bodyPosition.y * SCALE;

    // Set facing orientation based on mouse cursor position
    m_vCursor.SetPosition(inputSystem.GetMousePosition());
    if (m_vPosition.x < m_vCursor.x)
    {
        m_iFacingDirection = 1;
        m_bFlipHorizontally = false;
    }
    else if (m_vPosition.x > m_vCursor.x)
    {
        m_iFacingDirection = -1;
        m_bFlipHorizontally = true;
    }

    // Set the sprite's position to match the Box2D body position
    m_pSprSpriteBody->SetX((int)m_vPosition.x);
    m_pSprSpriteBody->SetY((int)m_vPosition.y);

    // Scale the sprite based on camera distance or other factors (you can adjust the scaling factor)
    m_pSprSpriteBody->SetScale(m_fScale);

    // Process health bar updates (ensure health bar reflects player's current status)
    if (m_pHealthbar)
    {
        m_pHealthbar->Process(deltaTime, inputSystem);
    }


}

void Character::DrawWithCam(Renderer& renderer, Camera& camera)
{
    // Get the camera offset
    Vector2* cameraOffset = camera.GetOffset();

    // Adjust the character position based on the camera offset
    int adjustedX = m_pSprSpriteBody->GetX() - cameraOffset->x;

    // Update sprite position
    m_pSprSpriteBody->SetX(adjustedX);

    // Draw character sprite with adjusted position
    m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, false);

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


void
Character::GetInputs(InputSystem& inputSystem)
{
    // Gets movement keys' states
    m_sMotionKeyStates.MoveForward = inputSystem.GetKeyState(SDL_SCANCODE_D);
    m_sMotionKeyStates.MoveBackward = inputSystem.GetKeyState(SDL_SCANCODE_A);
    m_sMotionKeyStates.MoveUp = inputSystem.GetKeyState(SDL_SCANCODE_W); // For future updates
    m_sMotionKeyStates.MoveDown = inputSystem.GetKeyState(SDL_SCANCODE_S); // For future updates
    m_sMotionKeyStates.Jump = inputSystem.GetKeyState(SDL_SCANCODE_SPACE);
    m_sMotionKeyStates.LeftClickAttack = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT);

    if (m_sMotionKeyStates.Jump == BS_PRESSED)
    {
        m_sKeyboardMotions.Heave = MOTION_JUMP;
    }

    if (m_sMotionKeyStates.LeftClickAttack == BS_PRESSED)
    {
        m_sKeyboardMotions.Attack = MOTION_ATTACK;
    }
    else
    {
        m_sKeyboardMotions.Attack = MOTION_NONE;
    }

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

    if (m_sMotionKeyStates.MoveUp == BS_PRESSED || m_sMotionKeyStates.MoveUp == BS_HELD)
    {
        m_sKeyboardMotions.Sway = MOTION_UP;
    }
    else if (m_sMotionKeyStates.MoveDown == BS_PRESSED || m_sMotionKeyStates.MoveDown == BS_HELD)
    {
        m_sKeyboardMotions.Sway = MOTION_DOWN;
    }
    else
    {
        m_sKeyboardMotions.Sway = MOTION_NONE;
    }
}


void Character::HandleInput(float deltaTime, InputSystem& inputSystem)
{
    b2Vec2 velocity = m_pBody->GetLinearVelocity();

    // Move right when pressing D
    if (inputSystem.GetKeyState(SDL_SCANCODE_D) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_D) == BS_HELD) {
        velocity.x = 1.0f;  // Set a fixed speed to move right

    }
    else if (inputSystem.GetKeyState(SDL_SCANCODE_A) == BS_PRESSED || inputSystem.GetKeyState(SDL_SCANCODE_A) == BS_HELD) {
        velocity.x = -1.0f;  // Set a fixed speed to move left
    }
    else {
        velocity.x = 0.0f;  // No horizontal movement
    }

    // Jumping logic
    if (inputSystem.GetKeyState(SDL_SCANCODE_SPACE) == BS_PRESSED) {
        if (!m_bJumping) {
            // First jump
            velocity.y = -5.0f;  // Apply upward force
            m_bJumping = true;        // Character is now jumping
        }
        else if (m_bJumping && !m_bDoubleJump) {
            // Double jump
            velocity.y = -5.0f;  // Apply upward force
            m_bDoubleJump = true;     // Double jump has been used
        }
    }


    // If the player is jumping, update the jump timer
    if (m_bJumping) {
        m_jumpTimer += deltaTime;
        if (m_jumpTimer >= 3.0f) {
            m_bJumping = false;  // Reset jump
            m_bDoubleJump = false;
            m_jumpTimer = 0.0f;  // Reset the timer
        }
    }

    m_pBody->SetLinearVelocity(velocity);

    // Get the body's position in Box2D
    b2Vec2 bodyPos = m_pBody->GetPosition();
    // Update sprite position based on Box2D body position, applying the scale factor
    float spriteX = bodyPos.x * SCALE;  // Convert from Box2D meters to pixels
    float spriteY = bodyPos.y * SCALE;  // Convert from Box2D meters to pixels


    // Apply the calculated position to the sprite
    m_pSprSpriteBody->SetX(static_cast<int>(spriteX));
    m_pSprSpriteBody->SetY(static_cast<int>(spriteY));

}


bool
Character::SetBodySprites(Renderer& renderer)
{   // Change made by Karl - Start
    m_pSprSpriteHead = renderer.CreateSprite("Sprites\\characterbase\\head.png");

    if (!(m_pSprSpriteHead))
    {
        LogManager::GetInstance().Log("Character Head failed to initialise!");
        return false;
    }

    m_pSprSpriteBody = renderer.CreateSprite("Sprites\\characterbase\\body.png");

    if (!(m_pSprSpriteBody))
    {
        LogManager::GetInstance().Log("Character Body failed to initialise!");
        return false;
    }

    m_pSprSpriteLegLeft = renderer.CreateSprite("Sprites\\characterbase\\leg.png");

    if (!(m_pSprSpriteLegLeft))
    {
        LogManager::GetInstance().Log("Character Left Leg failed to initialise!");
        return false;
    }

    m_pSprSpriteLegRight = renderer.CreateSprite("Sprites\\characterbase\\leg.png");

    if (!(m_pSprSpriteLegRight))
    {
        LogManager::GetInstance().Log("Character Right Leg failed to initialise!");
        return false;
    }

    m_pSprSpriteShadow = renderer.CreateSprite("Sprites\\characterbase\\floorshadow.png");
    // Change made by Karl - End
    if (!(m_pSprSpriteShadow))
    {
        LogManager::GetInstance().Log("Character Shadow failed to initialise!");
        return false;
    }

    m_pSprSpriteBody->SetAngle(180.0f);
    m_pSprSpriteHead->SetAngle(180.0f);
    m_pSprSpriteLegLeft->SetAngle(180.0f);
    m_pSprSpriteLegRight->SetAngle(180.0f);
    m_pSprSpriteShadow->SetAngle(180.0f);

    return true;
}

b2Vec2
Character::GetPosition()
{
    return m_pBody->GetPosition();
}

Vector2&
Character::GetVelocityBody()
{
    return m_velocityBody;
}

int
Character::GetWeapontype()
{
    return m_iWeaponType;
}

Vector2&
Character::GetProjectilePos()
{
    return m_pEntArrow->GetPosition();
}

float
Character::GetProjWidth()
{
    return m_pEntArrow->GetWidth();
}

float
Character::GetProjHeight()
{
    return m_pEntArrow->GetHeight();
}

bool
Character::IsProjAlive()
{
    return m_pEntArrow->IsAlive();
}

void
Character::SetProjAlive(bool alive)
{
    m_pEntArrow->SetAlive(alive);
}

void Character::Draw(Renderer& renderer, Camera& camera)
{}

//void
//Character::DebugDraw()
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