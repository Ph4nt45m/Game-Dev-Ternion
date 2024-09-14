#ifndef CHARACTER_H
#define CHARACTER_H

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "inputsystem.h"

// Forward declarations:
class Renderer;
class InputSystem;
class Sprite;
class AnimatedSprite;
class ProjectileArrow;

// Class declaration:
class Character : public Entity
{
    // Member methods:
public:
    Character();
    ~Character();

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
    Vector2& GetFeetPos();
    Vector2& GetVelocityBody();
    int GetWeapontype();
    Vector2& GetProjectilePos();
    float GetProjWidth();
    float GetProjHeight();
    bool IsProjAlive();
    void SetProjAlive(bool alive);
    void SetCentered(bool centered);
    bool IsCentered();
    int GetBodyWidth();
    void ShiftX(float amount);
    void ShiftY(float amount);

    //void DebugDraw() override;

protected:
    void ComputeBounds(float width, float height);
    void HandleLegs(float deltaTime);

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
    ProjectileArrow* m_pEntArrow;
    AnimatedSprite* m_pASprWeapAttack;
    int m_iWeaponType;
    float m_fAngleOfAttack;
    bool m_bDoubleJump;

private:
    float m_fHeadBodyOffset;
    float m_fLengthFootToBody;
    float m_fLegBodyOffset;
    float m_fStepTimer;
    float m_fStepDuration;
};

#endif // !CHARACTER_H

