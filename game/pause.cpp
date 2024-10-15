#include "pause.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "sceneManager.h"
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "scene.h"
#include "inputsystem.h"

//Buttons
#include "MenuButton.h"
#include "ExitButton.h"

// Library includes:
#include <cassert>
#include <cstdlib>
#include <string>

//construct
PauseScene::PauseScene()
    : pauseScreen(nullptr)
    , transparante(nullptr)
    , MainMenu(nullptr)
    , m_pExitButton(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
PauseScene::~PauseScene()
{
    delete m_pExitButton;
    m_pExitButton = nullptr;

    delete transparante;
    transparante = nullptr;

    delete pauseScreen;
    pauseScreen = nullptr;
}

bool PauseScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, pauseScreen, "..\\Sprites\\Menus\\pauseMenu\\pauseMenu.png");
    SceneManager::GetInstance().LoadImage(renderer, transparante, "..\\Sprites\\Menus\\background.png");

    transparante->SetAlpha(0.4f);
    transparante->SetBlueTint(0.4f);
    transparante->SetRedTint(0.4f);
    transparante->SetGreenTint(0.4f);

    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();
    // Calculate button position as a percentage of window size
    float MenuButtonX = windowWidth * 0.8f; // 20% of the window width
    float MenuButtonY = windowHeight * 0.7f; // 70% of the window height // Changes made by Karl
    float exitButtonX = windowWidth * 0.2f; // 20% of the window width
    float exitButtonY = windowHeight * 0.7f; // 70% of the window height // Changes made by Karl


    m_pExitButton = new ExitButton(exitButtonX, exitButtonY);
    m_pExitButton->Initialise(renderer);
    MainMenu = new MenuButton(MenuButtonX, MenuButtonY);
    MainMenu->Initialise(renderer);

    return (pauseScreen && transparante);
}

void PauseScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;
    if (m_pExitButton)
    {
        m_pExitButton->Update(deltaTime, inputSystem);
    }
    if (MainMenu)
    {
        MainMenu->Update(deltaTime, inputSystem);
    }
}

void PauseScene::Draw(Renderer& renderer)
{
    if (pauseScreen)
    {
        transparante->Draw(renderer, true, false);
        pauseScreen->Draw(renderer, true, false);
    }
    if (m_pExitButton)
    {
        m_pExitButton->Draw(renderer);
    }
    if (MainMenu)
    {
        MainMenu->Draw(renderer);
    }
}

void PauseScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}