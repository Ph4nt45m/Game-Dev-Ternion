#include "SettingsMenu.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "sceneManager.h"
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "scene.h"
#include "game.h"

// Buttons
#include "MenuButton.h"
#include "IncreaseButton.h"
#include "DecreaseButton.h"
#include "IncreaseSoundEffectButton.h"
#include "DecreaseSoundEffectButton.h"


// Library includes:
#include <cassert>
#include <cstdlib>
#include <string>

//construct
SettingScene::SettingScene()
    : m_BackGroundScene(nullptr)
    , MainMenu(nullptr)
    , increase(nullptr)
    , decrease(nullptr)
    , decreaseSound(nullptr)
    , increaseSound(nullptr)
    , alphabet(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
SettingScene::~SettingScene()
{
    delete MainMenu;
    MainMenu = nullptr;

    delete increase;
    increase = nullptr;

    delete decrease;
    decrease = nullptr;

    delete decreaseSound;
    decreaseSound = nullptr;

    delete increaseSound;
    increaseSound = nullptr;

    delete alphabet;
    alphabet = nullptr;

    delete m_BackGroundScene;
    m_BackGroundScene = nullptr;
}

bool SettingScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, m_BackGroundScene, "..\\Sprites\\characterselect\\charselectbg.png"); // Changes made by Karl

    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();
    // Calculate button position as a percentage of window size
    float MenuButtonX = windowWidth * 0.8f;
    float MenuButtonY = windowHeight * 0.9f;
    float IncreaseX = windowWidth * 0.7f;
    float IncreaseY = windowHeight * 0.3f;
    float DecreaseX = windowWidth * 0.3f;
    float DecreaseY = windowHeight * 0.3f;
    float MusicBarX = windowWidth * 0.5f;
    float MusicBarY = windowWidth * 0.3f;
    float IncreaseSoundX = windowWidth * 0.7f;
    float IncreaseSoundY = windowHeight * 0.45f;
    float DecreaseSoundX = windowWidth * 0.3f;
    float DecreaseSoundY = windowHeight * 0.45f;

    MainMenu = new MenuButton(MenuButtonX, MenuButtonY);
    MainMenu->Initialise(renderer);

    decrease = new DecreaseButton(DecreaseX, DecreaseY);
    decrease->Initialise(renderer);

    increase = new IncreaseButton(IncreaseX, IncreaseY);
    increase->Initialise(renderer);

    increaseSound = new IncreaseSoundButton(IncreaseSoundX, IncreaseSoundY);
    increaseSound->Initialise(renderer);

    decreaseSound = new DecreaseSoundButton(DecreaseSoundX, DecreaseSoundY);
    decreaseSound->Initialise(renderer);

    alphabet = new Alphabet();
    alphabet->Initialize(renderer);

    return (m_BackGroundScene != nullptr);
}

void SettingScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;

    //Call button updates
    if (increase)
    {
        increase->Update(deltaTime, inputSystem);
    }
    if (decrease)
    {
        decrease->Update(deltaTime, inputSystem);
    }
    if (increaseSound)
    {
        increaseSound->Update(deltaTime, inputSystem);
    }
    if (decreaseSound)
    {
        decreaseSound->Update(deltaTime, inputSystem);
    }
    if (MainMenu)
    {
        MainMenu->Update(deltaTime, inputSystem);
    }
    std::string sentence = std::to_string(Game::GetInstance().GetSounds()->getMusicVolume());

    alphabet->Process(sentence, 1.0f, 1.0f, 1.0f);
}

void SettingScene::Draw(Renderer& renderer)
{
    if (m_BackGroundScene)
    {
        m_BackGroundScene->Draw(renderer, true, false);
    }
    if (MainMenu)
    {
        MainMenu->Draw(renderer);
    }
    if (increase)
    {
        increase->Draw(renderer);
    }
    if (decrease)
    {
        decrease->Draw(renderer);
    }
    if (increaseSound)
    {
        increaseSound->Draw(renderer);
    }
    if (decreaseSound)
    {
        decreaseSound->Draw(renderer);
    }
    std::string MusicSentence = std::to_string(Game::GetInstance().GetSounds()->getMusicVolume());
    std::string SoundSentence = std::to_string(Game::GetInstance().GetSounds()->getSoundVolume("bounce"));

    alphabet->DrawTextW(renderer, MusicSentence, 0.5f, 0.3f, 5.0f);
    alphabet->DrawTextW(renderer, SoundSentence, 0.5f, 0.45f, 5.0f);
}

void SettingScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: splashSCreen");

}