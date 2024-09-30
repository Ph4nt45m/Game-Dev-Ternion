// This include:
#include "healthbar.h"

// Local includes:
#include "renderer.h"
#include "logmanager.h"
#include "sprite.h"

// Library includes:
#include <cmath>

Healthbar::Healthbar(Renderer& renderer)
	: m_fCurrentHealth(100), m_fMaxHealth(100)
{
	// Load healthbar sprites
	m_pHealthbarSprite = renderer.CreateSprite("..\\Sprites\\healthbar\\Healthbar.png");
	m_pHealthbarBackgroundSprite = renderer.CreateSprite("..\\Sprites\\healthbar\\HealthbarBackground.png");

	if (!m_pHealthbarSprite || !m_pHealthbarBackgroundSprite)
	{
		LogManager::GetInstance().Log("Healthbar sprite failed to load");
	}

	// Set healthbar position and scale
	// Canvas: 1536 x 846
	int bbWidth = renderer.GetWidth();
	int bbHeight = renderer.GetHeight();

	m_fHealthbarX = 300;
	m_fHealthbarY = bbHeight - 100;
	m_fHealthbarScale = 2.0f;

	m_pHealthbarSprite->SetX(m_fHealthbarX);
	m_pHealthbarSprite->SetY(m_fHealthbarY);
	m_pHealthbarSprite->SetScale(m_fHealthbarScale);
	m_pHealthbarBackgroundSprite->SetX(m_fHealthbarX);
	m_pHealthbarBackgroundSprite->SetY(m_fHealthbarY);
	m_pHealthbarBackgroundSprite->SetScale(m_fHealthbarScale);
}

Healthbar::~Healthbar()
{

}

void Healthbar::Process(float deltaTime)
{
	// Calculate health bar size
	float healthbarMaxWidth = 100.0f * m_fHealthbarScale;
	float healthPercentage = m_fCurrentHealth / m_fMaxHealth;
	float healthbarWidth = round(healthbarMaxWidth * healthPercentage);
	m_pHealthbarSprite->SetWidth(healthbarWidth);

	// Calculate health bar position
	float newHealthbarX = m_fHealthbarX - healthbarMaxWidth + m_pHealthbarSprite->GetWidth() / 2.0f;
	m_pHealthbarSprite->SetX(newHealthbarX);
}

void Healthbar::Draw(Renderer& renderer)
{
	// Draw health bar
	m_pHealthbarBackgroundSprite->Draw(renderer, false, false);
	m_pHealthbarSprite->Draw(renderer, false, false);

}

void Healthbar::Damage(float damage)
{
	m_fCurrentHealth = fmaxf(m_fCurrentHealth - damage, 0.0f);
}

void Healthbar::Heal(float heal)
{
	m_fCurrentHealth = fminf(m_fCurrentHealth + heal, m_fMaxHealth);
}

void Healthbar::SetCurrentHealth(float newCurrentHealth)
{
	m_fCurrentHealth = fminf(newCurrentHealth, m_fMaxHealth);
}

void Healthbar::SetMaxHealth(float newMaxHealth)
{
	m_fMaxHealth = newMaxHealth;
}

float Healthbar::GetCurrentHealth() const
{
	return m_fCurrentHealth;
}

float Healthbar::GetMaxHealth() const
{
	return m_fMaxHealth;
}
