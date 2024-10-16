#ifndef GOLEM_H
#define GOLEM_H

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "inputsystem.h"
#include "golem.h"
#include "Camera.h"
#include "player.h"
#include "enemy.h"

//Box2d
#include<Box2D.h>

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
}Animations;

// Class declaration:
class Golem : public Enemy
{
    // Member methods:
public:
    Golem(b2World* world);
    ~Golem();

    bool Initialise(Renderer& renderer) override;
    void Process(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer, Camera& camera) override;
    bool SetBodySprites(Renderer& renderer) override;

    Vector2& GetPosition();
    void CheckPlayerDist();
    void SetCamera(Camera* camera);
    void ProcessAction();

    void Move(int attackType);
    void Action(float deltaTime);
    void SetPlayer(Player* player);
    void CreateSlashBody();
    void CreateSlamBody();
    void DeleteSlash();
    void DeleteSlam();

    //Kyle Changes
    Healthbar* getEnemyHealth();//Changes made by Kyle
    bool getIsAlive();
    void DeleteBody();


    //void DebugDraw() override;

protected:

private:
    Golem(const Golem& golem);
    Golem& operator=(const Golem& golem);

public:
    //box2d verables
    b2World* m_pWorld;
    b2Body* m_pBody;
    b2Body* m_pSlashBody;
    b2Body* m_pSlamBody;

    //attacks
    float slashWidth;
    float slashHeight;
    float m_fSlamWidth;
    float m_fSlamHeight;

    Camera* m_pcamera;
    bool IsCameraSet;

    // Member data:
protected:
    Player* m_pEntCharacter;
    Sprite* m_pSprSpriteBody;
    Projectile* m_pEntProjectile;
    Vector2 m_vStartingPos;
    Animations m_sAnimations;
    Healthbar* healthBar;//Changes made by kyle
    float m_fAnimateScale;
    float m_fExecutionTime;
    int m_iAttackType;
    float m_fDistToPlayer;
    float m_fSlashRangeMax;
    float m_fSlamRangeMax;
    float m_fThrowRangeMax;
    float m_fGroundY;
    float m_fSlashWidth;
    float m_fSlashHeight;
    bool m_bPlayerInRange;
    bool m_bSpotted;
    bool m_bEngage;
    bool m_bIsAnimating;
    bool m_bSlam;
    bool m_bWalk;
    float m_fSlamTime;
    float growSize;

private:

};

#endif // !GOLEM_H