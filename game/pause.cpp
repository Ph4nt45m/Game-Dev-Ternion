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
#include "resumeButton.h"
#include "MenuButton.h"
#include "ExitButton.h"
#include "restartButton.h"

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
    , resumeButton(nullptr)
    , restartButton(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
PauseScene::~PauseScene()
{
    delete m_pExitButton;
    m_pExitButton = nullptr;

    delete transparante;
    transparante = nullptr;

    delete resumeButton;
    resumeButton = nullptr;

    delete restartButton;
    restartButton = nullptr;

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
    float ResumeButtonX = windowWidth * 0.5f;
    float ResumeButtonY = windowHeight * 0.3f;
    float restartButtonX = windowWidth * 0.5f;
    float restartButtonY = windowHeight * 0.5f;
    float MenuButtonX = windowWidth * 0.65f; // 20% of the window width
    float MenuButtonY = windowHeight * 0.8f; // 70% of the window height // Changes made by Karl
    float exitButtonX = windowWidth * 0.35f; // 20% of the window width
    float exitButtonY = windowHeight * 0.8f; // 70% of the window height // Changes made by Karl


    resumeButton = new ResumeButton(ResumeButtonX, ResumeButtonY);
    resumeButton->Initialise(renderer);
//    restartButton = new RestartButton(restartButtonX, restartButtonY);
//    restartButton->Initialise(renderer);
    MainMenu = new MenuButton(MenuButtonX, MenuButtonY);
    MainMenu->Initialise(renderer);
    m_pExitButton = new ExitButton(exitButtonX, exitButtonY);
    m_pExitButton->Initialise(renderer);

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
    if (resumeButton)
    {
        resumeButton->Update(deltaTime, inputSystem);
    }
/*    if (restartButton)
    {
        restartButton->Update(deltaTime, inputSystem);
    }*/
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
    if (resumeButton)
    {
        resumeButton->Draw(renderer);
    }
/*    if (restartButton)
    {
        restartButton->Draw(renderer);
    }*/
}

void PauseScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}