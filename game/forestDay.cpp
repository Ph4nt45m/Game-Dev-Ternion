// COMP710 GP Framework 2022

// This include:
#include "forestDay.h"

// Local includes:
#include "game.h"
#include "logmanager.h"
#include "renderer.h"
#include "inputsystem.h"
#include "sprite.h"
#include "player.h"
#include "golem.h"
#include "mushroom.h"
#include "skeleton.h" // Changes made by Karl
#include "spider.h"
#include "LevelBuilder.h"
#include "sceneManager.h"

// Library includes:
#include <iostream>
#include <ctime>
#include "Camera.h"

// Static members:
//int ForestDay::sm_iSegmentWidth = 0;

ForestDay::ForestDay(b2World* world, Player* character)
	: m_sDayFrameOne{ 0, 0, 0 } // Changes made by Karl - Start
	, m_sDayFrameTwo{ 0, 0, 0 }
	, m_sFogFrameOne{ 0, 0, 0 }
	, m_sFogFrameTwo{ 0, 0, 0 } // Changes made by Karl - End
	, m_pGolem(0)
	, m_pMushroom(0)
	, m_pSpider(0)
	, m_pWorld(world)
	, camera()
	, m_pCharacter(character)
	, ground(nullptr)
	, level(nullptr)
	, m_fWindowWidth(0.0f) // Changes made by Karl
	, m_fWindowHeight(0.0f)
	, m_iBackground(0)
	, m_fLoopRange(0.0f)
	, m_vEnemies(0)
{

}

ForestDay::~ForestDay()
{	// Changes made by Karl - Start
	delete m_sDayFrameOne.m_pBackground;
	m_sDayFrameOne.m_pBackground = nullptr;

	delete m_sDayFrameOne.m_pForeground;
	m_sDayFrameOne.m_pForeground = nullptr;

	delete m_sDayFrameOne.m_pGround;
	m_sDayFrameOne.m_pGround = nullptr;

	delete m_sDayFrameTwo.m_pBackground;
	m_sDayFrameTwo.m_pBackground = nullptr;

	delete m_sDayFrameTwo.m_pForeground;
	m_sDayFrameTwo.m_pForeground = nullptr;

	delete m_sDayFrameTwo.m_pGround;
	m_sDayFrameTwo.m_pGround = nullptr;

	delete m_sFogFrameOne.m_pBackground;
	m_sFogFrameOne.m_pBackground = nullptr;

	delete m_sFogFrameOne.m_pForeground;
	m_sFogFrameOne.m_pForeground = nullptr;

	delete m_sFogFrameOne.m_pGround;
	m_sFogFrameOne.m_pGround = nullptr;

	delete m_sFogFrameTwo.m_pBackground;
	m_sFogFrameTwo.m_pBackground = nullptr;

	delete m_sFogFrameTwo.m_pForeground;
	m_sFogFrameTwo.m_pForeground = nullptr;

	delete m_sFogFrameTwo.m_pGround;
	m_sFogFrameTwo.m_pGround = nullptr;
	// Changes made by Karl - End
	delete m_pGolem;
	m_pGolem = 0;

	for (Enemy* enemy : m_vEnemies)
	{
		delete enemy;
		enemy = nullptr;
	}

	m_vEnemies.clear();

	delete ground;
	ground = nullptr;

	delete level;
	level = nullptr;	
}

bool
ForestDay::Initialise(Renderer& renderer)
{
	//Made by Rauen
	m_pCharacter->setCharacterPos();
	// Define some terrain segments at different positions and sizes
	Game::GetInstance().SetGravity(0.0f, 1.0f);
	m_pWorld->SetGravity(Game::GetInstance().GetGravity());
	// Changes made by Karl - Moved setting sprites to own function to debulk initialise
	// Set background sprites
	if (!SetBGSprites(renderer))
	{
		LogManager::GetInstance().Log("Background Sprites failed to initialise!");
		return false;
	}

	//const float SCALE = 30.0f;
	float terrainWidth = 300.0f / SCALE;  // Convert pixel width to meters
	float terrainHeight = 100.0f / SCALE;  // Convert pixel height to meters
	float wallWidth = 100.0f / SCALE;  // Thin wall, converted to meters
	float wallHeight = 10000.0f / SCALE;  // Wall height in meters
	float worldWidth = 500000.0f / SCALE;  // World width converted to meters
	m_fWindowHeight = renderer.GetHeight(); // Changes made by Karl - made width and height class variables
	m_fWindowWidth = renderer.GetWidth();
	m_fLoopRange = m_sDayFrameOne.m_pBackground->GetWidth() * 2.0f;
	float worldHeight = m_fWindowHeight / SCALE;  // World height converted to meters

	// Set the Y position to the bottom of the window and convert to meters
	//float groundY = (windowHeight - terrainHeight) / SCALE;  // Convert to meters

	// Create the ground object, converting width/height to meters
	ground = new Terrain(m_pWorld, 0.0f, m_fWindowHeight /SCALE, worldWidth, terrainHeight, GROUND);
	ground->Initialise(renderer);

	level = new LevelBuilder(m_pWorld, worldWidth, worldHeight, 5);
	level->BuildLevel(renderer);

	camera.SetCamera(m_fWindowWidth, m_fWindowHeight, worldWidth, worldHeight);
	SetEnemies(renderer);

	SceneManager::GetInstance().setSongPlayListGames();

	return true;
}

void
ForestDay::Process(float deltaTime, InputSystem& inputSystem)
{	// Changes made by Karl - Deleted old unused character process call
	if (!m_pGolem->getIsAlive())
	{
		Game::GetInstance().incrementDiffeculty();
		delete m_pGolem;
		m_pGolem = nullptr;
		SceneManager::GetInstance().ChangeScene(3);
		return;
	}
	// Update background sprite positions
	UpdateBackground();
	
	// Changes made by Karl - End
	m_pCharacter->Process(deltaTime, inputSystem);
	m_pGolem->Process(deltaTime, inputSystem);

	for (auto it = m_vEnemies.begin(); it != m_vEnemies.end(); ) {
		Enemy* enemy = *it;
		if (enemy->GetAlive()) {
			enemy->Process(deltaTime, inputSystem);
			++it;  // Move to the next element
		}
		else {
			delete enemy;  // Free memory
			it = m_vEnemies.erase(it);  // Erase and update iterator to the next element
		}
	}
	camera.Update(*m_pCharacter);
	
}

void
ForestDay::Draw(Renderer& renderer)
{	// Changes made by Karl - Start - Draw layered background sprites
	switch (m_iBackground)
	{
	case 0: // Day scene
		// Background frames
		m_sDayFrameOne.m_pBackground->Draw(renderer, false, true);
		m_sDayFrameTwo.m_pBackground->Draw(renderer, false, true);

		// Foreground frames
		m_sDayFrameOne.m_pForeground->Draw(renderer, false, true);
		m_sDayFrameTwo.m_pForeground->Draw(renderer, false, true);

		// Ground frames
		m_sDayFrameOne.m_pGround->Draw(renderer, false, true);
		m_sDayFrameTwo.m_pGround->Draw(renderer, false, true);
		break;
	case 1: // Fog scene
		// Background frames
		m_sFogFrameOne.m_pBackground->Draw(renderer, false, true);
		m_sFogFrameTwo.m_pBackground->Draw(renderer, false, true);

		// Foreground frames
		m_sFogFrameOne.m_pForeground->Draw(renderer, false, true);
		m_sFogFrameTwo.m_pForeground->Draw(renderer, false, true);

		// Ground frames
		m_sFogFrameOne.m_pGround->Draw(renderer, false, true);
		m_sFogFrameTwo.m_pGround->Draw(renderer, false, true);
		break;
	}
	// Changes made by Karl - End
	m_pCharacter->DrawWithCam(renderer, camera);

	ground->Draw(renderer, camera);  // Pass the camera object to adjust positions based on the camera's position

	level->Draw(renderer, camera);
	m_pGolem->Draw(renderer, camera);
	
	for (auto it = m_vEnemies.begin(); it != m_vEnemies.end(); ) {
		Enemy* enemy = *it;
		if (enemy->GetAlive()) {
			enemy->Draw(renderer, camera);
			++it;  // Move to the next element
		}
		else {
			delete enemy;  // Free memory
			it = m_vEnemies.erase(it);  // Erase and update iterator to the next element
		}
	}
}

//Testing stuff with enemies for later
bool
ForestDay::SetEnemies(Renderer& renderer)
{
	m_pGolem = new Golem(m_pWorld);
	m_pGolem->SetCamera(&camera);
	m_pGolem->SetPlayer(m_pCharacter);

	if (!(m_pGolem->Initialise(renderer)))
	{
		LogManager::GetInstance().Log("Golem failed to initialise!");
		return false;
	}

	for (int i = 0; i < Game::GetInstance().difficulty; i++)
	{
		m_pMushroom = new Mushroom(m_pWorld);
		m_vEnemies.push_back(m_pMushroom);
		m_pMushroom->SetCamera(&camera);
		m_pMushroom->SetPlayer(m_pCharacter);

		if (!(m_pMushroom->Initialise(renderer)))
		{
			LogManager::GetInstance().Log("Mushroom failed to initialise!");
			return false;
		}

		m_pSpider = new Spider(m_pWorld);
		m_vEnemies.push_back(m_pSpider);
		m_pSpider->SetCamera(&camera);
		m_pSpider->SetPlayer(m_pCharacter);

		if (!(m_pSpider->Initialise(renderer)))
		{
			LogManager::GetInstance().Log("Spider failed to initialise!");
			return false;
		}
	}
	return true;
}
// Changes made by Karl - Start
// Function to set all background sprites for layering
bool
ForestDay::SetBGSprites(Renderer& renderer)
{	
	// Day scene
	// Frame one
	m_sDayFrameOne.m_pBackground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\backgroundday.png");

	if (!(m_sDayFrameOne.m_pBackground))
	{
		LogManager::GetInstance().Log("Background day frame one failed to initialise!");
		return false;
	}
	else
	{
		m_sDayFrameOne.m_pBackground->SetX(renderer.GetWidth() / 2.0f);
		m_sDayFrameOne.m_pBackground->SetY(renderer.GetHeight() / 2.0f);
	}

	m_sDayFrameOne.m_pForeground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\foregroundday.png");

	if (!(m_sDayFrameOne.m_pForeground))
	{
		LogManager::GetInstance().Log("Foreground day frame one failed to initialise!");
		return false;
	}
	else
	{
		m_sDayFrameOne.m_pForeground->SetX(renderer.GetWidth() / 2.0f);
		m_sDayFrameOne.m_pForeground->SetY(renderer.GetHeight() / 2.0f);
	}

	m_sDayFrameOne.m_pGround = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\groundday.png");

	if (!(m_sDayFrameOne.m_pGround))
	{
		LogManager::GetInstance().Log("Ground day frame one failed to initialise!");
		return false;
	}
	else
	{
		m_sDayFrameOne.m_pGround->SetX(renderer.GetWidth() / 2.0f);
		m_sDayFrameOne.m_pGround->SetY(renderer.GetHeight() / 2.0f);
	}

	// Frame two
	m_sDayFrameTwo.m_pBackground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\backgroundday.png");

	if (!(m_sDayFrameTwo.m_pBackground))
	{
		LogManager::GetInstance().Log("Background day frame two failed to initialise!");
		return false;
	}
	else
	{
		m_sDayFrameTwo.m_pBackground->SetX((renderer.GetWidth() + (m_sDayFrameTwo.m_pBackground->GetWidth() / 2.0f)));
		m_sDayFrameTwo.m_pBackground->SetY(renderer.GetHeight() / 2.0f);
	}

	m_sDayFrameTwo.m_pForeground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\foregroundday.png");

	if (!(m_sDayFrameTwo.m_pForeground))
	{
		LogManager::GetInstance().Log("Foreground day frame two failed to initialise!");
		return false;
	}
	else
	{
		m_sDayFrameTwo.m_pForeground->SetX((renderer.GetWidth() + (m_sDayFrameTwo.m_pForeground->GetWidth() / 2.0f)));
		m_sDayFrameTwo.m_pForeground->SetY(renderer.GetHeight() / 2.0f);
	}

	m_sDayFrameTwo.m_pGround = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\groundday.png");

	if (!(m_sDayFrameTwo.m_pGround))
	{
		LogManager::GetInstance().Log("Ground day frame two failed to initialise!");
		return false;
	}
	else
	{
		m_sDayFrameTwo.m_pGround->SetX((renderer.GetWidth() + (m_sDayFrameTwo.m_pGround->GetWidth() / 2.0f)));
		m_sDayFrameTwo.m_pGround->SetY(renderer.GetHeight() / 2.0f);
	}

	// Fog scene
	// Frame one
	m_sFogFrameOne.m_pBackground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\backgroundfog.png");

	if (!(m_sFogFrameOne.m_pBackground))
	{
		LogManager::GetInstance().Log("Background fog frame one failed to initialise!");
		return false;
	}
	else
	{
		m_sFogFrameOne.m_pBackground->SetX(renderer.GetWidth() / 2.0f);
		m_sFogFrameOne.m_pBackground->SetY(renderer.GetHeight() / 2.0f);
	}

	m_sFogFrameOne.m_pForeground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\foregroundfog.png");

	if (!(m_sFogFrameOne.m_pForeground))
	{
		LogManager::GetInstance().Log("Foreground fog frame one failed to initialise!");
		return false;
	}
	else
	{
		m_sFogFrameOne.m_pForeground->SetX(renderer.GetWidth() / 2.0f);
		m_sFogFrameOne.m_pForeground->SetY(renderer.GetHeight() / 2.0f);
	}

	m_sFogFrameOne.m_pGround = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\groundfog.png");

	if (!(m_sFogFrameOne.m_pGround))
	{
		LogManager::GetInstance().Log("Ground fog frame one failed to initialise!");
		return false;
	}
	else
	{
		m_sFogFrameOne.m_pGround->SetX(renderer.GetWidth() / 2.0f);
		m_sFogFrameOne.m_pGround->SetY(renderer.GetHeight() / 2.0f);
	}

	// Frame two
	m_sFogFrameTwo.m_pBackground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\backgroundfog.png");

	if (!(m_sFogFrameTwo.m_pBackground))
	{
		LogManager::GetInstance().Log("Background fog frame two failed to initialise!");
		return false;
	}
	else
	{
		m_sFogFrameTwo.m_pBackground->SetX((renderer.GetWidth() + (m_sFogFrameTwo.m_pBackground->GetWidth() / 2.0f)));
		m_sFogFrameTwo.m_pBackground->SetY(renderer.GetHeight() / 2.0f);
	}

	m_sFogFrameTwo.m_pForeground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\foregroundfog.png");

	if (!(m_sFogFrameTwo.m_pForeground))
	{
		LogManager::GetInstance().Log("Foreground fog frame two failed to initialise!");
		return false;
	}
	else
	{
		m_sFogFrameTwo.m_pForeground->SetX((renderer.GetWidth() + (m_sFogFrameTwo.m_pForeground->GetWidth() / 2.0f)));
		m_sFogFrameTwo.m_pForeground->SetY(renderer.GetHeight() / 2.0f);
	}

	m_sFogFrameTwo.m_pGround = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\groundfog.png");

	if (!(m_sFogFrameTwo.m_pGround))
	{
		LogManager::GetInstance().Log("Ground fog frame two failed to initialise!");
		return false;
	}
	else
	{
		m_sFogFrameTwo.m_pGround->SetX((renderer.GetWidth() + (m_sFogFrameTwo.m_pGround->GetWidth() / 2.0f)));
		m_sFogFrameTwo.m_pGround->SetY(renderer.GetHeight() / 2.0f);
	}

	return true;
}

void
ForestDay::UpdateBackground()
{
	// Get camera vector
	Vector2* camoffset = camera.GetOffset();

	switch (m_iBackground)
	{
	case 0: // Day scene
		// Update frame one sprites x-coordinate
		m_sDayFrameOne.m_pBackground->SetX(((m_fWindowWidth / 2.0f) - (fmod((camoffset->x / 10.0f), m_fLoopRange))));
		m_sDayFrameOne.m_pForeground->SetX(((m_fWindowWidth / 2.0f) - (fmod((camoffset->x / 2.0f), m_fLoopRange))));
		m_sDayFrameOne.m_pGround->SetX(((m_fWindowWidth / 2.0f) - (fmod(camoffset->x, m_fLoopRange))));

		// Update frame two sprites x-coordinate
		m_sDayFrameTwo.m_pBackground->SetX((m_sDayFrameOne.m_pBackground->GetX() + m_sDayFrameOne.m_pBackground->GetWidth()));
		m_sDayFrameTwo.m_pForeground->SetX((m_sDayFrameOne.m_pForeground->GetX() + m_sDayFrameOne.m_pForeground->GetWidth()));
		m_sDayFrameTwo.m_pGround->SetX((m_sDayFrameOne.m_pGround->GetX() + m_sDayFrameOne.m_pGround->GetWidth()));

		// Loop background frame one sprite
		if (m_sDayFrameOne.m_pBackground->GetX() < -(m_sDayFrameOne.m_pBackground->GetWidth() / 2.0f))
		{
			m_sDayFrameOne.m_pBackground->SetX((m_sDayFrameTwo.m_pBackground->GetX() + m_sDayFrameTwo.m_pBackground->GetWidth()));
		}

		// Loop foreground frame one sprite
		if (m_sDayFrameOne.m_pForeground->GetX() < -(m_sDayFrameOne.m_pForeground->GetWidth() / 2.0f))
		{
			m_sDayFrameOne.m_pForeground->SetX((m_sDayFrameTwo.m_pForeground->GetX() + m_sDayFrameTwo.m_pForeground->GetWidth()));
		}

		// Loop ground frame one sprite
		if (m_sDayFrameOne.m_pGround->GetX() < -(m_sDayFrameOne.m_pGround->GetWidth() / 2.0f))
		{
			m_sDayFrameOne.m_pGround->SetX((m_sDayFrameTwo.m_pGround->GetX() + m_sDayFrameTwo.m_pGround->GetWidth()));
		}

		// Loop background frame two sprite
		if (m_sDayFrameTwo.m_pBackground->GetX() < -(m_sDayFrameTwo.m_pBackground->GetWidth() / 2.0f))
		{
			m_sDayFrameTwo.m_pBackground->SetX((m_sDayFrameOne.m_pBackground->GetX() + m_sDayFrameOne.m_pBackground->GetWidth()));
		}

		// Loop foreground frame two sprite
		if (m_sDayFrameTwo.m_pForeground->GetX() < -(m_sDayFrameTwo.m_pForeground->GetWidth() / 2.0f))
		{
			m_sDayFrameTwo.m_pForeground->SetX((m_sDayFrameOne.m_pForeground->GetX() + m_sDayFrameOne.m_pForeground->GetWidth()));
		}

		// Loop ground frame two sprite
		if (m_sDayFrameTwo.m_pGround->GetX() < -(m_sDayFrameTwo.m_pGround->GetWidth() / 2.0f))
		{
			m_sDayFrameTwo.m_pGround->SetX((m_sDayFrameOne.m_pGround->GetX() + m_sDayFrameOne.m_pGround->GetWidth()));
		}
		break;
	case 1: // Fog scene
		// Update frame one sprites x-coordinate
		m_sFogFrameOne.m_pBackground->SetX(((m_fWindowWidth / 2.0f) - (fmod((camoffset->x / 10.0f), m_fLoopRange))));
		m_sFogFrameOne.m_pForeground->SetX(((m_fWindowWidth / 2.0f) - (fmod((camoffset->x / 2.0f), m_fLoopRange))));
		m_sFogFrameOne.m_pGround->SetX(((m_fWindowWidth / 2.0f) - (fmod(camoffset->x, m_fLoopRange))));

		// Update frame two sprites x-coordinate
		m_sFogFrameTwo.m_pBackground->SetX((m_sFogFrameOne.m_pBackground->GetX() + m_sFogFrameOne.m_pBackground->GetWidth()));
		m_sFogFrameTwo.m_pForeground->SetX((m_sFogFrameOne.m_pForeground->GetX() + m_sFogFrameOne.m_pForeground->GetWidth()));
		m_sFogFrameTwo.m_pGround->SetX((m_sFogFrameOne.m_pGround->GetX() + m_sFogFrameOne.m_pGround->GetWidth()));

		// Loop background frame one sprite
		if (m_sFogFrameOne.m_pBackground->GetX() < -(m_sFogFrameOne.m_pBackground->GetWidth() / 2.0f))
		{
			m_sFogFrameOne.m_pBackground->SetX((m_sFogFrameTwo.m_pBackground->GetX() + m_sFogFrameTwo.m_pBackground->GetWidth()));
		}

		// Loop foreground frame one sprite
		if (m_sFogFrameOne.m_pForeground->GetX() < -(m_sFogFrameOne.m_pForeground->GetWidth() / 2.0f))
		{
			m_sFogFrameOne.m_pForeground->SetX((m_sFogFrameTwo.m_pForeground->GetX() + m_sFogFrameTwo.m_pForeground->GetWidth()));
		}

		// Loop ground frame one sprite
		if (m_sFogFrameOne.m_pGround->GetX() < -(m_sFogFrameOne.m_pGround->GetWidth() / 2.0f))
		{
			m_sFogFrameOne.m_pGround->SetX((m_sFogFrameTwo.m_pGround->GetX() + m_sFogFrameTwo.m_pGround->GetWidth()));
		}

		// Loop background frame two sprite
		if (m_sFogFrameTwo.m_pBackground->GetX() < -(m_sFogFrameTwo.m_pBackground->GetWidth() / 2.0f))
		{
			m_sFogFrameTwo.m_pBackground->SetX((m_sFogFrameOne.m_pBackground->GetX() + m_sFogFrameOne.m_pBackground->GetWidth()));
		}

		// Loop foreground frame two sprite
		if (m_sFogFrameTwo.m_pForeground->GetX() < -(m_sFogFrameTwo.m_pForeground->GetWidth() / 2.0f))
		{
			m_sFogFrameTwo.m_pForeground->SetX((m_sFogFrameOne.m_pForeground->GetX() + m_sFogFrameOne.m_pForeground->GetWidth()));
		}

		// Loop ground frame two sprite
		if (m_sFogFrameTwo.m_pGround->GetX() < -(m_sFogFrameTwo.m_pGround->GetWidth() / 2.0f))
		{
			m_sFogFrameTwo.m_pGround->SetX((m_sFogFrameOne.m_pGround->GetX() + m_sFogFrameOne.m_pGround->GetWidth()));
		}
		break;
	}
}
// Changes made by Karl - End
void
ForestDay::DebugDraw()
{

}
