#ifndef ENEMY_H
#define ENEMY_H

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
class Enemy : public Entity
{
	// Member methods: 
public:
	Enemy();
	virtual ~Enemy();

	//virtual void DebugDraw() = 0;

protected:

private:
	Enemy(const Enemy& enemy);
	Enemy& operator=(const Enemy& enemy);

	// Member data: 
public:


protected:
	

private:

};

#endif // !ENEMY_H

