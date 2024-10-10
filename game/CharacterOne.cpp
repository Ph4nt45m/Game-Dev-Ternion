#include "CharacterOne.h"

// Local Include
#include "game.h"
#include "renderer.h"
#include "logmanager.h"

// Libs
#include <utility>
#include <string>

CharacterOne::CharacterOne(float x, float y)
    : Button(x, y),
    m_buttonSpriteNormal(nullptr),
    m_buttonSpriteHovered(nullptr)
{
}

CharacterOne::~CharacterOne()
{
    delete m_buttonSpriteNormal;
    m_buttonSpriteNormal = nullptr;

    delete m_buttonSpriteHovered;
    m_buttonSpriteHovered = nullptr;
}

bool CharacterOne::Initialise(Renderer& renderer)
{
    // Load the button sprites
    m_buttonSpriteNormal = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\warrior\\anim8waridle.png");
    m_buttonSpriteHovered = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\warrior\\anim8warrun.png");
    m_buttonSpriteNormal->SetupFrames(212, 124);
    m_buttonSpriteHovered->SetupFrames(212, 124);
    m_buttonSpriteNormal->SetFrameDuration(0.15f);
    m_buttonSpriteHovered->SetFrameDuration(0.15f);
    m_buttonSpriteNormal->Animate();
    m_buttonSpriteNormal->SetLooping(true);

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
    m_buttonSpriteHovered->SetScale(finalScale);

    // Set the size of the button interaction area (adjust scale for interaction zone)
    SetWidth(spriteWidth * finalScale);
    SetHeight(spriteHeight * finalScale);

    // Position the button (this assumes m_x and m_y are already set to the correct positions)
    m_buttonSpriteNormal->SetX(m_x);
    m_buttonSpriteNormal->SetY(m_y);
    m_buttonSpriteHovered->SetX(m_x);
    m_buttonSpriteHovered->SetY(m_y);

    // Ensure all sprites are loaded
    return (m_buttonSpriteNormal && m_buttonSpriteHovered);
}

void CharacterOne::Update(float deltaTime, InputSystem& inputSystem)
{
    Button::Update(deltaTime, inputSystem);

    if (m_isReleased)
    {
        SceneManager::GetInstance().ChangeScene(4);
    }
    else if (m_isHovered)
    {
        if (m_buttonSpriteNormal->IsAnimating())
        {
            m_buttonSpriteNormal->Inanimate();
            m_buttonSpriteNormal->SetLooping(false);
        }
        
        if (!m_buttonSpriteHovered->IsAnimating())
        {
            m_buttonSpriteHovered->Animate();
            m_buttonSpriteHovered->SetLooping(true);
        }
    }
    else
    {
        if (!m_buttonSpriteNormal->IsAnimating())
        {
            m_buttonSpriteNormal->Animate();
            m_buttonSpriteNormal->SetLooping(true);
        }

        if (m_buttonSpriteHovered->IsAnimating())
        {
            m_buttonSpriteHovered->Inanimate();
            m_buttonSpriteHovered->SetLooping(false);
        }
    }

    m_buttonSpriteNormal->Process(deltaTime);
    m_buttonSpriteHovered->Process(deltaTime);
}

void CharacterOne::Draw(Renderer& renderer)
{
    if (m_isHovered)
    {
        m_buttonSpriteHovered->Draw(renderer, true, true);
    }
    else
    {
        m_buttonSpriteNormal->Draw(renderer, true, true);
    }
}