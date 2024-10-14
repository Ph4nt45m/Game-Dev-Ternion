// This include: 
#include "game.h"
#include "MyContactListener.h"

// Local includes:
#include "renderer.h" 
#include "logmanager.h"
#include "scenecheckerboards.h"
#include "scenebouncingballs.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "inputsystem.h"
#include "SDL_scancode.h"
#include "character.h"
#include "animatedsprite.h"
#include "vector2.h"
#include "sceneManager.h"
#include "Warrior.h" // Changes made by Karl
#include "Mage.h" // Changes made by Karl
#include "Archer.h" // Changes made by Karl

// Library includes:
#include <windows.h>
#include <Box2D.h>
#include <iostream>
#include <stdio.h>

// Static Members:
Game* Game::sm_pInstance = 0;

Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	return (*sm_pInstance);
}

void Game::DestroyInstance()
{
	delete sm_pInstance; sm_pInstance = 0;
}
Game::Game()
	: m_pRenderer(0)
	, m_iCurrentScene(0)
	, m_iLastTime(0)
	, m_fExecutionTime(0)
	, m_fElapsedSeconds(0)
	, m_iFrameCount(0)
	, m_iFPS(0)
	, m_pCursor()
	, m_pScForestScene(0)
	, m_pEntCharacter(0)
	, m_pASprAnimatedSprite(0)
	, m_sprCursorBodySprite(0)
	, m_sprCursorBorderSprite(0)
	, m_bShowDebugWindow(0)
	, m_pInputSystem(0)
	, m_iMouseState(0)
	, m_bLooping(true)
	, soundManager(0)
{

}

Game::~Game()
{
	delete m_pRenderer; 
	m_pRenderer = 0;

	delete m_pASprAnimatedSprite;
	m_pASprAnimatedSprite = 0;

	delete m_sprCursorBorderSprite;
	m_sprCursorBorderSprite = 0;

	delete m_pInputSystem;
	m_pInputSystem = 0;

	delete m_pEntCharacter;
	m_pEntCharacter = 0;

	if (soundManager) {
		delete soundManager;
		soundManager = nullptr;
	}
}

void Game::Quit()
{
	m_bLooping = false;
}

bool Game::Initialise()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int bbWidth = 1550; // 1550 originally
	int bbHeight = 800; // 800 originally

	//World init
	SetGravity(0.0f, 0.0f);
	world = new b2World{ m_gravity };
	world->SetContactListener(&m_contactListener);

	//Renderder
	m_pRenderer = new Renderer();
	if (!m_pRenderer->Initialise(true, bbWidth, bbHeight)) // true = windowed, false = fullscreen
	{
		LogManager::GetInstance().Log("Renderer failed to initialise!"); 
		return false;
	}

	bbWidth = m_pRenderer->GetWidth(); // 1536
	bbHeight = m_pRenderer->GetHeight(); // 846

	m_iLastTime = SDL_GetPerformanceCounter();
	m_pRenderer->SetClearColour(255, 255, 255);

	//Input System
	m_pInputSystem = new InputSystem();
	if (!m_pInputSystem->Initialise())
	{
		LogManager::GetInstance().Log("InputSystem failed to initialise!");
		return false;
	}

	//Character made
	/*m_pEntCharacter = new Character(world); // Changes made by Karl
	if (!m_pEntCharacter->Initialise(*m_pRenderer))
	{
		LogManager::GetInstance().Log("Character failed to initialise!");
		return false;
	}*/

	//Scene
	// Initialize SceneManager and the first scene
	SceneManager& sceneManager = SceneManager::GetInstance();
	if (!sceneManager.Initialise(*m_pRenderer))
	{
		LogManager::GetInstance().Log("SceneManager failed to initialise!");
		return false;
	}
	// Optionally, load the first scene if not using transitions right away
	sceneManager.ChangeScene(0); // Load initial scene (e.g., splash screen, menu)
	sceneManager.PerformSceneTransition(); // Perform the transition to the first scene


	m_pASprAnimatedSprite = m_pRenderer->CreateAnimatedSprite("Sprites\\explosion.png");
	if (!m_pASprAnimatedSprite)
	{
		LogManager::GetInstance().Log("AnimatedSprite failed to initialise!");
		return false;
	}
	else
	{
		m_pASprAnimatedSprite->SetupFrames(64, 64);
		m_pASprAnimatedSprite->SetFrameDuration(0.08f);
	}

	m_sprCursorBorderSprite = m_pRenderer->CreateSprite("Sprites\\cursor.png");
	m_sprCursorBodySprite = m_pRenderer->CreateSprite("Sprites\\cursor.png");

	for (b2Body* body = world->GetBodyList(); body != nullptr; body = body->GetNext()) {
		printf("Body: %p, UserData: %p\n", (void*)body, body->GetUserData());
	}

	//Kyle code
	// Initialize the SoundManager
	soundManager = new SoundManager();

	// Initialize SDL_mixer
	if (!soundManager->init()) {
		return false;
	}
	//// Load sounds and music
	soundManager->loadSound("bounce", "..\\Sprites\\sounds\\Bounce-SoundBible.com-12678623.wav");
	soundManager->loadMusic("background", "..\\Sprites\\sounds\\JoshWoodward-Circles-NoVox.mp3");
	//// Load and play the new music
	soundManager->loadMusic("newBackground", "..\\Sprites\\sounds\\JoshWoodward-AttS-07-WordsFallApart-NoVox.mp3");

	// Play the background music (loop infinitely)
//	soundManager->playMusic("background", -1);	//Kyle end
	soundManager->setMusicVolume(80);
	//Kyle code end

	return true;
}

bool Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	m_pInputSystem->ProcessInput();

	if (m_bLooping)
	{
		Uint64 current = SDL_GetPerformanceCounter();
		float deltaTime = (current - m_iLastTime) / static_cast<float>(SDL_GetPerformanceFrequency());

		m_iLastTime = current;

		m_fExecutionTime += deltaTime;

		Process(deltaTime);

#ifdef USE_LAG
		m_fLag += deltaTime;

		int innerLag = 0;

		while (m_fLag >= stepSize)
		{
			Process(stepSize);

			m_fLag -= stepSize;

			++m_iUpdateCount;
			++innerLag;
		}
#endif //USE_LAG
		
		Draw(*m_pRenderer);
	}

	return m_bLooping;
}

void
Game::Process(float deltaTime)
{
	ProcessFrameCounting(deltaTime);
	m_elapsedTime += deltaTime;
	// TODO: Add game objects to process here!
	if (m_elapsedTime > 3.0f)
	{
		soundManager->setSoundVolume("bounce", getsoundEffectsVolume());
		soundManager->playSound("bounce");
		printf("We played a sound effect");
		m_elapsedTime = 0;
	}
	// Box2D time step
	const float32 timeStep = 1.0f / 60.0f;  // 60Hz update rate
	const int32 velocityIterations = 6;     // Box2D velocity solver iterations
	const int32 positionIterations = 2;     // Box2D position solver iterations

	// Step the Box2D world to simulate physics
	
	// Step the world
	world->Step(timeStep, velocityIterations, positionIterations);

	//Checks which scene, if scene is legal and runs process for scene.
	SceneManager::GetInstance().Process(deltaTime, *m_pInputSystem);

	//I think this runs player character
	/*if (m_pEntCharacter->IsDefined()) // Changes made by Karl
	{
		m_pEntCharacter->Process(deltaTime, *m_pInputSystem);
	}*/
	
	m_pASprAnimatedSprite->Process(deltaTime);

	//Cursor that follows mouse and explodes when presses
	if (m_sprCursorBorderSprite)
	{
		m_pCursor.SetPosition(m_pInputSystem->GetMousePosition());

		m_sprCursorBorderSprite->SetCursorBorder(&m_pCursor, m_sprCursorBorderSprite);
		m_sprCursorBorderSprite->Process(deltaTime);
		m_sprCursorBodySprite->SetCursorBody(&m_pCursor, m_sprCursorBodySprite);
		m_sprCursorBodySprite->Process(deltaTime);

		m_iMouseState = m_pInputSystem->GetMouseButtonState(SDL_BUTTON_LEFT);

		if (m_iMouseState == BS_PRESSED)
		{
			m_pASprAnimatedSprite->SetX((int)m_pCursor.GetX());
			m_pASprAnimatedSprite->SetY((int)m_pCursor.GetY());
			m_pASprAnimatedSprite->Animate();
		}
	}

}

void
Game::Draw(Renderer& renderer)
{
	++m_iFrameCount;

	renderer.Clear();

	// TODO: Add game objects to draw here!

	SceneManager::GetInstance().Draw(renderer);

	if (m_pASprAnimatedSprite->IsAnimating())
	{
		m_pASprAnimatedSprite->Draw(renderer, false, false);
	}

	if (m_sprCursorBorderSprite)
	{
		m_sprCursorBorderSprite->Draw(renderer, false, false);
	}

	if (m_sprCursorBodySprite)
	{
		m_sprCursorBodySprite->Draw(renderer, false, false);
	}

	DebugDraw();

	renderer.Present();
}

void
Game::ProcessFrameCounting(float deltaTime)
{
	// Count total simulation time elapsed: 
	m_fElapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_fElapsedSeconds > 1.0f)
	{
		m_fElapsedSeconds -= 1.0f; m_iFPS = m_iFrameCount; m_iFrameCount = 0;
	}
}

void
Game::DebugDraw()
{
	bool open = true;
	//ImGui::Begin("Debug Window - Bouncing Ball", &open, ImGuiWindowFlags_MenuBar);
	////ImGui::Text("COMP710 GP Framework (%s)", "2022, S2");

	//if (ImGui::Button("Quit"))
	//{
	//	Quit();
	//}

	//ImGui::SliderInt("Active scene", &m_iCurrentScene, 0, (m_scenes.size() > 0) ? m_scenes.size() - 1 : 0, "%d");
	//m_scenes[m_iCurrentScene]->DebugDraw(); // Call DebugDraw of the scene, for example bouncing balls
	
	//ImGui::End();

	ImGui::Begin("Debug Window - Cursor", &open, ImGuiWindowFlags_MenuBar);
	//ImGui::Text("COMP710 GP Framework (%s)", "2022, S2");

	if (ImGui::Button("Quit"))
	{
		Quit();
	}

	float colourBorder[4] = {  };
	colourBorder[0] = m_sprCursorBorderSprite->GetRedTint();
	colourBorder[1] = m_sprCursorBorderSprite->GetGreenTint();
	colourBorder[2] = m_sprCursorBorderSprite->GetBlueTint();
	colourBorder[3] = m_sprCursorBorderSprite->GetAlpha();
	ImGui::ColorEdit4("Border colour", colourBorder);
	m_sprCursorBorderSprite->SetRedTint(colourBorder[0]);
	m_sprCursorBorderSprite->SetGreenTint(colourBorder[1]);
	m_sprCursorBorderSprite->SetBlueTint(colourBorder[2]);
	m_sprCursorBorderSprite->SetAlpha(colourBorder[3]);

	float colourBody[4] = {  };
	colourBody[0] = m_sprCursorBodySprite->GetRedTint();
	colourBody[1] = m_sprCursorBodySprite->GetGreenTint();
	colourBody[2] = m_sprCursorBodySprite->GetBlueTint();
	colourBody[3] = m_sprCursorBodySprite->GetAlpha();
	ImGui::ColorEdit4("Body colour", colourBody);
	m_sprCursorBodySprite->SetRedTint(colourBody[0]);
	m_sprCursorBodySprite->SetGreenTint(colourBody[1]);
	m_sprCursorBodySprite->SetBlueTint(colourBody[2]);
	m_sprCursorBodySprite->SetAlpha(colourBody[3]);

	ImGui::End();
}

void
Game::ToggleDebugWindow()
{
	m_bShowDebugWindow = !m_bShowDebugWindow;

	m_pInputSystem->ShowMouseCursor(m_bShowDebugWindow);
}
// Changes made by Karl
void
Game::CreateCharacter(int type)
{
	switch (type)
	{
	case 0:
		m_pEntCharacter = new Warrior(world);
		break;
	case 1:
		m_pEntCharacter = new Mage(world);
		break;
	case 2:
		m_pEntCharacter = new Archer(world);
		break;
	}

	if (!m_pEntCharacter->Initialise(*m_pRenderer))
	{
		LogManager::GetInstance().Log("Character failed to initialise!");
	}
	else
	{
		m_pEntCharacter->SetCharacterType(*m_pRenderer, type);
	}
}
// Changes made by Karl
//Character* Game::GetCharacter() const
//{
//	return m_pEntCharacter;
//}

Player* Game::GetCharacter() const
{
	return m_pEntCharacter;
}

b2World* Game::GetWorld() const
{
	return world;
}

b2Vec2 Game::GetGravity()
{
	return m_gravity;
}

void Game::SetGravity(float x, float y)
{
	m_gravity.x = x;
	m_gravity.y = y;
}

SoundManager* Game::GetSounds()
{
	return soundManager;
}

void Game::setsoundEffectsVolume(int SoundVol)
{
	soundEffectsVolume = SoundVol;
}
int Game::getsoundEffectsVolume()
{
	return soundEffectsVolume;
}
