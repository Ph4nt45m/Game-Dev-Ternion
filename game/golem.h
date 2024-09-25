#ifndef GOLEM_H
#define GOLEM_H

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "inputsystem.h"

// Forward declarations:
class Renderer;
class Sprite;
class AnimatedSprite;
class Character;
class Projectile;

// Golem animations:
typedef struct {
    AnimatedSprite* m_pASprGolemWalk;
    AnimatedSprite* m_pASprGolemSlash;
    AnimatedSprite* m_pASprGolemJump;
    AnimatedSprite* m_pASprGolemSlam;
    AnimatedSprite* m_pASprGolemThrow;
} Animations;

// Class declaration:
class Golem : public Entity
{
    // Member methods:
public:
    Golem();
    ~Golem();

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
    void SetCharacter(Character& character);
    void CheckPlayerDist();
    int GetBodyWidth();
    void ShiftX(float amount);
    void SetNumWalkableSegs(int amount);

    void Move(int attackType);
    void Action();
    void ProcessAction();

    //void DebugDraw() override;

protected:
    void ComputeBounds(float width, float height);

private:
    Golem(const Golem& golem);
    Golem& operator=(const Golem& golem);

    // Member data:
protected:
    Character* m_pEntCharacter;
    Sprite* m_pSprSpriteBody;
    Projectile* m_pEntProjectile;
    Vector2 m_vStartingPos;
    Animations m_sAnimations;
    int m_iNumSegments;
    int m_iNumWalkableSegs;
    float m_fAnimateScale;
    float m_fExecutionTime;
    int m_iAttackType;
    float m_fDistToPlayer;
    float m_fHitBoxRange;
    float m_fSlashRangeMax;
    float m_fSlamRangeMax;
    float m_fThrowRangeMax;
    float m_fGroundY;
    bool m_bPlayerInRange;
    bool m_bSpotted;
    bool m_bEngage;
    bool m_bIsAnimating;
    bool m_bSlam;
    bool m_bWalk;

private:
    
};

#endif // !GOLEM_H


