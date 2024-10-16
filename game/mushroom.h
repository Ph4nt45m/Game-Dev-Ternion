#ifndef MUSHROOM_H
#define MUSHROOM_H

// Local includes:
#include "entity.h"
#include "enemy.h" // Changes made by Karl
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
    AnimatedSprite* m_pASprMushIdle;
    AnimatedSprite* m_pASprMushWalk;
    AnimatedSprite* m_pASprMushAttack;
} MushAnimations;

// Class declaration:
class Mushroom : public Enemy
{
    // Member methods:
public:
    Mushroom(b2World* world);
    ~Mushroom();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer, Camera& camera) override;
    bool SetBodySprites(Renderer& renderer) override;

    void CheckPlayerDist();
    int GetBodyWidth();

    void SetCamera(Camera* camera);
    void SetPlayer(Player* player);
    void CreateHeadButt();
    void DeleteHeadButt();
    void DeleteBody();


    void Move(void);
    void Action();
    void ProcessAction();

    //Kyle Changes
    Healthbar* getEnemyHealth();//Changes made by Kyle

    //void DebugDraw() override;

protected:

private:
    Mushroom(const Mushroom& mushroom);
    Mushroom& operator=(const Mushroom& mushroom);

    // Member data:
protected:
    Player* m_pEntCharacter;
    Sprite* m_pSprSpriteBody;
    Vector2 m_vStartingPos;
    MushAnimations m_sAnimations;
    Healthbar* healthBar;//Changes made by kyle
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
    b2Body* m_pHeadButt;
    Camera* m_pCamera;

private:

};

#endif // !MUSHROOM_H