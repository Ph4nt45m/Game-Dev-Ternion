#ifndef PROJECTILE_H
#define PROJECTILE_H

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "inputsystem.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;

// Class declaration:
class Projectile : public Entity
{
	// Member methods:
public:
	Projectile();
	~Projectile();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer, Camera& camera) override;
    bool SetBodySprites(Renderer& renderer) override;

    Vector2& GetPosition();
    bool SetProjectileSprite(Renderer& renderer, const char* filePath);
    void SetScale(float scale); // Changes made by Karl
    float GetWidth();
    float GetHeight();
    Sprite* GetSprite();
    void SetStartPos(float st_x, float st_y);
    void SetTargetPos(float t_x, float t_y);
    void SetGroundY(float g_y);
    void SetTimeToTarget(float time);
    void Shoot();
    bool IsAlive();
    void SetAlive(bool alive);
    bool HasTargetPos() const { return targetSet; }

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
    bool targetSet = false;

protected:


private:

};

#endif // !PROJECTILE_H

