#ifndef ARMOUR_H
#define ARMOUR_H

// Local includes:

// Library includes:

// Forward Declarations:
class Renderer;
class Sprite;

// class declaration:
class Armour
{
	// Member methods:
public:
	Armour();
	~Armour();
	void Process(float deltaTime);
	void Draw(Renderer& renderer);
	float GetHealthModifier() const;
	float GetSpeedModifier() const;
protected:

private:

	// Member data:
public:

protected:

private:
	Sprite* m_pArmourSprite;
	float m_fHealthModifier;
	float m_fSpeedModifier;
};

#endif // !ARMOUR_H
