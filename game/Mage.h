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
    void ProcessActions(float deltaTime); // Changes made by Karl

    b2Vec2 GetPosition() override;
    Vector2& GetVelocityBody();
    int GetCharactertype() override; // Changes made by Karl
    void SetCharacterType(Renderer& renderer, int type) override;
    void DefineCharacter(Renderer& renderer);
    bool IsDefined();
    void SetDefined(bool define);
    void CreateSPAttack();
    void DeleteSPAttack();
    Vector2& GetProjectilePos();
    float GetProjWidth();
    float GetProjHeight();
    bool IsProjAlive();
    void SetProjAlive(bool alive);

    //Changes by kyle
    Healthbar* getPlayerHealthbar() override;
    void setCharacterPos() override;

    // Changes made by Karl
    virtual void SetAlive(bool alive) override;
    virtual bool IsGodmode() override;

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
    bool m_bDefined;

    //Box2D verables
    b2Body* m_pBody;
    b2World* m_pWorld;
    b2Body* m_pSPAttackBody; // Changes made by Karl

    Actions m_sActions;
private:
    float m_fHeadBodyOffset;
    float m_fLengthFootToBody;
    float m_fLegBodyOffset;
    float m_fElapsedTime; // Changes made by Karl
    float m_fStepTimer;
    float m_fStepDuration;
    float m_jumpTimer;
    float m_fOffset;
    float m_fPlayerWidth; // Changes made by Karl
    float m_fPlayerHeight;
    float m_fAttackWidth;
    float m_fAttackHeight;
};

#endif // !MAGE_H

