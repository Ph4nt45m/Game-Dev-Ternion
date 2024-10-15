#include "CharacterThree.h"

// Local Include
#include "game.h"
#include "character.h" // Changes made by Karl
#include "renderer.h"
#include "logmanager.h"

// Libs
#include <utility>
#include <string>

CharacterThree::CharacterThree(float x, float y)
    : Button(x, y),
    m_buttonFrameNormal(nullptr),
    m_buttonFrameHovered(nullptr),
    m_buttonSpriteNormal(nullptr),
    m_buttonSpriteHovered(nullptr),
    m_fBlueTint(0.0f),
    m_iCharYOffset(0) // Changes made by Karl
{
}

CharacterThree::~CharacterThree()
{ 
    delete m_buttonFrameNormal;
    m_buttonFrameNormal = nullptr;
    
    delete m_buttonFrameHovered;
    m_buttonFrameHovered = nullptr;

    delete m_buttonSpriteNormal;
    m_buttonSpriteNormal = nullptr;

    delete m_buttonSpriteHovered;
    m_buttonSpriteHovered = nullptr;
}

bool CharacterThree::Initialise(Renderer& renderer)
{
    // Load the button sprites
    m_buttonFrameNormal = renderer.CreateSprite("..\\Sprites\\Menus\\characterselect\\archerframe.png");
    m_buttonFrameHovered = renderer.CreateSprite("..\\Sprites\\Menus\\characterselect\\framehighlight.png");
    m_buttonSpriteNormal = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\archer\\anim8archidle.png");
    m_buttonSpriteHovered = renderer.CreateAnimatedSprite("..\\Sprites\\characters\\archer\\anim8archrun.png");

    // Set sprite attributes
    m_buttonSpriteNormal->SetupFrames(515, 286);
    m_buttonSpriteHovered->SetupFrames(515, 286);
    m_buttonSpriteNormal->SetFrameDuration(0.15f);
    m_buttonSpriteHovered->SetFrameDuration(0.15f);
    m_buttonSpriteNormal->Animate();
    m_buttonSpriteNormal->SetLooping(true);
    // Changes made by Karl
    // Set Y offset
    m_iCharYOffset = 100;

    // Get the screen dimensions
    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();

    // Calculate the scale factor based on the screen size and sprite dimensions
    float spriteWidth = static_cast<float>(212);
    float spriteHeight = static_cast<float>(124);

    // Get scale factors based on screen size
    float scaleX = windowWidth / spriteWidth; // Proportional to screen width
    float scaleY = windowHeight / spriteHeight; // Proportional to screen height

    // Decide on a final uniform scale factor (use the smaller of the two to maintain aspect ratio)
    float finalScale = std::min(scaleX, scaleY) / -5.0f; // Adjust denominator for desired size

    // Apply scaling to sprites
    m_buttonFrameNormal->SetScale(finalScale);
    m_buttonFrameHovered->SetScale(finalScale);
    m_buttonSpriteNormal->SetScale(finalScale);
    m_buttonSpriteHovered->SetScale(finalScale);

    // Set the size of the button interaction area (adjust scale for interaction zone)
    SetWidth(spriteWidth * finalScale);
    SetHeight(spriteHeight * finalScale);

    // Position the button (this assumes m_x and m_y are already set to the correct positions)
    m_buttonFrameNormal->SetX(m_x);
    m_buttonFrameNormal->SetY(m_y);
    m_buttonFrameHovered->SetX(m_x);
    m_buttonFrameHovered->SetY(m_y);
    m_buttonSpriteNormal->SetX(m_x);
    m_buttonSpriteNormal->SetY(m_y - m_iCharYOffset); // Changes made by Karl
    m_buttonSpriteHovered->SetX(m_x);
    m_buttonSpriteHovered->SetY(m_y - m_iCharYOffset); // Changes made by Karl
    
    // Set initial blue tint values
    m_fBlueTint = 1.0f;

    // Ensure all sprites are loaded
    return (m_buttonSpriteNormal && m_buttonSpriteHovered);
}

void CharacterThree::Update(float deltaTime, InputSystem& inputSystem)
{
    Button::Update(deltaTime, inputSystem);

    if (m_isReleased)
    {
        Game::GetInstance().CreateCharacter(2); // Changes made by Karl - load level with archer
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
        
        if (m_fBlueTint > 0.0f)
        {
            m_fBlueTint -= 2.0f * deltaTime;
        }
        else {
            m_fBlueTint = 0.0f;
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
        
        if (m_fBlueTint < 1.0f)
        {
            m_fBlueTint += 2.0f * deltaTime;
        }
        else
        {
            m_fBlueTint = 1.0f;
        }
    }
    
    // Set blue tint for frame highlight
    m_buttonFrameHovered->SetBlueTint(m_fBlueTint);

    m_buttonSpriteNormal->Process(deltaTime);
    m_buttonSpriteHovered->Process(deltaTime);
}

void CharacterThree::Draw(Renderer& renderer)
{   
    m_buttonFrameNormal->Draw(renderer, true, false);
    m_buttonFrameHovered->Draw(renderer, true, false);

    if (m_isHovered)
    {
        m_buttonSpriteHovered->Draw(renderer, true, true);
    }
    else
    {
        m_buttonSpriteNormal->Draw(renderer, true, true);
    }
}