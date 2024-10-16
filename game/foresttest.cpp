// COMP710 GP Framework 2022

// This include:
#include "foresttest.h"

// Local includes:
#include "game.h"
#include "logmanager.h"
#include "renderer.h"
#include "inputsystem.h"
#include "sprite.h"
#include "forest.h"
#include "character.h"
#include "player.h"
#include "golem.h"
#include "mushroom.h"
#include "skeleton.h" // Changes made by Karl
#include "spider.h"

// Library includes:
#include <iostream>
#include <ctime>
#include "Camera.h"

// Static members:
//int ForestTest::sm_iSegmentWidth = 0;

ForestTest::ForestTest(b2World* world, Player* character)
	: m_pSkyFrameOne(nullptr) // Changes made by Karl
	, m_pSkyFrameTwo(nullptr)
	, m_pTreesFrameOne(nullptr)
	, m_pTreesFrameTwo(nullptr)
	, m_pGroundFrameOne(nullptr)
	, m_pGroundFrameTwo(nullptr)
	, m_pGolem(0)
	, m_pMushroom(0)
	, m_pSkeleton(0) // Changes made by Karl
	, m_pSpider(0)
	, m_pWorld(world)
	, camera()
	, m_pCharacter(character)
	, ground(nullptr)
	, leftWall(nullptr)
	, rightWall(nullptr)
	, platform(nullptr)
	, m_fWindowWidth(0.0f) // Changes made by Karl
	, m_fWindowHeight(0.0f)
{

}

ForestTest::~ForestTest()
{	// Changes made by Karl - Start
	delete m_pSkyFrameOne;
	m_pSkyFrameOne = nullptr;
	
	delete m_pSkyFrameTwo;
	m_pSkyFrameTwo = nullptr;

	delete m_pTreesFrameOne;
	m_pTreesFrameOne = nullptr;

	delete m_pTreesFrameTwo;
	m_pTreesFrameTwo = nullptr;

	delete m_pGroundFrameOne;
	m_pGroundFrameOne = nullptr;

	delete m_pGroundFrameTwo;
	m_pGroundFrameTwo = nullptr;
	// Changes made by Karl - End
	delete m_pGolem;
	m_pGolem = 0;

	delete m_pMushroom;
	m_pMushroom = 0;

	delete m_pSkeleton;
	m_pSkeleton = 0;

	delete m_pSpider;
	m_pSpider = 0;
	
}

bool
ForestTest::Initialise(Renderer& renderer)
{

	//Made by Rauen
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
	float worldHeight = 10000.0f / SCALE;  // World height converted to meters
	m_fWindowHeight = renderer.GetHeight(); // Changes made by Karl - made width and height class variables
	m_fWindowWidth = renderer.GetWidth();

	// Set the Y position to the bottom of the window and convert to meters
	//float groundY = (windowHeight - terrainHeight) / SCALE;  // Convert to meters

	// Create the ground object, converting width/height to meters
	ground = new Terrain(m_pWorld, 0.0f, m_fWindowHeight /SCALE, worldWidth, terrainHeight, GROUND);
	m_terrainSegments.push_back(ground);  // Ground
	ground->Initialise(renderer);

	//platform = new Terrain(m_pWorld, 1200.0 / SCALE, groundY - terrainHeight, terrainWidth, terrainHeight);
	//m_terrainSegments.push_back(platform);  // Another platform
	//platform->SetSprite(renderer, PLATFORM, terrainWidth * SCALE, terrainHeight * SCALE);

	// Add a left wall
	//leftWall = new Terrain(m_pWorld, 0.0f, 0.0f, wallWidth, wallHeight);
	//m_terrainSegments.push_back(leftWall);  // Left boundary
	//leftWall->SetSprite(renderer, LEFT_WALL, wallWidth * SCALE, wallHeight * 2* SCALE);

	//// Add a right wall 
	//rightWall = new Terrain(m_pWorld, worldWidth, 0.0f, wallWidth, wallHeight);
	//m_terrainSegments.push_back(rightWall);  // Right boundary
	//rightWall->SetSprite(renderer, RIGHT_WALL, wallWidth * SCALE, wallHeight * SCALE);

	//m_terrainSegments.push_back(new Terrain(m_pWorld, 450.0f, 450.0f, terrainWidth, terrainHeight));  // Elevated platform


	camera.SetCamera(m_fWindowWidth, m_fWindowHeight, worldWidth, worldHeight);
	SetEnemies(renderer);

	return true;
}

void
ForestTest::Process(float deltaTime, InputSystem& inputSystem)
{
	//if (m_pCharacter->IsDefined()) // Changes made by Karl
	//{
	//	m_pCharacter->Process(deltaTime, inputSystem);
	//}
	// Changes made by Karl - Start - Background looping functionality added
	// Get camera vector
	Vector2* camoffset = camera.GetOffset();
	
	// Range of loop
	float range = m_pSkyFrameOne->GetWidth() * 2.0f;
	
	// Update frame one sprites x-coordinate
	m_pSkyFrameOne->SetX(((m_fWindowWidth / 2.0f) - (fmod((camoffset->x / 10.0f), range))));
	m_pTreesFrameOne->SetX(((m_fWindowWidth / 2.0f) - (fmod((camoffset->x / 2.0f), range))));
	m_pGroundFrameOne->SetX(((m_fWindowWidth / 2.0f) - (fmod(camoffset->x, range))));

	// Update frame two sprites x-coordinate
	m_pSkyFrameTwo->SetX((m_pSkyFrameOne->GetX() + m_pSkyFrameOne->GetWidth()));
	m_pTreesFrameTwo->SetX((m_pTreesFrameOne->GetX() + m_pTreesFrameOne->GetWidth()));
	m_pGroundFrameTwo->SetX((m_pGroundFrameOne->GetX() + m_pGroundFrameOne->GetWidth()));

	// Loop sky frame one sprite
	if (m_pSkyFrameOne->GetX() < -(m_pSkyFrameOne->GetWidth() / 2.0f))
	{
		m_pSkyFrameOne->SetX((m_pSkyFrameTwo->GetX() + m_pSkyFrameTwo->GetWidth()));
	}

	// Loop trees frame one sprite
	if (m_pTreesFrameOne->GetX() < -(m_pTreesFrameOne->GetWidth() / 2.0f))
	{
		m_pTreesFrameOne->SetX((m_pTreesFrameTwo->GetX() + m_pTreesFrameTwo->GetWidth()));
	}

	// Loop ground frame one sprite
	if (m_pGroundFrameOne->GetX() < -(m_pGroundFrameOne->GetWidth() / 2.0f))
	{
		m_pGroundFrameOne->SetX((m_pGroundFrameTwo->GetX() + m_pGroundFrameTwo->GetWidth()));
	}

	// Loop sky frame two sprite
	if (m_pGroundFrameTwo->GetX() < -(m_pGroundFrameTwo->GetWidth() / 2.0f))
	{
		m_pSkyFrameTwo->SetX((m_pSkyFrameOne->GetX() + m_pSkyFrameOne->GetWidth()));
	}

	// Loop trees frame two sprite
	if (m_pTreesFrameTwo->GetX() < -(m_pTreesFrameTwo->GetWidth() / 2.0f))
	{
		m_pTreesFrameTwo->SetX((m_pTreesFrameOne->GetX() + m_pTreesFrameOne->GetWidth()));
	}

	// Loop ground frame two sprite
	if (m_pGroundFrameTwo->GetX() < -(m_pGroundFrameTwo->GetWidth() / 2.0f))
	{
		m_pGroundFrameTwo->SetX((m_pGroundFrameOne->GetX() + m_pGroundFrameOne->GetWidth()));
	}
	// Changes made by Karl - End
	m_pCharacter->Process(deltaTime, inputSystem);
	m_pGolem->Process(deltaTime, inputSystem);
	m_pMushroom->Process(deltaTime, inputSystem);
	//m_pSkeleton->Process(deltaTime, inputSystem); // Changes made by Karl
	//m_pSpider->Process(deltaTime, inputSystem);
	camera.Update(*m_pCharacter);
	
	//printf("char: %f\n", m_pCharacter->GetPosition().x - platform->GetPosition().x);
}

void
ForestTest::Draw(Renderer& renderer)
{	// Changes made by Karl - Start - Draw layered background sprites
	// Sky frames
	m_pSkyFrameOne->Draw(renderer, false, true);
	m_pSkyFrameTwo->Draw(renderer, false, true);

	// Trees frames
	m_pTreesFrameOne->Draw(renderer, false, true);
	m_pTreesFrameTwo->Draw(renderer, false, true);

	// Ground frames
	m_pGroundFrameOne->Draw(renderer, false, true);
	m_pGroundFrameTwo->Draw(renderer, false, true);
	// Changes made by Karl - End
	m_pCharacter->DrawWithCam(renderer, camera);
	//printf("Char: %f\n", m_pCharacter->GetPosition().x);

	for (auto* terrain : m_terrainSegments) {
		terrain->Draw(renderer, camera);  // Pass the camera object to adjust positions based on the camera's position
	}

	m_pGolem->Draw(renderer, camera);
	m_pMushroom->Draw(renderer, camera);
	//m_pSkeleton->Draw(renderer, camera); // Changes made by Karl
	//m_pSpider->Draw(renderer, camera);
}

//Testing stuff with enemies for later
bool
ForestTest::SetEnemies(Renderer& renderer)
{
	m_pGolem = new Golem(m_pWorld);
	m_pGolem->SetCamera(&camera);
	m_pGolem->SetPlayer(m_pCharacter);

	if (!(m_pGolem->Initialise(renderer)))
	{
		LogManager::GetInstance().Log("Golem failed to initialise!");
		return false;
	}

	m_pMushroom = new Mushroom(m_pWorld);
	m_pMushroom->SetCamera(&camera);
	m_pMushroom->SetPlayer(m_pCharacter);

	if (!(m_pMushroom->Initialise(renderer)))
	{
		LogManager::GetInstance().Log("Mushroom failed to initialise!");
		return false;
	}
	/*
	m_pSkeleton = new Skeleton(m_pWorld);
	m_pSkeleton->SetCamera(&camera);
	m_pSkeleton->SetPlayer(m_pCharacter);

	if (!(m_pSkeleton->Initialise(renderer)))
	{
		LogManager::GetInstance().Log("Skeleton failed to initialise!");
		return false;
	}

	m_pSpider = new Spider(m_pWorld);
	m_pSpider->SetCamera(&camera);
	m_pSpider->SetPlayer(m_pCharacter);

	if (!(m_pSpider->Initialise(renderer)))
	{
		LogManager::GetInstance().Log("Spider failed to initialise!");
		return false;
	}*/

	return true;
}
// Changes made by Karl - Start - Function to set all background sprites for layering
bool
ForestTest::SetBGSprites(Renderer& renderer)
{
	// Frame one
	m_pSkyFrameOne = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\sky.png");

	if (!(m_pSkyFrameOne))
	{
		LogManager::GetInstance().Log("Sky frame one failed to initialise!");
		return false;
	}
	else
	{
		m_pSkyFrameOne->SetX(renderer.GetWidth() / 2.0f);
		m_pSkyFrameOne->SetY(renderer.GetHeight() / 2.0f);
	}

	m_pTreesFrameOne = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\trees.png");

	if (!(m_pTreesFrameOne))
	{
		LogManager::GetInstance().Log("Trees frame one failed to initialise!");
		return false;
	}
	else
	{
		m_pTreesFrameOne->SetX(renderer.GetWidth() / 2.0f);
		m_pTreesFrameOne->SetY(renderer.GetHeight() / 2.0f);
	}

	m_pGroundFrameOne = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\ground.png");

	if (!(m_pGroundFrameOne))
	{
		LogManager::GetInstance().Log("Ground frame one failed to initialise!");
		return false;
	}
	else
	{
		m_pGroundFrameOne->SetX(renderer.GetWidth() / 2.0f);
		m_pGroundFrameOne->SetY(renderer.GetHeight() / 2.0f);
	}

	// Frame two
	m_pSkyFrameTwo = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\sky.png");

	if (!(m_pSkyFrameTwo))
	{
		LogManager::GetInstance().Log("Sky frame two failed to initialise!");
		return false;
	}
	else
	{
		m_pSkyFrameTwo->SetX((renderer.GetWidth() + (m_pSkyFrameTwo->GetWidth() / 2.0f)));
		m_pSkyFrameTwo->SetY(renderer.GetHeight() / 2.0f);
	}

	m_pTreesFrameTwo = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\trees.png");

	if (!(m_pTreesFrameTwo))
	{
		LogManager::GetInstance().Log("Trees frame two failed to initialise!");
		return false;
	}
	else
	{
		m_pTreesFrameTwo->SetX((renderer.GetWidth() + (m_pTreesFrameTwo->GetWidth() / 2.0f)));
		m_pTreesFrameTwo->SetY(renderer.GetHeight() / 2.0f);
	}

	m_pGroundFrameTwo = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\ground.png");

	if (!(m_pGroundFrameTwo))
	{
		LogManager::GetInstance().Log("Ground frame two failed to initialise!");
		return false;
	}
	else
	{
		m_pGroundFrameTwo->SetX((renderer.GetWidth() + (m_pGroundFrameTwo->GetWidth() / 2.0f)));
		m_pGroundFrameTwo->SetY(renderer.GetHeight() / 2.0f);
	}

	return true;
}
// Changes made by Karl - End
void
ForestTest::DebugDraw()
{

}
