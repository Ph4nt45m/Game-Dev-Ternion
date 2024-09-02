#ifndef ENTITY_H
#define ENTITY_H

// Forward declarations:
class Renderer;
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

// Represents motion of the entity
typedef struct {
	int Surge;		// Move forward or back		[<0 = Backward,	0 = Stop, >0 = Forward]
	int Sway;		// Move up or down (strafe)	[<0 = Down, 0 = Stop, >0 = Up]
	int Heave;		// Jump	or decent			[<0 = Decent, 0 = Stop, >0 = Jump]
} Motions;

// Class declaration: 
class Entity
{
	// Member methods: 
public:
	Entity();
	virtual ~Entity();
	virtual bool Initialise(Renderer& renderer) = 0;
	virtual void Process(float deltaTime, InputSystem& inputSystem) = 0;
	virtual void Draw(Renderer& renderer) = 0;

	//virtual void DebugDraw() = 0;

protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	// Member data: 
public:
	

protected:


private:

};

#endif // !ENTITY_H

