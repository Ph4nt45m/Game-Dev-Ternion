#ifndef FOREST_H
#define FOREST_H

// Local includes:
#include "vector2.h"

// Forward declarations:
class Renderer;
class Sprite;

class Forest
{
	// Member methods:
public:
	Forest();
	~Forest();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime);
	void Draw(Renderer& renderer);

	Sprite* GetSprite();
	void SetSprite(Sprite* sprite);
	void SetX(int x);
	void SetY(int y);
	void SetAngle(float angle);
	void SetIndex(int index);
	bool IsAlive();
	void SetAlive(bool alive);

	void DebugDraw();

protected:


private:
	Forest(const Forest& forest);
	Forest& operator=(const Forest& forest);

	// Member data:
public:


protected:
	Sprite* m_pSprSprite;
	int m_iSegmentIndex;
	bool m_bAlive;

private:

};

#endif // !FOREST_H
