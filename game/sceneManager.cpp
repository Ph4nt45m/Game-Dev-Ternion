#include "sceneManager.h"

// local includes
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "sprite.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"

// Scenes
#include "forestNight.h"
#include "AUTSplashScene.h"
#include "TernionMenu.h"
#include "CharacterSelect.h"
#include "forestDay.h"
#include "SettingsMenu.h"
#include "deathScene.h"
#include "CreditScene.h"

// includes
#include <cassert>
#include <algorithm>
#include <cstdlib>


// Static member initialization
SceneManager* SceneManager::sm_pInstance = nullptr;

SceneManager& SceneManager::GetInstance()
{
    if (sm_pInstance == nullptr)
    {
        sm_pInstance = new SceneManager();
    }
    return *sm_pInstance;
}

void SceneManager::DestroyInstance()
{
    delete sm_pInstance;
    sm_pInstance = nullptr;
}

SceneManager::SceneManager()
	:m_isTransitionPending(false)
	, m_pendingSceneID(-1)
	, m_iCurrentScene(0)
	, m_pRenderer(0)
	, m_fElapsedTime(0.0f) // Changes made by Karl
	, sceneId(0)
	, m_iRed(0)
	, m_iGreen(0)
	, m_iBlue(0)
	, pause(false)
	, pauseScene(nullptr)
{
    // Constructor code here
}

SceneManager::~SceneManager()
{
    // Destructor code here
    // Clean up all allocated sprites
	delete pauseScene;

    for (Sprite* sprite : m_sprites)
    {
        delete sprite;
    }
    m_sprites.clear();
}

bool SceneManager::Initialise(Renderer& renderer)
{
    // Initialize scenes and resources
    // For example:
	m_pRenderer = &renderer;
    // Add more scene or resource initialization here
	pauseScene = new PauseScene();
	pauseScene->Initialise(renderer);
    return true; // Return false if initialization fails
}

void SceneManager::Process(float deltaTime, InputSystem& inputSystem)
{
	m_fElapsedTime += deltaTime;

	ButtonState PauseState = inputSystem.GetKeyState(SDL_SCANCODE_ESCAPE);

	if (PauseState == BS_PRESSED && (sceneId == 3 || sceneId == 4))
	{
		pause = !pause;  // Toggle the value of pause
	}

	if (!m_scenes.empty())
	{
		// Process the current scene
		if (!pause)
		{
			m_scenes[m_iCurrentScene]->Process(deltaTime, inputSystem);
		}
		if (pause)
		{
			pauseScene->Process(deltaTime, inputSystem);
		}
		// Changes made by Karl
		if (sceneId == 0 && m_fElapsedTime > 1.0f)
		{
			if (m_iRed < 31 || m_iBlue < 37 || m_iGreen < 13)
			{
				m_iRed += 15.5f * deltaTime;
				m_iGreen += 6.5f * deltaTime;
				m_iBlue += 19.0f * deltaTime;
			}
			else
			{
				m_iRed = 255;
				m_iGreen = 255;
				m_iBlue = 255;
			}

			m_pRenderer->SetClearColour(m_iRed, m_iGreen, m_iBlue);
		}
	}
	else
	{
		LogManager::GetInstance().Log("\nThis didn't work");
	}

	PerformSceneTransition();
}

void SceneManager::Draw(Renderer& renderer)
{
	if (!m_scenes.empty())
	{
		// Draw the current scene
		m_scenes[m_iCurrentScene]->Draw(renderer);
	}
	if (pause)
	{
		pauseScene->Draw(renderer);
	}
}
void SceneManager::LoadImage(Renderer& renderer, Sprite*& backgroundImage, std::string filePath)
{
    backgroundImage = renderer.CreateSprite(filePath.c_str());  // Create the sprite with the file path
    assert(backgroundImage);

	//set scale to screen scale
    float scaleX = static_cast<float>(renderer.GetWidth()) / backgroundImage->GetWidth();
    float scaleY = static_cast<float>(renderer.GetHeight()) / backgroundImage->GetHeight();
    float scale = std::min(scaleX, scaleY);
	//flip background to be correct
    backgroundImage->SetScale(-scale); // Flip the image

	//set pos x set pos y
    backgroundImage->SetX(static_cast<float>(renderer.GetWidth()) / 2.0f);
    backgroundImage->SetY(static_cast<float>(renderer.GetHeight()) / 2.0f);
}


//Scene transition handling
void SceneManager::ChangeScene(int newSceneID)
{    // Set the transition flag and store the pending scene ID
	m_isTransitionPending = true;
	m_pendingSceneID = newSceneID;
}
Scene* SceneManager::CreateSceneByID(int sceneID)
{
	Scene* newScene = nullptr;
	sceneId = sceneID; // Changes made by Karl

	switch (sceneID)
	{
	case 0://AUT Splash Scene
		m_pRenderer->SetClearColour(m_iRed, m_iGreen, m_iBlue); // Changes made by Karl
		newScene = new SplashScene();
		break;
	case 1://Ternion Menu Scene
		newScene = new MenuScene();
		break;
	case 2://Character select Scene
		newScene = new CharacterSelectScene(); // Changes made by Karl
		break;
	case 3://Foreset Scene(Dev Test Level)
		m_pRenderer->SetClearColour(255, 255, 255);
		//newScene = new ForestScene(Game::GetInstance().GetWorld(), Game::GetInstance().GetCharacter());
		newScene = new ForestDay(Game::GetInstance().GetWorld(), Game::GetInstance().GetCharacter());
		break;
	case 4://Foreset Scene(first level)
		m_pRenderer->SetClearColour(255, 255, 255);
		//newScene = new ForestScene(Game::GetInstance().GetWorld(), Game::GetInstance().GetCharacter());
		newScene = new ForestNight(Game::GetInstance().GetWorld(), Game::GetInstance().GetCharacter());
		break;
	case 5:
		newScene = new SettingScene();
		break;
	case 6:
		m_pRenderer->SetClearColour(0, 0, 0);
		newScene = new DeathScene(Game::GetInstance().GetWorld(), Game::GetInstance().GetCharacter());
		break;
	case 7:
		m_pRenderer->SetClearColour(0, 0, 0);
		newScene = new CreditScene();
		break;
	default:
		// Handle invalid scene ID
		return nullptr;
	}

	// Initialize the scene
	if (newScene && !newScene->Initialise(*m_pRenderer))
	{
		// Handle initialization failure
		delete newScene;
		return nullptr;
	}

	return newScene;
}
void SceneManager::PerformSceneTransition()
{
	if (m_isTransitionPending)
	{
		// Only proceed if there's a valid scene to transition to
		if (!m_scenes.empty() && m_scenes[m_iCurrentScene] != nullptr)
		{
			delete m_scenes[m_iCurrentScene];     // Delete the current scene
			m_scenes[m_iCurrentScene] = nullptr;  // Set to nullptr to avoid issues
			m_scenes.erase(m_scenes.begin());     // Erase the deleted scene from the vector
		}

		// Create the new scene using the stored scene ID
		Scene* newScene = CreateSceneByID(m_pendingSceneID);
		if (newScene)
		{
			m_scenes.insert(m_scenes.begin(), newScene);  // Insert the new scene at index 0
			m_iCurrentScene = 0;  // Ensure the current scene index is always 0
		}

		// Reset the transition flag
		m_isTransitionPending = false;
	}
}

bool SceneManager::getpauseBool()
{
	return pause;
}
void SceneManager::setpauseBool(bool setPause)
{
	pause = setPause;
}