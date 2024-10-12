#ifndef __GAME_H_
#define __GAME_H_

// Local includes:
#include "scene.h"
#include "vector2.h"
#include "inputsystem.h"
#include "MyContactListener.h"
#include "SoundManager.h"

// Library includes:
#include <vector>

//box2D
#include <Box2D.h>

// Forward declarations:
class Renderer;
class Sprite;
class AnimatedSprite;
class Character;
class Player; // Changes made by Karl
class scene;
class Game
{
	// Member methods:
public:
	static Game& GetInstance(); 
	static void DestroyInstance();

	void ToggleDebugWindow();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	b2Vec2 GetGravity();
	void SetGravity(float x, float y);

	void CreateCharacter(int type); // Changes made by Karl
	//Character* GetCharacter() const; // Changes made by Karl
	Player* GetCharacter() const;
	b2World* GetWorld() const;

protected:
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	void ProcessFrameCounting(float deltaTime);

	void DebugDraw();


private:
	Game();
	~Game();
	Game(const Game& game);
	Game& operator=(const Game& game);



	// Member data:
public:
	//Box2D
	
	b2World* world;
	b2Vec2 m_gravity;

	//Box2D stuff

protected:
	static Game* sm_pInstance;
	Renderer* m_pRenderer;
	std::vector<Scene*> m_scenes; 
	int m_iCurrentScene;
	__int64 m_iLastTime;
	float m_fExecutionTime;
	float m_fElapsedSeconds;
	int m_iFrameCount;
	int m_iFPS;

	bool m_bShowDebugWindow;

	InputSystem* m_pInputSystem;

	Vector2 m_pCursor;
	Scene* m_pScForestScene;
	/*Character* m_pEntCharacter;*/ // Changes made by Karl
	Player* m_pEntCharacter;

#ifdef USE_LAG
	float m_fLag;
	int m_iUpdateCount;
#endif // USE_LAG

	bool m_bLooping;

private:
	//Sprite* m_pCheckerboard;
	AnimatedSprite* m_pASprAnimatedSprite;
	Sprite* m_sprCursorBodySprite;
	Sprite* m_sprCursorBorderSprite;
	int m_iMouseState;

	SoundManager* soundManager;

	MyContactListener m_contactListener;
};
#endif // __GAME_H_
