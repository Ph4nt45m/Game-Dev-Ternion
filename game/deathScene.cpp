#include "deathScene.h"

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
DeathScene::DeathScene()
    : m_splashScene(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
DeathScene::~DeathScene()
{
    LogManager::GetInstance().Log("Splash destructor called");

    delete m_splashScene;
    m_splashScene = nullptr;
}

bool DeathScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, m_splashScene, "..\\Sprites\\Menus\\DeathMenu\\deathMenu.png");
    return (m_splashScene != nullptr);
}

void DeathScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;

}

void DeathScene::Draw(Renderer& renderer)
{
    if (m_splashScene)
    {
        m_splashScene->Draw(renderer, true, false);
    }
}

void DeathScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}