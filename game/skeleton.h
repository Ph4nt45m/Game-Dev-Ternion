#ifndef SKELETON_H
#define SKELETON_H

// Local includes:
#include "entity.h"
#include "enemy.h"
#include "vector2.h"
#include "inputsystem.h"
#include "healthbar.h"

#include <Box2D.h>

// Forward declarations:
class Renderer;
class Sprite;
class AnimatedSprite;
class Player;

// Golem animations:
typedef struct {
    AnimatedSprite* m_pASprSkelIdle;
    AnimatedSprite* m_pASprSkelWalk;
    AnimatedSprite* m_pASprSkelAttack;
} SkelAnimations;

// Class declaration:
class Skeleton : public Enemy
{
    // Member methods:
public:
    Skeleton(b2World* world);
    ~Skeleton() override;

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer, Camera& camera) override;
    bool SetBodySprites(Renderer& renderer) override;

    void CheckPlayerDist();
    int GetBodyWidth();

    void SetCamera(Camera* camera);
    void SetPlayer(Player* player);
    void CreateSlash();
    void DeleteSlash();


    void Move(void);
    void Action();
    void ProcessAction();

    Healthbar* getEnemyHealth();//Changes made by Kyle

    //void DebugDraw() override;

protected:

private:
    Skeleton(const Skeleton& skeleton);
    Skeleton& operator=(const Skeleton& skeleton);

    // Member data:
protected:
    Player* m_pEntCharacter;
    Sprite* m_pSprSpriteBody;
    Vector2 m_vStartingPos;
    Healthbar* healthBar;
    SkelAnimations m_sAnimations;
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

    b2World* m_pWorld;
    b2Body* m_pBody;
    b2Body* m_pSlash;
    Camera* m_pCamera;

private:

};

#endif // !SKELETON_H