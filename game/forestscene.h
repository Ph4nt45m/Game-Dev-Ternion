// COMP710 GP Framework 2022

#ifndef FORESTSCENE_H
#define FORESTSCENE_H

// Local includes:
#include "scene.h"

// Forward declarations: 
class Renderer;
class Forest;

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

protected:


private:
	ForestScene(const ForestScene& forestScene);
	ForestScene& operator=(const ForestScene& forestScene);

	// Member data:
public:
	int m_iFrameWidth;
	int m_iFrameHeight;

protected:
	Forest* m_frtSegments[20];
	int m_iShowCount;

	static int sm_iSegmentWidth;

private:
	
};

#endif // FORESTSCENE_H

