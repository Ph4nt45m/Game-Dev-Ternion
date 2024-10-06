#include "TernionMenu.h"

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
MenuScene::MenuScene()
    : m_splashScene(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
MenuScene::~MenuScene()
{
    LogManager::GetInstance().Log("Splash destructor called");

    delete m_splashScene;
    m_splashScene = nullptr;
}

bool MenuScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, m_splashScene, "..\\Sprites\\Menus\\ternion_Menu.png");
    return (m_splashScene != nullptr);
}

void MenuScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;

}

void MenuScene::Draw(Renderer& renderer)
{
    if (m_splashScene)
    {
        m_splashScene->Draw(renderer, true, false);
    }
}

void MenuScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}