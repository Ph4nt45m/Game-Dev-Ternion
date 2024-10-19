// COMP710 GP Framework 2022

#ifndef FORESTNIGHT_H
#define FORESTNIGHT_H

// Local includes:
#include "scene.h"
#include "inputsystem.h"
#include "Terrain.h"
#include "Camera.h"
#include "LevelBuilder.h"
#include "enemy.h"
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

typedef struct
{
	Sprite* m_pBackground;
	Sprite* m_pForeground;
	Sprite* m_pGround;
} NightBackGround;

// Class declarations:
class ForestNight : public Scene
{
	// Member methods:
public:
	ForestNight(b2World* world, Player* character);
	virtual ~ForestNight();
	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();

	bool SetEnemies(Renderer& renderer);
	bool SetBGSprites(Renderer& renderer); // Changes made by Karl - Function for setting background sprites
	void UpdateBackground();

protected:


private:
	ForestNight(const ForestNight& forestTest);
	ForestNight& operator=(const ForestNight& forestTest);

	// Member data:
public:
	//Box2D
	b2World* m_pWorld;

protected:
	NightBackGround m_sDayFrameOne; // Changes made by Karl - Start - Structs for background frames instead of singular sprite variables
	NightBackGround m_sDayFrameTwo;
	NightBackGround m_sFogFrameOne;
	NightBackGround m_sFogFrameTwo; // Changes made by Karl - End
	Golem* m_pGolem;
	Skeleton* m_pSkeleton; // Changes made by Karl
	Spider* m_pSpider;
	Player* m_pCharacter;
	Camera camera;
	Terrain* ground;

	LevelBuilder* level;

	float m_fWindowWidth;
	float m_fWindowHeight;
	int m_iBackground;
	float m_fLoopRange;
private:
	std::vector<Enemy*> m_vEnemies;
};

#endif // FORESTTEST_H
