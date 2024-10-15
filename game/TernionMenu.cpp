#include "TernionMenu.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "sceneManager.h"
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "scene.h"

// Buttons
#include "StartButton.h"
#include "SettingsButton.h"
#include "ExitButton.h"

// Library includes:
#include <cassert>
#include <cstdlib>
#include <string>

//construct
MenuScene::MenuScene()
    : m_BackGroundScene(nullptr)
    , m_pStartButton(nullptr)
    , m_pSettingsButton(nullptr) // Changes made by Karl
    , m_pExitButton(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
MenuScene::~MenuScene()
{
    delete m_pStartButton;
    m_pStartButton = nullptr;

    delete m_pSettingsButton;
    m_pSettingsButton = nullptr;

    delete m_pExitButton;
    m_pExitButton = nullptr;

    delete m_BackGroundScene;
    m_BackGroundScene = nullptr;
}

bool MenuScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, m_BackGroundScene, "..\\Sprites\\Menus\\ternion_Menu.png");

    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();
    // Calculate button position as a percentage of window size
    float startButtonX = windowWidth * 0.2f; // 20% of the window width
    float startButtonY = windowHeight * 0.7f; // 70% of the window height // Changes made by Karl
    float settButtonX = windowWidth * 0.5f;  // 20% of the window width
    float settButtonY = windowHeight * 0.7f;  // 70% of the window height
    float exitButtonX = windowWidth * 0.8f; // 20% of the window width
    float exitButtonY = windowHeight * 0.7f; // 70% of the window height // Changes made by Karl

    // Initialize the StartButton with relative position and size
    m_pStartButton = new StartButton(startButtonX, startButtonY); // 100x50 is the button size
    m_pStartButton->Initialise(renderer);
    // Changes made by Karl
    m_pSettingsButton = new SettingsButton(settButtonX, settButtonY);
    m_pSettingsButton->Initialise(renderer);

    m_pExitButton = new ExitButton(exitButtonX, exitButtonY);
    m_pExitButton->Initialise(renderer);


    return (m_BackGroundScene != nullptr);
}

void MenuScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;

    //Call button updates
    if (m_pStartButton)
    {
        m_pStartButton->Update(deltaTime, inputSystem);
    }
    if (m_pSettingsButton)
    {
        m_pSettingsButton->Update(deltaTime, inputSystem);
    }
    if (m_pExitButton)
    {
        m_pExitButton->Update(deltaTime, inputSystem);
    }

}

void MenuScene::Draw(Renderer& renderer)
{
    if (m_BackGroundScene)
    {
        m_BackGroundScene->Draw(renderer, true, false);
    }
    if (m_pStartButton)
    {
        m_pStartButton->Draw(renderer);
    }
    if (m_pSettingsButton)
    {
        m_pSettingsButton->Draw(renderer);
    }
    if (m_pExitButton)
    {
        m_pExitButton->Draw(renderer);
    }
}

void MenuScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}