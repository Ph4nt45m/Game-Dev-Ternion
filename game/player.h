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

// Class declaration: 
class Player : public Entity
{
	// Member methods: 
public:
	Player();
	virtual ~Player();

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

#endif // !ENEMY_H

