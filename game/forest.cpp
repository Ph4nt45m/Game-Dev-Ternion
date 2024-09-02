// This include:
#include "forest.h"

// Local includes:
#include "renderer.h"
#include "sprite.h"
#include "../imgui/imgui.h"

#include <cstdio>

Forest::Forest()
	: m_pSprSprite(0)
	, m_iSegmentIndex(0)
	, m_bAlive(false)
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
	return true;
}

void
Forest::Process(float deltaTime)
{
	
}

void
Forest::Draw(Renderer& renderer)
{
	m_pSprSprite->Draw(renderer, false, false);
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
Forest::SetX(int x)
{
	m_pSprSprite->SetX(x);
}

void
Forest::SetY(int y)
{
	m_pSprSprite->SetY(y);
}

void
Forest::SetAngle(float angle)
{
	m_pSprSprite->SetAngle(angle);
}

void
Forest::SetIndex(int index)
{
	m_iSegmentIndex = index;
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
