// This include:
#include "forest.h"

// Local includes:
#include "renderer.h"
#include "inputsystem.h"
#include "character.h"
#include "golem.h"
#include "vector2.h"
#include "sprite.h"
#include "../imgui/imgui.h"

// Static members:
int Forest::sm_iSegmentWidth = 0;
int Forest::sm_iSegmentHeight = 0;
int Forest::sm_iScreenWidth = 0;
int Forest::sm_iScreenHeight = 0;
float Forest::sm_fVelocity = 0.0f;
bool Forest::sm_bLeftCollision = false;
bool Forest::sm_bRightCollision = false;
int Forest::sm_iTerrainLevel = 0;

#include <cstdio> // Temp for printing

Forest::Forest()
	: m_sTerrainMotions{ BS_NEUTRAL, BS_NEUTRAL }
	, m_iSurge(0)
	, m_pSprSprite(0)
	, m_iNumSegments(0)
	, m_iSegmentIndex(0)
	, m_iSegmentType(0)
	, m_fPositionX(0.0f)
	, m_iLowerBound(0)
	, m_iUpperBound(0)
	, m_fFloorLowerBound(0.0f)
	, m_fFloorUpperBound(0.0f)
	, m_fPlatformHeight(0.0f)
	, m_bLeftCollision(false)
	, m_bRightCollision(false)
	, m_bHorizontalFlip(false)
	, m_bAlive(false)
	, m_pEntCharacter(0)
	, m_vCharPos(0)
	, m_vFeetPos(0)
	, m_iCharWidth(0)
{

}

Forest::~Forest()
{
	delete m_pSprSprite;
	m_pSprSprite = 0;
}

bool
Forest::Initialise(Renderer& renderer)
{
	const int SEGMENT_WIDTH = m_pSprSprite->GetWidth();
	sm_iSegmentWidth = static_cast<int>(SEGMENT_WIDTH);
	const int SEGMENT_HEIGHT = m_pSprSprite->GetHeight();
	sm_iSegmentHeight = static_cast<int>(SEGMENT_HEIGHT);
	const int SCREEN_WIDTH = renderer.GetWidth();
	sm_iScreenWidth = static_cast<int>(SCREEN_WIDTH);
	const int SCREEN_HEIGHT = renderer.GetHeight();
	sm_iScreenHeight = static_cast<int>(SCREEN_HEIGHT);
	m_fPositionX = (float)m_pSprSprite->GetX();
	m_fFloorLowerBound = ((sm_iSegmentHeight / 7.0f) * 5.0f);
	m_fFloorUpperBound = (float)sm_iScreenHeight;
	m_fPlatformHeight = 89.0f; //  Switch to 90.0f when 3D movement implemented for offset

	SetBounds();

	return true;
}

void
Forest::Process(float deltaTime, InputSystem& inputSystem)
{
	GetInputs(inputSystem);
	HandleInputs();
	HandlePlayerCollisions();
	//HandlePlayerCollisions();

	m_fPositionX += Forest::sm_fVelocity * deltaTime;
	m_pSprSprite->SetX((int)m_fPositionX);
}

void
Forest::GetInputs(InputSystem& inputSystem)
{
	m_sTerrainMotions.MoveForward = inputSystem.GetKeyState(SDL_SCANCODE_D);
	m_sTerrainMotions.MoveBackward = inputSystem.GetKeyState(SDL_SCANCODE_A);

	if (m_sTerrainMotions.MoveForward == BS_PRESSED || m_sTerrainMotions.MoveForward == BS_HELD)
	{
		m_iSurge = MOTION_FORWARD;
	}
	else if (m_sTerrainMotions.MoveBackward == BS_PRESSED || m_sTerrainMotions.MoveBackward == BS_HELD)
	{
		m_iSurge = MOTION_BACKWARD;
	}
	else
	{
		m_iSurge = MOTION_NONE;
	}
}

void
Forest::HandleInputs()
{
	if (m_iSurge > 0)
	{
		if (m_pSprSprite->GetX() > m_iLowerBound && m_vCharPos->x >= (float)(sm_iScreenWidth / 2))
		{
			m_pEntCharacter->SetCentered(true);

			if (!Forest::sm_bLeftCollision)
			{
				Forest::sm_fVelocity = -200.0f;
				Forest::sm_bRightCollision = false;
				m_pEntCharacter->SetTerrainMoving(true);
			}
			else
			{
				Forest::sm_fVelocity = 0.0f;
				m_pEntCharacter->SetTerrainMoving(false);
			}
		}
		else
		{
			m_pEntCharacter->SetCentered(false);
			m_pSprSprite->SetX(m_iLowerBound);
			Forest::sm_fVelocity = 0.0f;
		}
	}
	else if (m_iSurge < 0)
	{
		if (m_pSprSprite->GetX() < m_iUpperBound && m_vCharPos->x <= (float)(sm_iScreenWidth / 2))
		{
			m_pEntCharacter->SetCentered(true);

			if (!Forest::sm_bRightCollision)
			{
				Forest::sm_fVelocity = 200.0f;
				Forest::sm_bLeftCollision = false;
				m_pEntCharacter->SetTerrainMoving(true);
			}
			else
			{
				Forest::sm_fVelocity = 0.0f;
				m_pEntCharacter->SetTerrainMoving(false);
			}
		}
		else
		{
			m_pEntCharacter->SetCentered(false);
			m_pSprSprite->SetX(m_iUpperBound);
			Forest::sm_fVelocity = 0.0f;
		}
	}
	else
	{
		m_pEntCharacter->SetCentered(false);
		Forest::sm_fVelocity = 0.0f;
		m_pEntCharacter->SetTerrainMoving(false);
	}
}

void
Forest::HandlePlayerCollisions()
{
	// Collision vectors:
	// Left wall & leftmost vectors for other segments
	Vector2 frontPointL;
	Vector2 backPointL;

	// Right wall && rightmost vectors for other segments
	Vector2 frontPointR;
	Vector2 backPointR;

	// Middle vector for step platform segment - For future updates
	Vector2 middlePoint;

	// For future updates
	float tX;
	float yAtTX;

	// Check if character is within the segment
	if ((int)m_vCharPos->x >= (m_fPositionX - ((sm_iSegmentWidth - 6) / static_cast<float>(2)) - ((float)m_iCharWidth * 1.5f)) &&
		(int)m_vCharPos->x <= (m_fPositionX + ((sm_iSegmentWidth - 6) / static_cast<float>(2)) + ((float)m_iCharWidth * 1.5f)))
	{
		switch (m_iSegmentType)
		{
		case ST_BOUNDARY:
			// No collision
			break;
		case ST_LEFTWALL:
			// Initialise points that make up line of right wall collision
			frontPointL.x = 0.0f;
			frontPointL.y = m_fFloorLowerBound;

			// Initialise points that make up line of left wall collision - For future updates
			frontPointL.x = 6.0f;
			frontPointL.y = m_fFloorUpperBound;

			backPointL.x = 38.0f;
			backPointL.y = m_fFloorLowerBound;
			
			tX = ((m_vCharPos->x - ((float)m_iCharWidth / 2.0f)) - frontPointL.x) / (backPointL.x - frontPointL.x);
			yAtTX = frontPointL.y + tX * (backPointL.y - frontPointL.y);

			if (tX >= 0.0f && tX <= 1.0f && m_vCharPos->y <= yAtTX)
			{
				// Collision has occured, now handle
				m_pEntCharacter->ShiftX(1.0f);
			}

			break;
		case ST_RIGHTWALL:
			// Initialise points that make up line of right wall collision
			frontPointR.x = (float)sm_iScreenWidth;
			frontPointR.y = m_fFloorLowerBound;

			// Initialise points that make up line of right wall collision - For future updates
			frontPointR.x = sm_iScreenWidth - 6.0f;
			frontPointR.y = m_fFloorUpperBound;

			backPointR.x = sm_iScreenWidth - 38.0f;
			backPointR.y = m_fFloorLowerBound;

			tX = ((m_vCharPos->x + ((float)m_iCharWidth / 2.0f)) - frontPointR.x) / (backPointR.x - frontPointR.x);
			yAtTX = frontPointR.y + tX * (backPointR.y - frontPointR.y);

			if (tX >= 0.0f && tX <= 1.0f && m_vCharPos->y <= yAtTX)
			{
				// Collision has occured, now handle
				m_pEntCharacter->ShiftX(-1.0f);
			}

			break;
		case ST_BLANK:
			// Initialise points that represent segment range for detection
			frontPointL.x = m_fPositionX - ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointL.y = m_fFloorLowerBound;

			frontPointR.x = m_fPositionX + ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointR.y = m_fFloorLowerBound;

			// Check if player is within the terrain segment
			if ((m_vFeetPos->x >= frontPointL.x && m_vFeetPos->x <= frontPointR.x))
			{
				// Check if player is above ground level
				if (Forest::sm_iTerrainLevel > 0)
				{
					// Handle decent of player from higher surface
					Forest::sm_bLeftCollision = false;
					Forest::sm_bRightCollision = false;

					if (m_vFeetPos->x < m_fPositionX)
					{
						m_pEntCharacter->ShiftX(2.0f);
					}
					else if (m_vFeetPos->x > m_fPositionX)
					{
						m_pEntCharacter->ShiftX(-2.0f);
					}

					m_pEntCharacter->ShiftY((m_fPlatformHeight * Forest::sm_iTerrainLevel));
					Forest::sm_iTerrainLevel = 0;
				}
			}

			break;
		case ST_PLATFORM:
			// Initialise points for collision on sides of platform segment
			frontPointL.x = m_fPositionX - ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointL.y = m_fFloorLowerBound - m_fPlatformHeight;

			frontPointR.x = m_fPositionX + ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointR.y = m_fFloorLowerBound - m_fPlatformHeight;

			// Check if player has reach minimum height of platform
			if (m_vFeetPos->y < (m_vCharPos->y - m_fPlatformHeight))
			{
				// Remove collision to allow player to jump onto platform
				Forest::sm_bLeftCollision = false;
				Forest::sm_bRightCollision = false;

				// Check if player is above platform during jump
				if (m_vFeetPos->x >= frontPointL.x && m_vFeetPos->x <= frontPointR.x)
				{
					// Check if player is jumping onto platform, not decending onto
					if (Forest::sm_iTerrainLevel < 1)
					{
						// Adjust terrain level and character position values
						m_pEntCharacter->ShiftY(-(m_fPlatformHeight));
						Forest::sm_iTerrainLevel++;
					}
				}
			}
			// Check if player is colliding with left platform wall
			else if ((int)(m_vCharPos->x + ((float)m_iCharWidth / 2.0f)) >= (int)frontPointL.x &&
				!((int)m_vCharPos->x > (int)frontPointR.x) && Forest::sm_iTerrainLevel == 0)
			{
				Forest::sm_bLeftCollision = true;
			}
			// Check if player is colliding with right platform wall
			else if ((m_vCharPos->x - ((float)m_iCharWidth / 2.0f)) <= frontPointR.x &&
				!((int)m_vCharPos->x < (int)frontPointL.x) && Forest::sm_iTerrainLevel == 0)
			{
				Forest::sm_bRightCollision = true;
			}
			// Check if player is within platform range
			else if ((m_vFeetPos->x >= frontPointL.x && m_vFeetPos->x <= frontPointR.x))
			{
				// Check if player is decending from a higher surface
				if (Forest::sm_iTerrainLevel > 1)
				{
					Forest::sm_bLeftCollision = false;
					Forest::sm_bRightCollision = false;

					// Adjust character position values based on terrain level and decrease terrain level to 1 after
					if (m_vFeetPos->x < m_fPositionX)
					{
						m_pEntCharacter->ShiftX(2.0f);
					}
					else if (m_vFeetPos->x > m_fPositionX)
					{
						m_pEntCharacter->ShiftX(-2.0f);
					}

					m_pEntCharacter->ShiftY((m_fPlatformHeight * (Forest::sm_iTerrainLevel - 1)));
					Forest::sm_iTerrainLevel = 1;
				}
			}

			break;
		case ST_FLOATINGPLATFORM:
			// To be implemented in future update
			break;
		case ST_TALLPLATFORM:
			// Initialise points for collision on sides of tall platform segment
			frontPointL.x = m_fPositionX - ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointL.y = m_fFloorLowerBound - (m_fPlatformHeight * 2.0f);

			frontPointR.x = m_fPositionX + ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointR.y = m_fFloorLowerBound - (m_fPlatformHeight * 2.0f);

			// Check if player has reached minimum height of tall platform
			if (m_vFeetPos->y < (m_vCharPos->y - (m_fPlatformHeight * 2.0f)))
			{
				// Remove collision to allow player to jump onto platform
				Forest::sm_bLeftCollision = false;
				Forest::sm_bRightCollision = false;

				if (m_vFeetPos->x >= frontPointL.x && m_vFeetPos->x <= frontPointR.x)
				{
					// Check if player is from ground level
					if (Forest::sm_iTerrainLevel < 1)
					{
						// Adjust terrain level and character position values
						Forest::sm_iTerrainLevel += 2;
						m_pEntCharacter->ShiftY(-(m_fPlatformHeight * 2.0f));
					}
				}
			}
			// Check if player has reached minimum height from terrain level 1 to tall platform height
			else if(m_vFeetPos->y < (m_vCharPos->y - m_fPlatformHeight) && Forest::sm_iTerrainLevel == 1)
			{
				// Remove collision to allow player to jump onto platform
				Forest::sm_bLeftCollision = false;
				Forest::sm_bRightCollision = false;

				// Check if player is above tall platform
				if (m_vFeetPos->x >= frontPointL.x && m_vFeetPos->x <= frontPointR.x)
				{
					// Adjust terrain level and character position values
					Forest::sm_iTerrainLevel++;
					m_pEntCharacter->ShiftY(-m_fPlatformHeight);
				}
			}
			// Check if player is colliding with left platform wall
			else if ((int)(m_vCharPos->x + ((float)m_iCharWidth / 2.0f)) >= (int)frontPointL.x &&
				!((int)m_vCharPos->x > (int)frontPointR.x) && Forest::sm_iTerrainLevel < 2)
			{
				Forest::sm_bLeftCollision = true;
			}
			// Check if player is colliding with right platform wall
			else if ((m_vCharPos->x - ((float)m_iCharWidth / 2.0f)) <= frontPointR.x &&
				!((int)m_vCharPos->x < (int)frontPointL.x) && Forest::sm_iTerrainLevel < 2)
			{
				Forest::sm_bRightCollision = true;
			}

			break;
		}
	}
}

void
Forest::HandleProjCollisions()
{
	// Collision vectors:
	// Left wall & leftmost vectors for other segments
	Vector2 frontPointL;
	Vector2 backPointL;

	// Right wall && rightmost vectors for other segments
	Vector2 frontPointR;
	Vector2 backPointR;

	// Middle vector for step platform segment - For future updates
	//Vector2 middlePoint;

	// For future updates
	/*float tX;
	float yAtTX;*/

	// Check if projectile is within the segment
	if (m_pEntCharacter->GetProjectilePos().x >= (m_fPositionX - ((sm_iSegmentWidth - 6) / static_cast<float>(2)) - (m_pEntCharacter->GetProjWidth() * 1.5f)) &&
		m_pEntCharacter->GetProjectilePos().x <= (m_fPositionX + ((sm_iSegmentWidth - 6) / static_cast<float>(2)) + (m_pEntCharacter->GetProjWidth() * 1.5f)))
	{
		switch (m_iSegmentType)
		{
		case ST_BOUNDARY:
			// No collision
			break;
		case ST_LEFTWALL:
			// Initialise points that make up line of right wall collision
			frontPointL.x = 0.0f;
			frontPointL.y = m_fFloorLowerBound;

			// Check if player arrow has collided with left wall or hit the ground
			if (m_pEntCharacter->IsProjAlive())
			{
				if (((m_pEntCharacter->GetProjectilePos().x - (m_pEntCharacter->GetProjWidth() / 2.0f)) <= frontPointL.x) ||
					(m_pEntCharacter->GetProjectilePos().y >= frontPointL.y))
				{
					// Remove player arrow
					m_pEntCharacter->SetProjAlive(false);
				}
			}

			break;
		case ST_RIGHTWALL:
			// Initialise points that make up line of right wall collision
			frontPointR.x = (float)sm_iScreenWidth;
			frontPointR.y = m_fFloorLowerBound;

			// Check if player arrow has collided with right wall or hit the ground
			if (((m_pEntCharacter->GetProjectilePos().x + (m_pEntCharacter->GetProjWidth() / 2.0f)) >= frontPointL.x) ||
				(m_pEntCharacter->GetProjectilePos().y >= frontPointR.y))
			{
				if (m_pEntCharacter->IsProjAlive())
				{
					// Remove player arrow
					m_pEntCharacter->SetProjAlive(false);
				}
			}

			break;
		case ST_BLANK:
			// Initialise points that represent segment range for detection
			frontPointL.x = m_fPositionX - ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointL.y = m_fFloorLowerBound;

			frontPointR.x = m_fPositionX + ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointR.y = m_fFloorLowerBound;

			// Check if player arrow is in segment
			if (m_pEntCharacter->GetProjectilePos().x >= frontPointL.x &&
				m_pEntCharacter->GetProjectilePos().x <= frontPointR.x)
			{
				// Check if player arrow has hit the ground
				if (m_pEntCharacter->GetProjectilePos().y >= frontPointL.y && m_pEntCharacter->IsProjAlive())
				{
					// Remove player arrow
					m_pEntCharacter->SetProjAlive(false);
				}
			}

			break;
		case ST_PLATFORM:
			// Initialise points for collision on sides of platform segment
			frontPointL.x = m_fPositionX - ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointL.y = m_fFloorLowerBound - m_fPlatformHeight;

			frontPointR.x = m_fPositionX + ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointR.y = m_fFloorLowerBound - m_fPlatformHeight;

			// Check if player arrow is colliding with left platform wall
			if (m_pEntCharacter->GetProjectilePos().x + (m_pEntCharacter->GetProjWidth() / 2.0f) >= frontPointL.x &&
				!(m_pEntCharacter->GetProjectilePos().x > frontPointR.x) &&
				m_pEntCharacter->GetProjectilePos().y >= frontPointL.y && m_pEntCharacter->GetProjectilePos().y <= m_fFloorLowerBound)
			{
				// Remove player arrow
				m_pEntCharacter->SetProjAlive(false);
			}
			// Check if player arrow is colliding with right platform wall
			else if (m_pEntCharacter->GetProjectilePos().x - (m_pEntCharacter->GetProjWidth() / 2.0f) <= frontPointR.x &&
				!(m_pEntCharacter->GetProjectilePos().x < frontPointL.x) &&
				m_pEntCharacter->GetProjectilePos().y >= frontPointR.y && m_pEntCharacter->GetProjectilePos().y <= m_fFloorLowerBound)
			{
				// Remove player arrow
				m_pEntCharacter->SetProjAlive(false);
			}
			// Check if player arrow is in range of top of platform
			else if (m_pEntCharacter->GetProjectilePos().x >= frontPointL.x && m_pEntCharacter->GetProjectilePos().x <= frontPointR.x)
			{
				// Check if player arrow is colliding with top of platform
				if (m_pEntCharacter->GetProjectilePos().y >= frontPointL.y)
				{
					// Remove player arrow
					m_pEntCharacter->SetProjAlive(false);
				}
			}

			break;
		case ST_FLOATINGPLATFORM:
			// To be implemented in future update
			break;
		case ST_TALLPLATFORM:
			// Initialise points for collision on sides of tall platform segment
			frontPointL.x = m_fPositionX - ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointL.y = m_fFloorLowerBound - (m_fPlatformHeight * 2.0f);

			frontPointR.x = m_fPositionX + ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointR.y = m_fFloorLowerBound - (m_fPlatformHeight * 2.0f);

			// Check if player arrow is colliding with left platform wall
			if (m_pEntCharacter->GetProjectilePos().x + (m_pEntCharacter->GetProjWidth() / 2.0f) >= frontPointL.x &&
				!(m_pEntCharacter->GetProjectilePos().x > frontPointR.x) &&
				m_pEntCharacter->GetProjectilePos().y >= frontPointL.y && m_pEntCharacter->GetProjectilePos().y <= m_fFloorLowerBound)
			{
				// Remove player arrow
				m_pEntCharacter->SetProjAlive(false);
			}
			// Check if player arrow is colliding with right platform wall
			else if (m_pEntCharacter->GetProjectilePos().x - (m_pEntCharacter->GetProjWidth() / 2.0f) <= frontPointR.x &&
				!(m_pEntCharacter->GetProjectilePos().x < frontPointL.x) &&
				m_pEntCharacter->GetProjectilePos().y >= frontPointR.y && m_pEntCharacter->GetProjectilePos().y <= m_fFloorLowerBound)
			{
				// Remove player arrow
				m_pEntCharacter->SetProjAlive(false);
			}
			// Check if player arrow is in range of top of platform
			else if (m_pEntCharacter->GetProjectilePos().x >= frontPointL.x && m_pEntCharacter->GetProjectilePos().x <= frontPointR.x)
			{
				// Check if player arrow is colliding with top of platform
				if (m_pEntCharacter->GetProjectilePos().y >= frontPointL.y)
				{
					// Remove player arrow
					m_pEntCharacter->SetProjAlive(false);
				}
			}

			break;
		}
	}
}

void
Forest::HandleGolemCollisions()
{
	// Collision vectors:
	// Left wall & leftmost vectors for other segments
	Vector2 frontPointL;
	Vector2 backPointL;

	// Right wall && rightmost vectors for other segments
	Vector2 frontPointR;
	Vector2 backPointR;

	// Middle vector for step platform segment - For future updates
	//Vector2 middlePoint;

	// For future updates
	/*float tX;
	float yAtTX;*/

	// Check if golem is within the segment
	if ((int)m_vGolemPos->x >= (m_fPositionX - ((sm_iSegmentWidth - 6) / static_cast<float>(2)) - (m_fGolemWidth / 2.0f)) &&
		(int)m_vGolemPos->x <= (m_fPositionX + ((sm_iSegmentWidth - 6) / static_cast<float>(2)) + (m_fGolemWidth / 2.0f)))
	{
		switch (m_iSegmentType)
		{
		case ST_BOUNDARY:
			// No collision
			break;
		case ST_RIGHTWALL:
			// Initialise points that make up line of right wall collision
			frontPointR.x = (float)sm_iScreenWidth;
			frontPointR.y = m_fFloorLowerBound;

			if ((m_vGolemPos->x + (m_fGolemWidth / 2.0f)) >= frontPointR.x)
			{
				// Collision has occured, now handle
				m_pGolem->ShiftX(-1.0f);
			}

			break;
		case ST_PLATFORM:
			// Initialise points for collision on sides of platform segment
			frontPointL.x = m_fPositionX - ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointL.y = m_fFloorLowerBound - m_fPlatformHeight;

			frontPointR.x = m_fPositionX + ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointR.y = m_fFloorLowerBound - m_fPlatformHeight;

			// Check if golem is colliding with left platform wall
			if ((int)(m_vGolemPos->x + (m_fGolemWidth / 2.0f)) >= (int)frontPointL.x &&
				!((int)m_vGolemPos->x > (int)frontPointR.x) && Forest::sm_iTerrainLevel == 0)
			{
				m_pGolem->ShiftX(-1.0f);
			}
			// Check if golem is colliding with right platform wall
			else if (((int)m_vGolemPos->x - (m_fGolemWidth / 2.0f)) <= frontPointR.x &&
				!((int)m_vGolemPos->x < (int)frontPointL.x) && Forest::sm_iTerrainLevel == 0)
			{
				m_pGolem->ShiftX(1.0f);
			}

			break;
		case ST_FLOATINGPLATFORM:
			// To be implemented in future update
			break;
		case ST_TALLPLATFORM:
			// Initialise points for collision on sides of tall platform segment
			frontPointL.x = m_fPositionX - ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointL.y = m_fFloorLowerBound - (m_fPlatformHeight * 2.0f);

			frontPointR.x = m_fPositionX + ((float)(sm_iSegmentWidth - 6) / 2.0f);
			frontPointR.y = m_fFloorLowerBound - (m_fPlatformHeight * 2.0f);

			// Check if golem is colliding with left platform wall
			if ((int)(m_vGolemPos->x + (m_fGolemWidth / 2.0f)) >= (int)frontPointL.x &&
				!((int)m_vGolemPos->x > (int)frontPointR.x) && Forest::sm_iTerrainLevel < 2)
			{
				m_pGolem->ShiftX(-1.0f);
			}
			// Check if golem is colliding with right platform wall
			else if ((m_vGolemPos->x - (m_fGolemWidth / 2.0f)) <= frontPointR.x &&
				!((int)m_vGolemPos->x < (int)frontPointL.x) && Forest::sm_iTerrainLevel < 2)
			{
				m_pGolem->ShiftX(1.0f);
			}

			break;
		}
	}
}

void
Forest::SetBounds()
{
	m_iUpperBound = ((m_iSegmentIndex * (sm_iSegmentWidth - 6)) - ((sm_iSegmentWidth - 6) / 2));
	m_iLowerBound = (((m_iSegmentIndex * (sm_iSegmentWidth - 6)) - ((sm_iSegmentWidth - 6) / 2)) - ((m_iNumSegments - 6) * (sm_iSegmentWidth - 6))) - 25;
}

void
Forest::SetCharacter(Character& character)
{
	m_pEntCharacter = &character;
}

void
Forest::SetCharPos(Vector2& position)
{
	m_vCharPos = &position;
}

void
Forest::SetCharFeetPos(Vector2& feetPos)
{
	m_vFeetPos = &feetPos;
}

void
Forest::SetCharWidth(int width)
{
	m_iCharWidth = width;
}

void
Forest::SetGolem(Golem& golem)
{
	m_pGolem = &golem;
}

void
Forest::SetGolemPos(Vector2& position)
{
	m_vGolemPos = &position;
}

void
Forest::SetGolemWidth(float width)
{
	m_fGolemWidth = width;
}

int
Forest::GetLBX()
{
	return m_iLowerBound;
}

int
Forest::GetUBX()
{
	return m_iUpperBound;
}

float
Forest::GetPos()
{
	return m_fPositionX;
}

float
Forest::GetV()
{
	return sm_fVelocity;
}

void
Forest::Draw(Renderer& renderer)
{
	m_pSprSprite->Draw(renderer, m_bHorizontalFlip, true);
}

Sprite*
Forest::GetSprite()
{
	return m_pSprSprite;
}

void
Forest::SetSprite(Sprite* sprite)
{
	m_pSprSprite = sprite;
}

void
Forest::SetNumSegments(int amount)
{
	m_iNumSegments = amount;
}

int
Forest::GetX()
{
	return m_pSprSprite->GetX();
}

void
Forest::SetX(int x)
{
	m_pSprSprite->SetX(x);
}

int
Forest::GetY()
{
	return m_pSprSprite->GetY();
}

void
Forest::SetY(int y)
{
	m_pSprSprite->SetY(y);
}

int
Forest::GetHeight()
{
	return m_pSprSprite->GetHeight();
}

void
Forest::SetAngle(float angle)
{
	m_pSprSprite->SetAngle(angle);
}

void
Forest::SetScale(float scale)
{
	m_pSprSprite->SetScale(scale);
}

void
Forest::SetIndex(int index)
{
	m_iSegmentIndex = index;
}

void
Forest::SetType(int type)
{
	m_iSegmentType = type;
}

void
Forest::SetHorizontalFlip(bool flip)
{
	m_bHorizontalFlip = flip;
}

bool
Forest::IsAlive()
{
	return m_bAlive;
}

void
Forest::SetAlive(bool alive)
{
	m_bAlive = alive;
}

void
Forest::DebugDraw()
{

}
