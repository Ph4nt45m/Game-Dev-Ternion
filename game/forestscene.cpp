// COMP710 GP Framework 2022

// This include:
#include "forestscene.h"

// Local includes:
#include "game.h"
#include "renderer.h"
#include "sprite.h"
#include "forest.h"

// Library includes:
#include <iostream>
#include <ctime>

// Static members:
int ForestScene::sm_iSegmentWidth = 0;

ForestScene::ForestScene()
	: m_frtSegments{ 0 }
	, m_iShowCount(0)
{

}

ForestScene::~ForestScene()
{
	for (int i = 0; i < 20; i++)
	{
		delete m_frtSegments[i];
		m_frtSegments[i] = 0;
	}
}

bool
ForestScene::Initialise(Renderer& renderer)
{
	sm_iSegmentWidth = static_cast<int>(390);
	m_iShowCount = 20;
	srand((int)time(0));

	int prevIndex = 0;
	bool closedSegment = false;

	for (int i = 0; i < m_iShowCount; i++)
	{
		if (i == 0 || i == (m_iShowCount - 1))
		{
			m_frtSegments[i] = new Forest();
			m_frtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\boundary.png"));
			m_frtSegments[i]->SetX(((sm_iSegmentWidth * i) + (sm_iSegmentWidth / 2)));
			m_frtSegments[i]->SetIndex(i);
			m_frtSegments[i]->SetAlive(true);
		}
		else if (i > 0 && i <= 3)
		{
			m_frtSegments[i] = new Forest();
			m_frtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\blank.png"));
			m_frtSegments[i]->SetX(((sm_iSegmentWidth * i) + (sm_iSegmentWidth / 2)));
			m_frtSegments[i]->SetIndex(i);
			m_frtSegments[i]->SetAlive(true);
		}
		else
		{
			int index = (rand() % 3);

			if (index == 0)
			{
				m_frtSegments[i] = new Forest();
				m_frtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\blank.png"));
				m_frtSegments[i]->SetX(((sm_iSegmentWidth * i) + (sm_iSegmentWidth / 2)));
				m_frtSegments[i]->SetIndex(i);
				m_frtSegments[i]->SetAlive(true);
				prevIndex = index;
				closedSegment = false;
			}
			else if (index == 1)
			{
				if (prevIndex == 0 || prevIndex == 1 || !closedSegment)
				{
					m_frtSegments[i] = new Forest();
					m_frtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\platform.png"));
					m_frtSegments[i]->SetX(((sm_iSegmentWidth * i) + (sm_iSegmentWidth / 2)));
					m_frtSegments[i]->SetIndex(i);
					m_frtSegments[i]->SetAlive(true);
					prevIndex = index;
					closedSegment = true;
				}
				else
				{
					i--;
				}
			}
			else if (index == 2)
			{
				m_frtSegments[i] = new Forest();
				m_frtSegments[i]->SetSprite(renderer.CreateSprite("..\\Sprites\\floatingplatform.png"));
				m_frtSegments[i]->SetX(((sm_iSegmentWidth * i) + (sm_iSegmentWidth / 2)));
				m_frtSegments[i]->SetIndex(i);
				m_frtSegments[i]->SetAlive(true);
				prevIndex = index;
			}
		}

		if (m_frtSegments[i]->IsAlive())
		{
			m_frtSegments[i]->SetY((renderer.GetHeight() / 2));
			m_frtSegments[i]->SetAngle(180.0f);
			m_frtSegments[i]->Initialise(renderer);
		}
	}

	return true;
}

void
ForestScene::Process(float deltaTime, InputSystem& inputSystem)
{
	for (int i = 0; i < m_iShowCount; i++)
	{
		m_frtSegments[i]->Process(deltaTime);
	}
}

void
ForestScene::Draw(Renderer& renderer)
{
	for (int i = 0; i < m_iShowCount; i++)
	{
		m_frtSegments[i]->Draw(renderer);
	}
}

void
ForestScene::DebugDraw()
{

}
