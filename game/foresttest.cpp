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
	: m_pBackground(nullptr)
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
{

}

ForestTest::~ForestTest()
{
	delete m_pBackground;
	m_pBackground = nullptr;
	// Changes made by Karl - Start
	delete m_pGolem;
	m_pGolem = 0;

	delete m_pMushroom;
	m_pMushroom = 0;

	delete m_pSkeleton;
	m_pSkeleton = 0;

	delete m_pSpider;
	m_pSpider = 0;
	// Changes made by Karl - End
}

bool
ForestTest::Initialise(Renderer& renderer)
{

	//Made by Rauen
	// Define some terrain segments at different positions and sizes
	Game::GetInstance().SetGravity(0.0f, 1.0f);
	m_pWorld->SetGravity(Game::GetInstance().GetGravity());

	// Set background sprite
	m_pBackground = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\forestday.png");
	m_pBackground->SetX(renderer.GetWidth() / 2.0f);
	m_pBackground->SetY(renderer.GetHeight() / 2.0f);

	//const float SCALE = 30.0f;
	float terrainWidth = 300.0f / SCALE;  // Convert pixel width to meters
	float terrainHeight = 100.0f / SCALE;  // Convert pixel height to meters
	float wallWidth = 100.0f / SCALE;  // Thin wall, converted to meters
	float wallHeight = 10000.0f / SCALE;  // Wall height in meters
	float worldWidth = 500000.0f / SCALE;  // World width converted to meters
	float worldHeight = 10000.0f / SCALE;  // World height converted to meters
	float windowHeight = renderer.GetHeight();
	float windowWidth = renderer.GetWidth();

	// Set the Y position to the bottom of the window and convert to meters
	//float groundY = (windowHeight - terrainHeight) / SCALE;  // Convert to meters

	// Create the ground object, converting width/height to meters
	ground = new Terrain(m_pWorld, 0.0f, windowHeight/SCALE, worldWidth, terrainHeight, GROUND);
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


	camera.SetCamera(windowWidth, windowHeight, worldWidth, worldHeight);
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
{
	m_pBackground->Draw(renderer, false, true);
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

void
ForestTest::DebugDraw()
{

}
