#include "DecreaseSoundEffectButton.h"

// Local Include
#include "game.h"
#include "renderer.h"
#include "logmanager.h"

// Libs
#include <utility>
#include <string>

DecreaseSoundButton::DecreaseSoundButton(float x, float y)
    : Button(x, y),
    m_buttonSpriteNormal(nullptr)
{
}

DecreaseSoundButton::~DecreaseSoundButton()
{
    delete m_buttonSpriteNormal;
    m_buttonSpriteNormal = nullptr;
}

bool DecreaseSoundButton::Initialise(Renderer& renderer)
{
    // Load the button sprites
    m_buttonSpriteNormal = renderer.CreateSprite("..\\Sprites\\Menus\\Settings\\lowerhigher.png");

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
    float finalScale = std::min(scaleX, scaleY) / -7.0f; // Adjust denominator for desired size

    // Apply scaling to sprites
    m_buttonSpriteNormal->SetScale(finalScale);

    // Set the size of the button interaction area (adjust scale for interaction zone)
    SetWidth(spriteWidth * finalScale);
    SetHeight(spriteHeight * finalScale);

    // Position the button (this assumes m_x and m_y are already set to the correct positions)
    m_buttonSpriteNormal->SetX(m_x);
    m_buttonSpriteNormal->SetY(m_y);

    // Ensure all sprites are loaded
    return (m_buttonSpriteNormal);
}

void DecreaseSoundButton::Update(float deltaTime, InputSystem& inputSystem)
{
    Button::Update(deltaTime, inputSystem);
    if (m_isPressed)
    {
        SceneManager::GetInstance().GetSounds()->loadSound("bounce", "..\\Sprites\\sounds\\Bounce-SoundBible.com-12678623.wav");
        SceneManager::GetInstance().GetSounds()->playSound("bounce", 0, SceneManager::GetInstance().getsoundEffectsVolume());
    }
    if (m_isReleased)
    {
        soundVolume();
    }
}

void DecreaseSoundButton::Draw(Renderer& renderer)
{
    m_buttonSpriteNormal->Draw(renderer, true, false);
}

void DecreaseSoundButton::soundVolume()
{
    // Decrease volume by 12
    int SoundVolume = SceneManager::GetInstance().GetSounds()->getSoundVolume("bounce") - 13;

    // Ensure it stays within the range [0, 100]
    SoundVolume = std::min(128, std::max(0, SoundVolume));

    SceneManager::GetInstance().setsoundEffectsVolume(SoundVolume);
}
