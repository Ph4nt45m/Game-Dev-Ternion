#ifndef MUSHROOM_H
#define MUSHROOM_H

// Local includes:
#include "entity.h"
#include "enemy.h" // Changes made by Karl
#include "vector2.h"
#include "inputsystem.h"

// Forward declarations:
class Renderer;
class Sprite;
class AnimatedSprite;
class Character;

// Golem animations:
typedef struct {
    AnimatedSprite* m_pASprMushIdle;
    AnimatedSprite* m_pASprSkelWalk;
    AnimatedSprite* m_pASprMushAttack;
} Animations;

// Class declaration:
class Mushroom : public Enemy
{
    // Member methods:
public:
    Mushroom();
    ~Mushroom();

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

    void Move(void);
    void Action();
    void ProcessAction();

    //void DebugDraw() override;

protected:
    void ComputeBounds(float width, float height);

private:
    Mushroom(const Mushroom& mushroom);
    Mushroom& operator=(const Mushroom& mushroom);

    // Member data:
protected:
    Character* m_pEntCharacter;
    Sprite* m_pSprSpriteBody;
    Vector2 m_vStartingPos;
    Animations m_sAnimations;
    int m_iNumSegments;
    int m_iNumWalkableSegs;
    float m_fAnimateScale;
    float m_fExecutionTime;
    float m_fDistToPlayer;
    float m_fHitBoxRange;
    float m_fAttackRangeMax;
    float m_fGroundY;
    bool m_bPlayerInRange;
    bool m_bSpotted;
    bool m_bEngage;
    bool m_bIsAnimating;
    bool m_bAttack;
    bool m_bRun;

private:

};

#endif // !MUSHROOM_H