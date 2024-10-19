#ifndef PLAYER_H
#define PLAYER_H

// Local includes:
#include "renderer.h"
#include "vector2.h"
#include "healthbar.h"
#include "inputsystem.h"
#include "entity.h"

// Forward declarations:
class Renderer;
class Vector2;
class Sprite;
class InputSystem;

// Types of player animations
typedef struct
{
	AnimatedSprite* m_pASpriteIdle;
	AnimatedSprite* m_pASpriteRun;
	AnimatedSprite* m_pASpriteJump;
	AnimatedSprite* m_pASpriteAttack;
	AnimatedSprite* m_pASpriteDeath;
} Actions;

// Class declaration: 
class Player : public Entity
{
	// Member methods: 
public:
	Player();
	virtual ~Player();

	virtual b2Vec2 GetPosition() = 0; // Changes made by Karl
	virtual void DrawWithCam(Renderer& renderer, Camera& camera) = 0;
	virtual int GetCharactertype() = 0;
	virtual void SetCharacterType(Renderer& renderer, int type) = 0;

	//Kyle
	virtual Healthbar* getPlayerHealthbar() = 0;
	// Changes made by Karl
	virtual void SetAlive(bool alive) = 0;
	virtual bool IsGodmode() = 0;
	//virtual void DebugDraw() = 0;

protected:

private:
	Player(const Player& player);
	Player& operator=(const Player& player);

	// Member data: 
public:


protected:
	Sprite* m_pStaticDeath;
	bool m_bDoubleJump;
	bool m_bAnimateDeath;
	bool m_bGodmode; // Changes made by Karl

private:

};

#endif // !PLAYER_H