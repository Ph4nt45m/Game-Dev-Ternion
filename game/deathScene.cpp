#include "deathScene.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "sceneManager.h"
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "scene.h"
#include "player.h"
#include "Camera.h"

//Buttons
#include "MenuButton.h"
#include "restartButton.h"
#include "exitButton.h"
#include "restartButton.h"
#include "CreditsButton.h"

// Library includes:
#include <cassert>
#include <cstdlib>
#include <string>

//construct
DeathScene::DeathScene(b2World* world, Player* character)
    : m_pWorld(world)
    , m_pCharacter(character)
    , m_camera()
    , m_splashScene(nullptr)
    , m_MenuButton(nullptr)
    , m_RollCredits(nullptr)
    , m_RestartButton(nullptr)
    , m_ExitButton(nullptr)
    , alphabet(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
DeathScene::~DeathScene()
{
    delete m_splashScene;
    m_splashScene = nullptr;

    delete m_MenuButton;
    m_MenuButton = nullptr;

    delete m_RollCredits;
    m_RollCredits = nullptr;

    delete m_RestartButton;
    m_RestartButton = nullptr;

    delete m_ExitButton;
    m_ExitButton = nullptr;

    delete alphabet;
    alphabet = nullptr;
}

bool DeathScene::Initialise(Renderer& renderer)
{
    Game::GetInstance().SetGravity(0.0f, 0.0f);
    m_pWorld->SetGravity(Game::GetInstance().GetGravity());
    SceneManager::GetInstance().LoadImage(renderer, m_splashScene, "..\\Sprites\\Menus\\DeathMenu\\deathMenu.png");
    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();
    // Calculate button position as a percentage of window size
    float restartButtonX = windowWidth * 0.2f;
    float restartButtonY = windowHeight * 0.85f;
    float creditsButtonX = windowWidth * 0.4f;
    float creditsButtonY = windowHeight * 0.85f;
    float MenuButtonX = windowWidth * 0.6f; // 20% of the window width
    float MenuButtonY = windowHeight * 0.85f; // 70% of the window height // Changes made by Karl
    float exitButtonX = windowWidth * 0.8f; // 20% of the window width
    float exitButtonY = windowHeight * 0.85f; // 70% of the window height // Changes made by Karl

    m_RollCredits = new CreditsButton(creditsButtonX, creditsButtonY);
    m_RollCredits->Initialise(renderer);
    m_RestartButton = new RestartButton(restartButtonX, restartButtonY);
    m_RestartButton->Initialise(renderer);
    m_MenuButton = new MenuButton(MenuButtonX, MenuButtonY);
    m_MenuButton->Initialise(renderer);
    m_ExitButton = new ExitButton(exitButtonX, exitButtonY);
    m_ExitButton->Initialise(renderer);

    alphabet = new Alphabet();
    alphabet->Initialize(renderer);

    SceneManager::GetInstance().setSongPlayListDeath();

    return (m_splashScene != nullptr);
}

void DeathScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;

    if (m_pCharacter)
    {
        m_pCharacter->Process(deltaTime, inputSystem);
    }

    if (m_RestartButton)
    {
        m_RestartButton->Update(deltaTime, inputSystem);
    }
    if (m_RollCredits)
    {
        m_RollCredits->Update(deltaTime, inputSystem);
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
    if (alphabet)
    {
        std::string words = "you reached level " + std::to_string(Game::GetInstance().difficulty);
        alphabet->DrawTextW(renderer, words, 0.5f, 0.2f, 3.0f);
    }
    if (m_pCharacter)
    {
        m_pCharacter->DrawWithCam(renderer, m_camera);
    }
    if (m_RollCredits)
    {
        m_RollCredits->Draw(renderer);
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