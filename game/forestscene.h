// COMP710 GP Framework 2022

#ifndef FORESTSCENE_H
#define FORESTSCENE_H

// Local includes:
#include "scene.h"
#include "inputsystem.h"

// Forward declarations: 
class Renderer;
class InputSystem;
class Forest;
class Character;
class Golem;

// Class declarations:
class ForestScene : public Scene
{
	// Member methods:
public:
	ForestScene();
	virtual ~ForestScene();
	virtual bool Initialise(Renderer& renderer);
	virtual void Process(float deltaTime, InputSystem& inputSystem);
	virtual void Draw(Renderer& renderer);
	virtual void DebugDraw();

	virtual void SetCharacter(Character& character, Renderer& renderer);
	virtual void SetCharPos(Vector2& characterPos);
	virtual void SetCharFeetPos(Vector2& feetPos);
	virtual void SetCharWidth(int width);
	virtual bool SetEnemies(Character& character, Renderer& renderer);

protected:
	

private:
	ForestScene(const ForestScene& forestScene);
	ForestScene& operator=(const ForestScene& forestScene);

	// Member data:
public:
	

protected:
	Forest* m_pFrtSegments[10];
	Golem* m_pGolem;

private:
	
};

#endif // FORESTSCENE_H

