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

// Static members:
//int ForestScene::sm_iSegmentWidth = 0;

ForestScene::ForestScene()
	: m_pFrtSegments{ 0 }
	, m_pGolem(0)
{

}

ForestScene::~ForestScene()
{
	for (int i = 0; i < m_iShowCount; i++)
	{
		delete m_pFrtSegments[i];
		m_pFrtSegments[i] = 0;
	}
}

bool
ForestScene::Initialise(Renderer& renderer)
{
	m_iShowCount = 20;
	srand((int)time(0));

	int prevIndex = 0;
	//bool closedSegment = false; // Temporary until floating platform is implemented

	for (int i = 0; i < m_iShowCount; i++)
	{
		if (i == 0 || i == (m_iShowCount - 1))
		{
			m_pFrtSegments[i] = new Forest();
			m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\terrainforest2D\\boundary2D.png"));
			m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2)));
			m_pFrtSegments[i]->SetIndex(i);
			m_pFrtSegments[i]->SetType(ST_BOUNDARY);
			m_pFrtSegments[i]->SetAlive(true);
		}
		else if (i == 1)
		{
			m_pFrtSegments[i] = new Forest();
			m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\terrainforest2D\\blank2D.png"));
			m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
			m_pFrtSegments[i]->SetIndex(i);
			m_pFrtSegments[i]->SetType(ST_LEFTWALL);
			m_pFrtSegments[i]->SetAlive(true);
		}
		else if (i == ((m_iShowCount - 1) - 1))
		{
			m_pFrtSegments[i] = new Forest();
			m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\terrainforest2D\\blank2D.png"));
			m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
			m_pFrtSegments[i]->SetIndex(i);
			m_pFrtSegments[i]->SetType(ST_RIGHTWALL);
			m_pFrtSegments[i]->SetAlive(true);
		}
		else if (i == 2 || i == 3 || i == ((m_iShowCount - 1) - 2) || i == ((m_iShowCount - 1) - 3))
		{
			m_pFrtSegments[i] = new Forest();
			m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\terrainforest2D\\blank2D.png"));
			m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
			m_pFrtSegments[i]->SetIndex(i);
			m_pFrtSegments[i]->SetType(ST_BLANK);
			m_pFrtSegments[i]->SetAlive(true);
		}
		else
		{
			//int index = (rand() % 4);
			int index = 3;

			if (index == 0)
			{
				// Temporary 'if' statement until double jump is implemented
				if (prevIndex == 0 || prevIndex == 1)
				{
					m_pFrtSegments[i] = new Forest();
					m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\terrainforest2D\\blank2D.png"));
					m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
					m_pFrtSegments[i]->SetIndex(i);
					m_pFrtSegments[i]->SetType(ST_BLANK);
					m_pFrtSegments[i]->SetAlive(true);
					prevIndex = index;
					//closedSegment = false;
				}
				else
				{
					i--;
				}
			}
			else if (index == 1)
			{
				if (prevIndex == 0 || prevIndex == 1 || prevIndex == 3) //  || !closedSegment add for floating platform
				{
					m_pFrtSegments[i] = new Forest();
					m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\terrainforest2D\\platform2D.png"));
					m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
					m_pFrtSegments[i]->SetIndex(i);
					m_pFrtSegments[i]->SetType(ST_PLATFORM);
					m_pFrtSegments[i]->SetAlive(true);
					prevIndex = index;
					//closedSegment = true;
				}
				else
				{
					i--;
				}
			}
			else if (index == 2)
			{
				//m_pFrtSegments[i] = new Forest();
				//m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\floatingplatform.png"));
				////m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\boundary.png"));
				//m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2)));
				//m_pFrtSegments[i]->SetIndex(i);
				// m_pFrtSegments[i]->SetType(ST_FLOATINGPLATFORM);
				//m_pFrtSegments[i]->SetAlive(true);
				//prevIndex = index;

				i--; // Temporary until logic collision, terrain transparency, and double jump are implemented
			}
			else if (index == 3)
			{
				//  ( !closedSegment ) add for floating platform, ( prevIndex == 0 ) add for double jump
				if (prevIndex == 0 || prevIndex == 1 || prevIndex == 3)
				{
					m_pFrtSegments[i] = new Forest();
					m_pFrtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\terrainforest2D\\tallplatform2D.png"));
					m_pFrtSegments[i]->SetX(((sm_iSegmentWidth * i) - (sm_iSegmentWidth / 2) - (6 * i)));
					m_pFrtSegments[i]->SetIndex(i);
					m_pFrtSegments[i]->SetType(ST_TALLPLATFORM);
					m_pFrtSegments[i]->SetAlive(true);
					prevIndex = index;
					//closedSegment = true;
				}
				else
				{
					i--;
				}
			}
		}

		if (m_pFrtSegments[i]->IsAlive())
		{
			m_pFrtSegments[i]->SetY((m_pFrtSegments[i]->GetHeight() / 2));
			m_pFrtSegments[i]->SetNumSegments(m_iShowCount);
			m_pFrtSegments[i]->Initialise(renderer);
		}
	}

	return true;
}

void
ForestScene::Process(float deltaTime, InputSystem& inputSystem)
{
	for (int i = 0; i < m_iShowCount; i++)
	{
		m_pFrtSegments[i]->Process(deltaTime, inputSystem);
	}

	m_pGolem->Process(deltaTime, inputSystem);
}

void
ForestScene::Draw(Renderer& renderer)
{
	for (int i = 0; i < m_iShowCount; i++)
	{
		m_pFrtSegments[i]->Draw(renderer);
	}

	m_pGolem->Draw(renderer);
}

void
ForestScene::SetCharacter(Character& character, Renderer& renderer)
{
	m_pEntCharacter = &character;
	m_pEntCharacter->SetNumSegments(m_iShowCount);
	SetCharPos(m_pEntCharacter->GetPosition());
	SetCharFeetPos(m_pEntCharacter->GetFeetPos());
	SetCharWidth(m_pEntCharacter->GetBodyWidth());

	for (int i = 0; i < m_iShowCount; i++)
	{
		m_pFrtSegments[i]->SetCharacter(character);
		m_pFrtSegments[i]->SetCharPos(*m_vCharPos);
		m_pFrtSegments[i]->SetCharFeetPos(*m_vFeetPos);
		m_pFrtSegments[i]->SetCharWidth(m_iCharWidth);
		
	}

	if (SetEnemies(renderer))
	{
		m_pGolem->SetCharacter(character);
	}
}

void
ForestScene::SetCharPos(Vector2& characterPos)
{
	m_vCharPos = &characterPos;
}

void
ForestScene::SetCharFeetPos(Vector2& feetPos)
{
	m_vFeetPos = &feetPos;
}

void
ForestScene::SetCharWidth(int width)
{
	m_iCharWidth = width;
}

bool
ForestScene::SetEnemies(Renderer& renderer)
{
	m_pGolem = new Golem();

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
