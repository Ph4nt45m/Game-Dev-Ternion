// COMP710 GP Framework 2022

// This include:
#include "forestscene.h"

// Local includes:
#include "game.h"
#include "logmanager.h"
#include "renderer.h"
#include "inputsystem.h"
#include "sprite.h"
#include "forest.h"
#include "character.h"
#include "golem.h"

// Library includes:
#include <iostream>
#include <ctime>
#include "Camera.h"

// Static members:
//int ForestScene::sm_iSegmentWidth = 0;

ForestScene::ForestScene(b2World* world, Character* character)
	: m_pGolem(0)
	, m_pWorld(world)
	, camera()
	, m_pCharacter(character)
	, ground(nullptr)
	, leftWall(nullptr)
	, rightWall(nullptr)
	, platform(nullptr)
{

}

ForestScene::~ForestScene()
{
}

bool
ForestScene::Initialise(Renderer& renderer)
{

//Made by Rauen
	  // Define some terrain segments at different positions and sizes
	Game::GetInstance().SetGravity(0.0f, 1.0f);
	m_pWorld->SetGravity(Game::GetInstance().GetGravity());

	const float SCALE = 30.0f;
	float terrainWidth = 300.0f / SCALE;  // Convert pixel width to meters
	float terrainHeight = 200.0f / SCALE;  // Convert pixel height to meters
	float wallWidth = 10.0f / SCALE;  // Thin wall, converted to meters
	float wallHeight = 1000.0f / SCALE;  // Wall height in meters
	float worldWidth = 50000.0f / SCALE;  // World width converted to meters
	float worldHeight = 1000.0f / SCALE;  // World height converted to meters
	float windowHeight = renderer.GetHeight();
	float windowWidth = renderer.GetWidth();

	// Set the Y position to the bottom of the window and convert to meters
	float groundY = (windowHeight - terrainHeight) / SCALE;  // Convert to meters

	// Create the ground object, converting width/height to meters
	ground = new Terrain(m_pWorld, 0.0f, groundY, worldWidth, terrainHeight);
	m_terrainSegments.push_back(ground);  // Ground
	ground->SetSprite(renderer, GROUND, worldWidth * SCALE, terrainHeight * SCALE);

	platform = new Terrain(m_pWorld, 1200.0/SCALE, groundY - terrainHeight, terrainWidth, terrainHeight);
	m_terrainSegments.push_back(platform);  // Another platform
	platform->SetSprite(renderer, PLATFORM, terrainWidth * SCALE, terrainHeight * SCALE);
	
	// Add a left wall
	leftWall = new Terrain(m_pWorld, 0.0f, groundY, wallWidth, wallHeight);
	m_terrainSegments.push_back(leftWall);  // Left boundary
	leftWall->SetSprite(renderer, LEFT_WALL, wallWidth * SCALE, wallHeight * SCALE);
	
	// Add a right wall 
	rightWall = new Terrain(m_pWorld, worldWidth, groundY, wallWidth, wallHeight);
	m_terrainSegments.push_back(rightWall);  // Right boundary
	rightWall->SetSprite(renderer, RIGHT_WALL, wallWidth * SCALE, wallHeight * SCALE);

	//m_terrainSegments.push_back(new Terrain(m_pWorld, 450.0f, 450.0f, terrainWidth, terrainHeight));  // Elevated platform


	camera.SetCamera(windowWidth, windowHeight, worldWidth, worldHeight);
	SetEnemies(renderer);

	return true;
}

void
ForestScene::Process(float deltaTime, InputSystem& inputSystem)
{
	m_pCharacter->Process(deltaTime, inputSystem);
	m_pGolem->Process(deltaTime, inputSystem);
	camera.Update(*m_pCharacter);
	//printf("char: %f\n", m_pCharacter->GetPosition().x - platform->GetPosition().x);
}

void
ForestScene::Draw(Renderer& renderer)
{
	
	// Update camera based on player position
	m_pCharacter->DrawWithCam(renderer, camera);
	//printf("Char: %f\n", m_pCharacter->GetPosition().x);

	for (auto* terrain : m_terrainSegments) {
		terrain->Draw(renderer, camera);  // Pass the camera object to adjust positions based on the camera's position
	}

	m_pGolem->Draw(renderer, camera);
}

//Testing stuff with enemies for later
bool
ForestScene::SetEnemies(Renderer& renderer)
{
	m_pGolem = new Golem(m_pWorld);
	m_pGolem->SetCamera(&camera);
	m_pGolem->SetPlayer(m_pCharacter);

	if (!(m_pGolem->Initialise(renderer)))
	{
		LogManager::GetInstance().Log("Golem failed to initialise!");
		return false;
	}

	return true;
}

void
ForestScene::DebugDraw()
{

}
