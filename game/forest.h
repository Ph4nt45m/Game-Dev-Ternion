#ifndef FOREST_H
#define FOREST_H

// Local includes:
#include "vector2.h"
#include "inputsystem.h"

// Forward declarations:
class Renderer;
class InputSystem;
class Sprite;
class Character;

// Represents the states of a set of keys used to side-scroll the terrain
typedef struct {
	ButtonState MoveForward;
	ButtonState MoveBackward;
} TerrainMotions;

enum SegmentType
{
	ST_BOUNDARY,
	ST_LEFTWALL,
	ST_RIGHTWALL,
	ST_BLANK,
	ST_PLATFORM,
	ST_FLOATINGPLATFORM,
	ST_TALLPLATFORM
};

class Forest
{
	// Member methods:
public:
	Forest();
	~Forest();

	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);

	Sprite* GetSprite();
	void SetSprite(Sprite* sprite);
	void SetNumSegments(int amount);
	int GetX();
	void SetX(int x);
	int GetY();
	void SetY(int y);
	int GetHeight();
	void SetAngle(float angle);
	void SetScale(float scale);
	void SetIndex(int index);
	void SetType(int type);
	void SetHorizontalFlip(bool flip);
	bool IsAlive();
	void SetAlive(bool alive);
	void SetBounds();

	void SetCharacter(Character& character);
	void SetCharPos(Vector2& position);
	void SetCharFeetPos(Vector2& feetPos);
	void SetCharWidth(int width);

	int GetLBX();
	int GetUBX();
	float GetPos();
	float GetV();

	void DebugDraw();

protected:
	void GetInputs(InputSystem& inputSystem);
	void HandleInputs();
	void HandlePlayerCollisions();
	void HandleProjCollisions();

private:
	Forest(const Forest& forest);
	Forest& operator=(const Forest& forest);

	// Member data:
public:


protected:
	TerrainMotions m_sTerrainMotions;
	int m_iSurge;
	Sprite* m_pSprSprite;
	int m_iNumSegments;
	int m_iSegmentIndex;
	int m_iSegmentType;
	float m_fPositionX;
	int m_iLowerBound;
	int m_iUpperBound;
	float m_fFloorLowerBound;
	float m_fFloorUpperBound;
	float m_fPlatformHeight;
	bool m_bLeftCollision;
	bool m_bRightCollision;
	bool m_bHorizontalFlip;
	bool m_bAlive;

	Character* m_pEntCharacter;
	Vector2* m_vCharPos;
	Vector2* m_vFeetPos;
	int m_iCharWidth;

	static int sm_iSegmentWidth;
	static int sm_iSegmentHeight;
	static int sm_iScreenWidth;
	static int sm_iScreenHeight;
	static float sm_fVelocity;
	static bool sm_bLeftCollision;
	static bool sm_bRightCollision;
	static int sm_iTerrainLevel;

private:

};

#endif // !FOREST_H
