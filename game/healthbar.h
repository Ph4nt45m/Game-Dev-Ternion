#ifndef HEALTHBAR_H
#define HEALTHBAR_H

// Local includes:

// Library includes:

// Forward Declarations:
class Renderer;
class Sprite;

// class declaration:
class Healthbar
{
	// Member methods:
	public:
		Healthbar(Renderer& renderer);
		~Healthbar();
		void Process(float deltaTime);
		void Draw(Renderer& renderer);
		void Damage(float damage);
		void Heal(float heal);
		void SetMaxHealth(float newMaxHealth);
		void SetCurrentHealth(float newCurrentHealth);
		float GetCurrentHealth() const;
		float GetMaxHealth() const;

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
