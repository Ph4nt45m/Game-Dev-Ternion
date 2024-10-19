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
	virtual bool Initialise(Renderer& renderer) = 0;
	virtual void Process(float deltaTime, InputSystem& inputSystem) = 0;
	virtual void Draw(Renderer& renderer, Camera& camera) = 0;
	virtual bool SetBodySprites(Renderer& renderer) = 0;
	bool GetAlive();

	//virtual void DebugDraw() = 0;

protected:

private:
	Enemy(const Enemy& enemy);
	Enemy& operator=(const Enemy& enemy);

	// Member data: 
public:
	bool IsCameraSet = false;

protected:


private:

};

#endif // !ENEMY_H