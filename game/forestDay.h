// COMP710 GP Framework 2022

#ifndef FORESTDAY_H
#define FORESTDAY_H

// Local includes:
#include "scene.h"
#include "inputsystem.h"
#include "Terrain.h"
#include "Camera.h"
#include "LevelBuilder.h"
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
} DayBackGround;

// Class declarations:
class ForestDay : public Scene
{
	// Member methods:
public:
	ForestDay(b2World* world, Player* character);
	virtual ~ForestDay();
	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();

	bool SetEnemies(Renderer& renderer);
	bool SetBGSprites(Renderer& renderer); // Changes made by Karl - Function for setting background sprites
	void UpdateBackground();

protected:


private:
	ForestDay(const ForestDay& forestTest);
	ForestDay& operator=(const ForestDay& forestTest);

	// Member data:
public:
	//Box2D
	b2World* m_pWorld;

protected:
	DayBackGround m_sDayFrameOne; // Changes made by Karl - Start - Structs for background frames instead of singular sprite variables
	DayBackGround m_sDayFrameTwo;
	DayBackGround m_sFogFrameOne;
	DayBackGround m_sFogFrameTwo; // Changes made by Karl - End
	Golem* m_pGolem;
	Mushroom* m_pMushroom;
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
	std::vector<Terrain*> m_terrainSegments;
};

#endif // FORESTDAY_H
