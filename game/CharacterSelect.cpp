#include "CharacterSelect.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "game.h"
#include "sceneManager.h"
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "scene.h"

#include "inputsystem.h"
#include "character.h"

// Buttons
#include "CharacterOne.h"
#include "CharacterTwo.h"
#include "CharacterThree.h"
#include "MenuButton.h"

// Library includes:
#include <cassert>
#include <cstdlib>
#include <string>

//construct
CharacterSelectScene::CharacterSelectScene()
    : m_ChrSelectScene(nullptr)
    , characterOne(nullptr)
    , characterTwo(nullptr)
    , characterThree(nullptr)
    , MainMenu(nullptr)
    , m_fElapsedTime(0.0f)
{
}//destruct
CharacterSelectScene::~CharacterSelectScene()
{
    LogManager::GetInstance().Log("Character select destructor called");

    delete characterOne;
    characterOne = nullptr;

    delete characterTwo;
    characterTwo = nullptr;

    delete characterThree;
    characterThree = nullptr;

    delete MainMenu;
    MainMenu = nullptr;

    delete m_ChrSelectScene;
    m_ChrSelectScene = nullptr;
}

bool CharacterSelectScene::Initialise(Renderer& renderer)
{
    SceneManager::GetInstance().LoadImage(renderer, m_ChrSelectScene, "..\\Sprites\\characterselect\\charselectbg.png"); // Changes made by Karl

    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();

    // Calculate button position as a percentage of window size
    float warriorButtonX = windowWidth * 0.2f; // 20% of the window width
    float warriorButtonY = windowHeight * 0.5f; // 50% of the window height
    float mageButtonX = windowWidth * 0.5f;
    float mageButtonY = windowHeight * 0.5f;
    float archerButtonX = windowWidth * 0.8f;
    float archerButtonY = windowHeight * 0.5f;
    float menuButtonX = windowWidth * 0.5f;
    float menuButtonY = windowHeight * 0.9f;

    // Initialize the CharacterOne Button with relative position and size
    characterOne = new CharacterOne(warriorButtonX, warriorButtonY);
    characterOne->Initialise(renderer);

    characterTwo = new CharacterTwo(mageButtonX, mageButtonY);
    characterTwo->Initialise(renderer);

    characterThree = new CharacterThree(archerButtonX, archerButtonY);
    characterThree->Initialise(renderer);

    MainMenu = new MenuButton(menuButtonX, menuButtonY);
    MainMenu->Initialise(renderer);

    return (m_ChrSelectScene != nullptr);
}

void CharacterSelectScene::Process(float deltaTime, InputSystem& inputSystem)
{
    m_fElapsedTime += deltaTime;

    if (characterOne)
    {
        characterOne->Update(deltaTime, inputSystem);
    }

    if (characterTwo)
    {
        characterTwo->Update(deltaTime, inputSystem);
    }

    if (characterThree)
    {
        characterThree->Update(deltaTime, inputSystem);
    }

    if (MainMenu)
    {
        MainMenu->Update(deltaTime, inputSystem);
    }

}

void CharacterSelectScene::Draw(Renderer& renderer)
{
    if (m_ChrSelectScene)
    {
        m_ChrSelectScene->Draw(renderer, true, false);
    }

    if (characterOne)
    {
        characterOne->Draw(renderer);
    }

    if (characterTwo)
    {
        characterTwo->Draw(renderer);
    }

    if (characterThree)
    {
        characterThree->Draw(renderer);
    }

    if (MainMenu)
    {
        MainMenu->Draw(renderer);
    }
}

void CharacterSelectScene::DebugDraw()
{
    // Implement any debug UI for this scene if necessary
    ImGui::Text("Scene: Character Select");

}