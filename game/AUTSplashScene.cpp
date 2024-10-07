#include "AUTSplashScene.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "sceneManager.h"
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "scene.h"

#include "game.h"
#include "logmanager.h"
#include "renderer.h"
#include "inputsystem.h"
#include "sprite.h"
#include "forest.h"
#include "character.h"
#include "golem.h"


// Library includes:
#include <cassert>
#include <cstdlib>
#include <string>

//construct
SplashScene::SplashScene()
    : m_splashScene(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
SplashScene::~SplashScene()
{
    LogManager::GetInstance().Log("Splash destructor called");

    delete m_splashScene;
    m_splashScene = nullptr;
}

bool SplashScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, m_splashScene, "..\\Sprites\\AUT-Logo-1.png");
    return (m_splashScene != nullptr);
}

void SplashScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;

    // For now, just transition to the next scene after a set time
    if (m_fElapsedTime > 0.1f) // e.g., 3 seconds
    {
        // Code to change scene (handled by Game or SceneManager)
        SceneManager::GetInstance().ChangeScene(4);
    }
}

void SplashScene::Draw(Renderer& renderer)
{
    if (m_splashScene)
    {
        m_splashScene->Draw(renderer, true, false);
    }
}

void SplashScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}