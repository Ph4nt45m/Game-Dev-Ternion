#ifndef MAGE_H
#define MAGE_H

// Local includes:
#include "entity.h"
#include "player.h"
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
class Mage : public Player
{
    // Member methods:
public:
    Mage(b2World* world);
    ~Mage();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void DrawWithCam(Renderer& renderer, Camera& camera) override;
    void Draw(Renderer& renderer, Camera& camera) override;
    bool SetBodySprites(Renderer& renderer) override;
    void GetInputs(InputSystem& inputSystem);
    void HandleInput(float deltaTime, InputSystem& inputSystem);

    b2Vec2 GetPosition() override;
    Vector2& GetVelocityBody();
    int GetCharactertype() override; // Changes made by Karl
    void SetCharacterType(Renderer& renderer, int type) override;
    void DefineCharacter(Renderer& renderer);
    bool IsDefined();
    void SetDefined(bool define);
    Vector2& GetProjectilePos();
    float GetProjWidth();
    float GetProjHeight();
    bool IsProjAlive();
    void SetProjAlive(bool alive);

    //void DebugDraw() override;

protected:

private:
    Mage(const Mage& mage);
    Mage& operator=(const Mage& mage);

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
    int m_iCharacterType;
    float m_fAngleOfAttack;
    bool m_bDoubleJump;
    bool m_bDefined;

    //Box2D verables
    b2Body* m_pBody;
    b2World* m_pWorld;

    Actions m_sActions;
private:
    float m_fHeadBodyOffset;
    float m_fLengthFootToBody;
    float m_fLegBodyOffset;
    float m_fStepTimer;
    float m_fStepDuration;
    float m_jumpTimer;
    float offset;
};

#endif // !MAGE_H

