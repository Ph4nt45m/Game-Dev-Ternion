#ifndef PLAYER_H
#define PLAYER_H

// Local includes:
#include "renderer.h"
#include "vector2.h"
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
	AnimatedSprite* m_pASpriteBlock; // Changes made by Karl - Start
	AnimatedSprite* m_pASpriteHurt;
	AnimatedSprite* m_pASpriteDeath; // Changes made by Karl - End
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

	//virtual void DebugDraw() = 0;

protected:

private:
	Player(const Player& player);
	Player& operator=(const Player& player);

	// Member data: 
public:


protected:


private:

};

#endif // !PLAYER_H