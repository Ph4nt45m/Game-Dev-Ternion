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
{

}

ForestScene::~ForestScene()
{
}

bool
ForestScene::Initialise(Renderer& renderer)
{
	//m_iShowCount = 10;
	//srand((int)time(0));

	//int prevIndex = 0;
	////bool closedSegment = false; // Temporary until floating platform is implemented

	//for (int i = 0; i < m_iShowCount; i++)
	//{
	//	if (i == 0 || i == (m_iShowCount - 1))
	//	{	// Changes made by Karl - Start
	//		m_pFrtSegments[i] = new Forest();
	//		m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("Sprites\\terrainbase3D\\boundary.png"));
	//		m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2)));
	//		m_pFrtSegments[i]->SetIndex(i);
	//		m_pFrtSegments[i]->SetType(ST_BOUNDARY);
	//		m_pFrtSegments[i]->SetAlive(true);
	//	}
	//	else if (i == 1)
	//	{
	//		m_pFrtSegments[i] = new Forest();
	//		m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("Sprites\\terrainbase3D\\leftwall.png"));
	//		m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
	//		m_pFrtSegments[i]->SetIndex(i);
	//		m_pFrtSegments[i]->SetType(ST_LEFTWALL);
	//		m_pFrtSegments[i]->SetAlive(true);
	//	}
	//	else if (i == ((m_iShowCount - 1) - 1))
	//	{
	//		m_pFrtSegments[i] = new Forest();
	//		m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("Sprites\\terrainbase3D\\rightwall.png"));
	//		m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
	//		m_pFrtSegments[i]->SetIndex(i);
	//		m_pFrtSegments[i]->SetType(ST_RIGHTWALL);
	//		m_pFrtSegments[i]->SetAlive(true);
	//	}
	//	else if (i == 2 || i == 3 || i == ((m_iShowCount - 1) - 2) || i == ((m_iShowCount - 1) - 3))
	//	{
	//		m_pFrtSegments[i] = new Forest();
	//		m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("Sprites\\terrainbase3D\\blank.png"));
	//		m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
	//		m_pFrtSegments[i]->SetIndex(i);
	//		m_pFrtSegments[i]->SetType(ST_BLANK);
	//		m_pFrtSegments[i]->SetAlive(true);
	//	}
	//	else
	//	{
	//		//int index = (rand() % 4);
	//		int index = 3;

	//		if (index == 0)
	//		{
	//			// Temporary 'if' statement until double jump is implemented
	//			if (prevIndex == 0 || prevIndex == 1)
	//			{
	//				m_pFrtSegments[i] = new Forest();
	//				m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("Sprites\\terrainbase3D\\blank.png"));
	//				m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
	//				m_pFrtSegments[i]->SetIndex(i);
	//				m_pFrtSegments[i]->SetType(ST_BLANK);
	//				m_pFrtSegments[i]->SetAlive(true);
	//				prevIndex = index;
	//				//closedSegment = false;
	//			}
	//			else
	//			{
	//				i--;
	//			}
	//		}
	//		else if (index == 1)
	//		{
	//			if (prevIndex == 0 || prevIndex == 1 || prevIndex == 3) //  || !closedSegment add for floating platform
	//			{
	//				m_pFrtSegments[i] = new Forest();
	//				m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("Sprites\\terrainbase3D\\tallplatform.png"));
	//				m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
	//				m_pFrtSegments[i]->SetIndex(i);
	//				m_pFrtSegments[i]->SetType(ST_PLATFORM);
	//				m_pFrtSegments[i]->SetAlive(true);
	//				prevIndex = index;
	//				//closedSegment = true;
	//			}
	//			else
	//			{
	//				i--;
	//			}
	//		}
	//		else if (index == 2)
	//		{
	//			//m_pFrtSegments[i] = new Forest();
	//			//m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("Sprites\\terrainbse3D\\floatingplatform.png"));
	//			////m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\boundary.png"));
	//			//m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2)));
	//			//m_pFrtSegments[i]->SetIndex(i);
	//			// m_pFrtSegments[i]->SetType(ST_FLOATINGPLATFORM);
	//			//m_pFrtSegments[i]->SetAlive(true);
	//			//prevIndex = index;

	//			i--; // Temporary until logic collision, terrain transparency, and double jump are implemented
	//		}
	//		else if (index == 3)
	//		{
	//			//  ( !closedSegment ) add for floating platform, ( prevIndex == 0 ) add for double jump
	//			if (prevIndex == 0 || prevIndex == 1 || prevIndex == 3)
	//			{
	//				m_pFrtSegments[i] = new Forest();
	//				m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("Sprites\\terrainbase3D\\tallplatform.png")); // Changes made by Karl - End
	//				m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
	//				m_pFrtSegments[i]->SetIndex(i);
	//				m_pFrtSegments[i]->SetType(ST_TALLPLATFORM);
	//				m_pFrtSegments[i]->SetAlive(true);
	//				prevIndex = index;
	//				//closedSegment = true;
	//			}
	//			else
	//			{
	//				i--;
	//			}
	//		}
	//	}

	//	if (m_pFrtSegments[i]->IsAlive())
	//	{
	//		m_pFrtSegments[i]->SetY((m_pFrtSegments[i]->GetHeight() / 2));
	//		m_pFrtSegments[i]->SetNumSegments(m_iShowCount);
	//		m_pFrtSegments[i]->Initialise(renderer);
	//	}
	//}

//Made by Rauen
	  // Define some terrain segments at different positions and sizes
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

	
	platform = new Terrain(m_pWorld, 1000.0f/ SCALE, groundY - terrainHeight, terrainWidth, terrainHeight);
	m_terrainSegments.push_back(platform);  // Another platform
	platform->SetSprite(renderer, PLATFORM, terrainWidth * SCALE, terrainHeight*SCALE);
	
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

	return true;
}

void
ForestScene::Process(float deltaTime, InputSystem& inputSystem)
{
	//m_pGolem->Process(deltaTime, inputSystem);
	camera.Update(*m_pCharacter);
	//printf("char: %f\n", m_pCharacter->GetPosition().x - platform->GetPosition().x);
}

void
ForestScene::Draw(Renderer& renderer)
{
	
	// Update camera based on player position
	m_pCharacter->DrawWithCam(renderer, camera);
	//printf("Char: %f\n", m_pCharacter->GetPosition().x);

	ground->Draw(renderer);
	platform->Draw(renderer);
	leftWall->Draw(renderer);
	rightWall->Draw(renderer);

	//m_pGolem->Draw(renderer);
}

void
ForestScene::SetCharPos(Vector2& characterPos)
{
	//m_vCharPos = &characterPos;
}

void
ForestScene::SetCharFeetPos(Vector2& feetPos)
{
	//m_vFeetPos = &feetPos;
}

void
ForestScene::SetCharWidth(int width)
{
	//m_iCharWidth = width;
}

bool
ForestScene::SetEnemies(Character& character, Renderer& renderer)
{
	/*m_pGolem = new Golem(m_pWorld);

	if (!(m_pGolem->Initialise(renderer)))
	{
		LogManager::GetInstance().Log("Golem failed to initialise!");
		return false;
	}
	else
	{
		m_pGolem->SetCharacter(character);
	}*/

	return true;
}

void
ForestScene::DebugDraw()
{

}
