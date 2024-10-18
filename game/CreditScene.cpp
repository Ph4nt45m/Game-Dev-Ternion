#include "CreditScene.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "sceneManager.h"
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "scene.h"

// Library includes:
#include <cassert>
#include <cstdlib>
#include <string>

//construct
CreditScene::CreditScene()
    : m_splashScene(nullptr)
    , alphabet(nullptr)
    , m_fElapsedTime(0.0f)
    , m_fmoveUp(1.0f)
{
}//destruct
CreditScene::~CreditScene()
{

    delete m_splashScene;
    m_splashScene = nullptr;

    delete alphabet;
    alphabet = nullptr;
}

bool CreditScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, m_splashScene, "..\\Sprites\\Menus\\Background.png");
    m_splashScene->SetBlueTint(0.0f);
    m_splashScene->SetRedTint(0.0f);
    m_splashScene->SetGreenTint(0.0f);
    
    alphabet = new Alphabet();
    alphabet->Initialize(renderer);

    return (m_splashScene != nullptr);
}

void CreditScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;
    m_fmoveUp -= deltaTime * 0.25f;
    if (Game::GetInstance().GetCharacter())
    {
        Game::GetInstance().DeleteCharacter();
    }

    // For now, just transition to the next scene after a set time
    if (m_fElapsedTime > 10.0f) // 2.35 to sync with sound
    {
        // Code to change scene (handled by Game or SceneManager)
        SceneManager::GetInstance().ChangeScene(0);
    }
}

void CreditScene::Draw(Renderer& renderer)
{
    if (m_splashScene)
    {
        m_splashScene->Draw(renderer, true, false);
    }
    if (alphabet)
    {
        std::string sentence = "Development Team\n\nKyle Le Grange\nRauen Le Grange\nEthan Smith\nKarl Francisco\n\n\n\nMusic From\n\nBlakeJ\nJosh Woodward\n\n\n\nSounds From\n\nsoundbible\n\n\n\n\nSprites from\n\nchierit\nMonoPixelArt\nrecursion corp\nPedrodis\nFoozle";
        alphabet->DrawTextW(renderer, sentence, 0.5f, m_fmoveUp, 3.0f);
    }
}

void CreditScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}