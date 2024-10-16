// COMP710 GP Framework 2022

#ifndef FORESTTEST_H
#define FORESTTEST_H

// Local includes:
#include "scene.h"
#include "inputsystem.h"
#include "Terrain.h"
#include "Camera.h"
#include <vector>
//Box2D
#include <Box2D.h>

// Forward declarations: 
class Renderer;
class InputSystem;
class Forest;
class Player;
class Golem;
class Mushroom;
class Skeleton; // Changes made by Karl
class Spider;
class Sprite;

// Class declarations:
class ForestTest : public Scene
{
	// Member methods:
public:
	ForestTest(b2World* world, Player* charcter);
	virtual ~ForestTest();
	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();

	bool SetEnemies(Renderer& renderer);

protected:


private:
	ForestTest(const ForestTest& forestTest);
	ForestTest& operator=(const ForestTest& forestTest);

	// Member data:
public:
	//Box2D
	b2World* m_pWorld;

protected:
	Sprite* m_pBackground;
	Golem* m_pGolem;
	Mushroom* m_pMushroom;
	Skeleton* m_pSkeleton; // Changes made by Karl
	Spider* m_pSpider;
	Player* m_pCharacter;
	Camera camera;
	Terrain* ground;
	Terrain* leftWall;
	Terrain* rightWall;
	Terrain* platform;
private:
	std::vector<Terrain*> m_terrainSegments;
};

#endif // FORESTTEST_H

