#ifndef PROJECTILEARROW_H
#define PROJECTILEARROW_H

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "inputsystem.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;

// Class declaration:
class ProjectileArrow : public Entity
{
	// Member methods:
public:
	ProjectileArrow();
	~ProjectileArrow();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
    bool SetBodySprites(Renderer& renderer) override;
    void SetNumSegments(int amount) override;
    void GetInputs(InputSystem& inputSystem) override;
    void HandleInput(float deltaTime) override;
    void SetTerrainMoving(bool moving) override;
    bool IsTerrainMoving() override;

    Vector2& GetPosition();
    float GetWidth();
    float GetHeight();
    void SetStartPos(float st_x, float st_y);
    void SetTargetPos(float t_x, float t_y);
    void SetGroundY(float g_y);
    void Shoot();
    bool IsAlive();
    void SetAlive(bool alive);

protected:


private:


	// Member data:
public:
    Vector2 m_vTarget;
    float m_fTimeToTarget;
    float m_fGravity;
    float m_fDX;
    float m_fDY;
    float m_fGroundY;
    float m_fWidth;
    float m_fHeight;

protected:


private:

};

#endif // !PROJECTILEARROW_H

