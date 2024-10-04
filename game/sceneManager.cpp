#include "sceneManager.h"

// local includes
#include "../imgui/imgui.h"
#include "logmanager.h"
#include "sprite.h"
#include "renderer.h"
#include "scene.h"
#include "game.h"

// Scenes
#include "forestscene.h"
#include "AUTSplashScene.h"

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
{
    // Constructor code here
}

SceneManager::~SceneManager()
{
    // Destructor code here
    // Clean up all allocated sprites
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

    return true; // Return false if initialization fails
}

void SceneManager::Process(float deltaTime, InputSystem& inputSystem)
{
	if (!m_scenes.empty())
	{
		// Process the current scene
		m_scenes[m_iCurrentScene]->Process(deltaTime, inputSystem);
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
	else
	{
		LogManager::GetInstance().Log("\ncouldn't be drawn");
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

	switch (sceneID)
	{
	case 0://AUT Splash Scene
		newScene = new SplashScene();
		break;
	case 1://Ternion Menu Scene
//		newScene = new menuScene();
		break;
	case 2://Character select Scene
		break;
	case 3://Foreset Scene(first level
		newScene = new ForestScene();
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
	//Set one character for the scene
	newScene->SetCharacter(*Game::GetInstance().GetCharacter(), *m_pRenderer);

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

