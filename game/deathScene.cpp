#include "deathScene.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "sceneManager.h"
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "scene.h"

//Buttons
#include "MenuButton.h"
#include "restartButton.h"
#include "exitButton.h"


// Library includes:
#include <cassert>
#include <cstdlib>
#include <string>

//construct
DeathScene::DeathScene()
    : m_splashScene(nullptr)
    , m_MenuButton(nullptr)
    , m_RestartButton(nullptr)
    , m_ExitButton(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
DeathScene::~DeathScene()
{
    delete m_splashScene;
    m_splashScene = nullptr;

    delete m_MenuButton;
    m_MenuButton = nullptr;

    delete m_RestartButton;
    m_RestartButton = nullptr;

    delete m_ExitButton;
    m_ExitButton = nullptr;
}

bool DeathScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, m_splashScene, "..\\Sprites\\Menus\\DeathMenu\\deathMenu.png");
    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();
    // Calculate button position as a percentage of window size
    float restartButtonX = windowWidth * 0.2f;
    float restartButtonY = windowHeight * 0.8f;
    float MenuButtonX = windowWidth * 0.6f; // 20% of the window width
    float MenuButtonY = windowHeight * 0.8f; // 70% of the window height // Changes made by Karl
    float exitButtonX = windowWidth * 0.8f; // 20% of the window width
    float exitButtonY = windowHeight * 0.8f; // 70% of the window height // Changes made by Karl


    m_RestartButton = new RestartButton(restartButtonX, restartButtonY);
    m_RestartButton->Initialise(renderer);
    m_MenuButton = new MenuButton(MenuButtonX, MenuButtonY);
    m_MenuButton->Initialise(renderer);
    m_ExitButton = new ExitButton(exitButtonX, exitButtonY);
    m_ExitButton->Initialise(renderer);


    return (m_splashScene != nullptr);
}

void DeathScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;
    if (m_RestartButton)
    {
        m_RestartButton->Update(deltaTime, inputSystem);
    }
    if (m_MenuButton)
    {
        m_MenuButton->Update(deltaTime, inputSystem);
    }
    if (m_ExitButton)
    {
        m_ExitButton->Update(deltaTime, inputSystem);
    }
}

void DeathScene::Draw(Renderer& renderer)
{
    if (m_splashScene)
    {
        m_splashScene->Draw(renderer, true, false);
    }
    if (m_RestartButton)
    {
        m_RestartButton->Draw(renderer);
    }
    if (m_MenuButton)
    {
        m_MenuButton->Draw(renderer);
    }
    if (m_ExitButton)
    {
        m_ExitButton->Draw(renderer);
    }

}

void DeathScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}