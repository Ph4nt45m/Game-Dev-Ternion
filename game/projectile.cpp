// This include:
#include "projectile.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "inputsystem.h"
#include "../imgui/imgui.h"

// Library includes:
#include <cassert>
#include <cstdio>

Projectile::Projectile()
	: m_fGravity(0.0f)
	, m_fTimeToTarget(0.0f)
	, m_fDX(0.0f)
	, m_fDY(0.0f)
	, m_fGroundY(0.0f)
	, m_fWidth(0.0f)
	, m_fHeight(0.0f)
{

}

Projectile::~Projectile()
{
	delete m_pSprSpriteBody;
	m_pSprSpriteBody = 0;
}

bool
Projectile::Initialise(Renderer& renderer)
{
	/*if (!SetBodySprites(renderer))
	{
		LogManager::GetInstance().Log("Projectile Arrow Sprites failed to initialise!");
		return false;
	}*/

	m_fGravity = 200.0f;
	m_fTimeToTarget = 0.5f;
	/*m_fWidth = (float)m_pSprSpriteBody->GetWidth();
	m_fHeight = (float)m_pSprSpriteBody->GetHeight();*/

	return true;
}

void
Projectile::Process(float deltaTime, InputSystem& inputSystem)
{
	if (m_bAlive)
	{
		m_vPosition.x += m_velocityBody.x * deltaTime;
		m_vPosition.y += m_velocityBody.y * deltaTime;
		m_velocityBody.y += m_fGravity * deltaTime;
	}

	if (m_vPosition.x < 0.0f || m_vPosition.x > sm_fBoundaryWidth || m_vPosition.y >= m_fGroundY)
	{
		m_bAlive = false;
		m_vPosition.x = 0.0f;
		m_vPosition.y = 0.0f;
	}

	m_fAngleOfProjectile = -(float)atan2(m_velocityBody.y, m_velocityBody.x) * 180.0f / PI;

	m_pSprSpriteBody->SetX((int)m_vPosition.x);
	m_pSprSpriteBody->SetY((int)m_vPosition.y);
	m_pSprSpriteBody->SetAngle(m_fAngleOfProjectile);
}

void
Projectile::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, false);
	}
}

bool
Projectile::SetBodySprites(Renderer& renderer)
{
	/*m_pSprSpriteBody = renderer.CreateSprite("..\\Sprites\\characterprojectile\\arrow.png");

	if (!m_pSprSpriteBody)
	{
		return false;
	}*/

	return true;
}

void
Projectile::SetNumSegments(int amount)
{

}

void
Projectile::GetInputs(InputSystem& inputSystem)
{

}

void
Projectile::HandleInput(float deltaTime)
{

}

void
Projectile::SetTerrainMoving(bool moving)
{

}

bool
Projectile::IsTerrainMoving()
{
	return sm_bTerrainMoving;
}

Vector2&
Projectile::GetPosition()
{
	return m_vPosition;
}

bool
Projectile::SetProjectileSprite(Renderer& renderer, const char* filePath)
{
	m_pSprSpriteBody = renderer.CreateSprite(filePath);

	if (!m_pSprSpriteBody)
	{
		return false;
	}
	else
	{
		m_fWidth = (float)m_pSprSpriteBody->GetWidth();
		m_fHeight = (float)m_pSprSpriteBody->GetHeight();
	}

	return true;
}

float
Projectile::GetWidth()
{
	return (float)m_pSprSpriteBody->GetWidth();
}

float
Projectile::GetHeight()
{
	return (float)m_pSprSpriteBody->GetHeight();
}

void
Projectile::SetStartPos(float st_x, float st_y)
{
	m_vPosition.x = st_x;
	m_vPosition.y = st_y;
	m_vStandingPos.x = st_x;
	m_vStandingPos.y = st_y;
}

void
Projectile::SetTargetPos(float t_x, float t_y)
{
	m_vTarget.x = t_x;
	m_vTarget.y = t_y;
}

void
Projectile::SetGroundY(float g_y)
{
	m_fGroundY = g_y;
}

void
Projectile::SetTimeToTarget(float time)
{
	m_fTimeToTarget = time;
}

void
Projectile::Shoot()
{
	m_fDX = m_vTarget.x - m_vStandingPos.x;
	m_fDY = m_vTarget.y - m_vStandingPos.y;

	m_velocityBody.x = m_fDX / m_fTimeToTarget;
	m_velocityBody.y = (m_fDY - 0.5f * m_fGravity * m_fTimeToTarget * m_fTimeToTarget) / m_fTimeToTarget;
	m_bAlive = true;
}

bool
Projectile::IsAlive()
{
	return m_bAlive;
}

void
Projectile::SetAlive(bool alive)
{
	m_bAlive = alive;
}
