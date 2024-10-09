// This include:
#include "armour.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"

// Library includes:

Armour::Armour()
	: m_fHealthModifier(0), m_fSpeedModifier(0)
{

}

Armour::~Armour()
{

}

void Armour::Process(float deltaTime)
{

}

void Armour::Draw(Renderer& renderer)
{

}

float Armour::GetHealthModifier() const
{
	return m_fHealthModifier;
}

float Armour::GetSpeedModifier() const
{
	return m_fSpeedModifier;
}
