#ifndef HEALTHBAR_H
#define HEALTHBAR_H

// Local includes:
#include "inputsystem.h"

// Library includes:

// Forward Declarations:
class Renderer;
class Sprite;
//class inputSystem;
// class declaration:
class Healthbar
{
	// Member methods:
	public:
		Healthbar(Renderer& renderer, float maxHealthbar);
		~Healthbar();
		void Process(float deltaTime, InputSystem& inputSystem);
		void Draw(Renderer& renderer);
		void Damage(float damage);
		void Heal(float heal);
		void SetMaxHealth(float newMaxHealth);
		void SetCurrentHealth(float newCurrentHealth);
		float GetCurrentHealth() const;
		float GetMaxHealth() const;
		bool Living();
	protected:

	private:

	// Member data:
	public:

	protected:

	private:
		float m_fCurrentHealth;
		float m_fMaxHealth;
		float m_fHealthbarX;
		float m_fHealthbarY;
		float m_fHealthbarScale;

		Sprite* m_pHealthbarSprite;
		Sprite* m_pHealthbarBackgroundSprite;
};

#endif // !HEALTHBAR_H
