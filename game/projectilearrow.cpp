// This include:
#include "projectilearrow.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"
#include "inputsystem.h"
#include "../imgui/imgui.h"

// Library includes:
#include <cassert>
#include <cstdio>

ProjectileArrow::ProjectileArrow()
	: m_fGravity(0.0f)
	, m_fTimeToTarget(0.0f)
	, m_fDX(0.0f)
	, m_fDY(0.0f)
	, m_fGroundY(0.0f)
	, m_fWidth(0.0f)
	, m_fHeight(0.0f)
{

}

ProjectileArrow::~ProjectileArrow()
{
	delete m_pSprSpriteBody;
	m_pSprSpriteBody = 0;
}

bool
ProjectileArrow::Initialise(Renderer& renderer)
{
	if (!SetBodySprites(renderer))
	{
		LogManager::GetInstance().Log("Projectile Arrow Sprites failed to initialise!");
		return false;
	}

	m_fGravity = 200.0f;
	m_fTimeToTarget = 0.5f;
	m_fWidth = (float)m_pSprSpriteBody->GetWidth();
	m_fHeight = (float)m_pSprSpriteBody->GetHeight();

	return true;
}

void
ProjectileArrow::Process(float deltaTime, InputSystem& inputSystem)
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
ProjectileArrow::Draw(Renderer& renderer)
{
	if (m_bAlive)
	{
		m_pSprSpriteBody->Draw(renderer, m_bFlipHorizontally, false);
	}
}

bool
ProjectileArrow::SetBodySprites(Renderer& renderer)
{
	m_pSprSpriteBody = renderer.CreateSprite("..\\Sprites\\characterprojectile\\arrow.png");

	if (!m_pSprSpriteBody)
	{
		return false;
	}

	return true;
}

void
ProjectileArrow::SetNumSegments(int amount)
{

}

void
ProjectileArrow::GetInputs(InputSystem& inputSystem)
{

}

void
ProjectileArrow::HandleInput(float deltaTime)
{

}

void
ProjectileArrow::SetTerrainMoving(bool moving)
{

}

bool
ProjectileArrow::IsTerrainMoving()
{
	return sm_bTerrainMoving;
}

Vector2&
ProjectileArrow::GetPosition()
{
	return m_vPosition;
}

float
ProjectileArrow::GetWidth()
{
	return (float)m_pSprSpriteBody->GetWidth();
}

float
ProjectileArrow::GetHeight()
{
	return (float)m_pSprSpriteBody->GetHeight();
}

void
ProjectileArrow::SetStartPos(float st_x, float st_y)
{
	m_vPosition.x = st_x;
	m_vPosition.y = st_y;
	m_vStandingPos.x = st_x;
	m_vStandingPos.y = st_y;
}

void
ProjectileArrow::SetTargetPos(float t_x, float t_y)
{
	m_vTarget.x = t_x;
	m_vTarget.y = t_y;
}

void
ProjectileArrow::SetGroundY(float g_y)
{
	m_fGroundY = g_y;
}

void
ProjectileArrow::Shoot()
{
	m_fDX = m_vTarget.x - m_vStandingPos.x;
	m_fDY = m_vTarget.y - m_vStandingPos.y;

	if (m_fDX < 0)
	{
		m_bFlipHorizontally = true;
	}
	else if (m_fDX > 0)
	{
		m_bFlipHorizontally = false;
	}

	m_velocityBody.x = m_fDX / m_fTimeToTarget;
	m_velocityBody.y = (m_fDY - 0.5f * m_fGravity * m_fTimeToTarget * m_fTimeToTarget) / m_fTimeToTarget;
	m_bAlive = true;
}

bool
ProjectileArrow::IsAlive()
{
	return m_bAlive;
}

void
ProjectileArrow::SetAlive(bool alive)
{
	m_bAlive = alive;
}
