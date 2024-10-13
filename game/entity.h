#ifndef ENTITY_H
#define ENTITY_H

// Local includes:
#include "renderer.h"
#include "vector2.h"
#include "inputsystem.h"
#include "Camera.h"

// Forward declarations:
class Renderer;
class Vector2;
class Sprite;
class InputSystem;

// Definition of pi
#define PI 3.14159265f

// Definitions for motions
#define MOTION_NONE 0				// No motion
#define MOTION_BACKWARD -1			// Backward motion
#define MOTION_FORWARD 1			// Forward motion
#define MOTION_DOWN -1				// Downward motion
#define MOTION_UP 1					// Upward motion
#define MOTION_JUMP 1               // Jump motion
#define MOTION_DECENT -1            // Decent motion
#define MOTION_ATTACK 1				// Attack motion

// Scaling factor
#define SCALE 30.0f

// Represents motion of the entity
typedef struct {
	int Surge;		// Move forward or back		[<0 = Backward,	0 = Stop, >0 = Forward]
	int Sway;		// Move up or down (strafe)	[<0 = Down, 0 = Stop, >0 = Up]
	int Heave;		// Jump	or decent			[<0 = Decent, 0 = Stop, >0 = Jump]
	int Attack;		// Attack or idle			[0 = Idle, >0 = Attack]
} Motions;

// Represents the states of a set of keys used to control an entity
typedef struct {
	ButtonState MoveForward;
	ButtonState MoveBackward;
	ButtonState MoveUp;
	ButtonState MoveDown;
	ButtonState Jump;
	ButtonState LeftClickAttack;
} MotionKeys;

// Class declaration: 
class Entity
{
	// Member methods: 
public:
	Entity();
	virtual ~Entity();
	virtual bool Initialise(Renderer& renderer) = 0;
	virtual void Process(float deltaTime, InputSystem& inputSystem) = 0;
	virtual void Draw(Renderer& renderer, Camera& camera) = 0;
	virtual bool SetBodySprites(Renderer& renderer) = 0;

	static void SetWindowBoundaries(Renderer& renderer)
	{
		sm_fBoundaryWidth = (float)renderer.GetWidth();
		sm_fBoundaryHeight = (float)renderer.GetHeight();
		sm_fSegmentWidth = 390.0f;
	}

	//virtual void DebugDraw() = 0;

protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	// Member data: 
public:
	

protected:
	MotionKeys m_sMotionKeyStates;
	Motions m_sKeyboardMotions;
	Sprite* m_pSprSpriteBody;
	Vector2 m_vPosition;
	Vector2 m_vFeetPos;
	Vector2 m_vStandingPos;
	Vector2 m_vProjectilePos;
	Vector2 m_vProjectileStartPos;
	Vector2 m_vProjectileEndPos;
	Vector2 m_velocityBody;
	Vector2 m_velocityPos;
	Vector2 m_velocityJump;
	Vector2 m_velocityProjectile;
	Vector2 m_velocityDirection;
	Vector2 m_vBoundaryLow;
	Vector2 m_vBoundaryHigh;
	int m_iFacingDirection;
	float m_fScale;
	float m_fScaleChangeRate;
	float m_fScaleMin;
	float m_fScaleMax;
	float m_fAngleOfProjectile;
	bool m_bFlipHorizontally;
	bool m_bMovingX;
	bool m_bMovingY;
	bool m_bJumping;
	bool m_bSlash;
	bool m_bShoot;
	bool m_bProjectile;
	bool m_bAlive;

	static int sm_iNumSegments;
	static float sm_fBoundaryWidth;
	static float sm_fBoundaryHeight;
	static float sm_fSegmentWidth;
	static bool sm_bCameraCentered;
	static bool sm_bTerrainMoving;

private:

};

#endif // !ENTITY_H

