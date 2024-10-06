#include "startButton.h"

// Local Include
#include "game.h"
#include "renderer.h"
#include "logmanager.h"

// Libs
#include <utility>
#include <string>

StartButton::StartButton(float x, float y)
    : Button(x, y),
    m_buttonSpriteNormal(nullptr),
    m_buttonSpriteHovered(nullptr),
    m_buttonSpritePressed(nullptr)
{
}

StartButton::~StartButton()
{
    delete m_buttonSpriteNormal;
    m_buttonSpriteNormal = nullptr;

    delete m_buttonSpriteHovered;
    m_buttonSpriteHovered = nullptr;

    delete m_buttonSpritePressed;
    m_buttonSpritePressed = nullptr;
}

bool StartButton::Initialise(Renderer& renderer)
{
    // Load the button sprites
    m_buttonSpriteNormal = renderer.CreateSprite("..\\Sprites\\Menus\\selectNormal.png");
    m_buttonSpriteHovered = renderer.CreateSprite("..\\Sprites\\Menus\\selectHovered.png");
    m_buttonSpritePressed = renderer.CreateSprite("..\\Sprites\\Menus\\selectPressed.png");

    // Get the screen dimensions
    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();

    // Calculate the scale factor based on the screen size and sprite dimensions
    float spriteWidth = static_cast<float>(m_buttonSpriteNormal->GetWidth());
    float spriteHeight = static_cast<float>(m_buttonSpriteNormal->GetHeight());

    // Get scale factors based on screen size
    float scaleX = windowWidth / spriteWidth; // Proportional to screen width
    float scaleY = windowHeight / spriteHeight; // Proportional to screen height

    // Decide on a final uniform scale factor (use the smaller of the two to maintain aspect ratio)
    float finalScale = std::min(scaleX, scaleY) / -5.0f; // Adjust denominator for desired size

    // Apply scaling to sprites
    m_buttonSpriteNormal->SetScale(finalScale);
    m_buttonSpritePressed->SetScale(finalScale);

    // Set the size of the button interaction area (adjust scale for interaction zone)
    SetWidth(spriteWidth * finalScale);
    SetHeight(spriteHeight * finalScale);

    // Position the button (this assumes m_x and m_y are already set to the correct positions)
    m_buttonSpriteNormal->SetX(m_x);
    m_buttonSpriteNormal->SetY(m_y);

    // Ensure all sprites are loaded
    return (m_buttonSpriteNormal && m_buttonSpriteHovered && m_buttonSpritePressed);
}

void StartButton::Update(float deltaTime, InputSystem& inputSystem)
{
    Button::Update(deltaTime, inputSystem);
    if (m_isReleased)
    {
        SceneManager::GetInstance().ChangeScene(2);
    }
}

void StartButton::Draw(Renderer& renderer)
{
    if (m_isHeld)
    {
        m_buttonSpritePressed->Draw(renderer, false, false);
    }
    else if (m_isHovered)
    {
        m_buttonSpriteHovered->Draw(renderer, false, false);
    }
    else
    {
        m_buttonSpriteNormal->Draw(renderer, false, false);
    }
}