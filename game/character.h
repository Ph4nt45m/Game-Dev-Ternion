#ifndef CHARACTER_H
#define CHARACTER_H

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "inputsystem.h"
#include "healthbar.h"
#include "Camera.h"

//Box2D
#include <Box2D.h>

// Forward declarations:
class Renderer;
class InputSystem;
class Sprite;
class AnimatedSprite;
class Projectile;
class Game;

// Class declaration:
class Character : public Entity
{
    // Member methods:
public:
    Character(b2World* world);
    ~Character();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void DrawWithCam(Renderer& renderer, Camera& camera);
    void Draw(Renderer& renderer, Camera& camera) override;
    bool SetBodySprites(Renderer& renderer) override;
    void GetInputs(InputSystem& inputSystem);
    void HandleInput(float deltaTime, InputSystem& inputSystem);

    b2Vec2 GetPosition();
    Vector2& GetVelocityBody();
    int GetWeapontype();
    Vector2& GetProjectilePos();
    float GetProjWidth();
    float GetProjHeight();
    bool IsProjAlive();
    void SetProjAlive(bool alive);

    //void DebugDraw() override;

protected:

private:
    Character(const Character& character);
    Character& operator=(const Character& character);

    // Member data:
protected:
    Vector2 m_vCursor;
    Vector2 m_vRelative;
    Sprite* m_pSprSpriteHead;
    Sprite* m_pSprSpriteLegLeft;
    Sprite* m_pSprSpriteLegRight;
    Sprite* m_pSprSpriteShadow;
    Sprite* m_pSprWeapon;
    Projectile* m_pEntArrow;
    AnimatedSprite* m_pASprWeapAttack;
    Healthbar* m_pHealthbar;
    int m_iWeaponType;
    float m_fAngleOfAttack;
    bool m_bDoubleJump;

    //Box2D verables
    b2Body* m_pBody;
    b2World* m_pWorld;


private:
    float m_fHeadBodyOffset;
    float m_fLengthFootToBody;
    float m_fLegBodyOffset;
    float m_fStepTimer;
    float m_fStepDuration;
    float m_jumpTimer;
};

#endif // !CHARACTER_H

