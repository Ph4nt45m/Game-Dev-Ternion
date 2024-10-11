#ifndef GOLEM_H
#define GOLEM_H

// Local includes:
#include "entity.h"
#include "enemy.h" // Changes made by Karl
#include "vector2.h"
#include "inputsystem.h"
#include "Camera.h"

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
} Animations;

// Class declaration:
class Golem : public Enemy // Changes made by Karl
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

    void Move(int attackType);
    void Action();
    void ProcessAction();
    void SetPlayer(Character* player);

    //void DebugDraw() override;

protected:

private:
    Golem(const Golem& golem);
    Golem& operator=(const Golem& golem);

public: 
    //box2d verables
    b2World* m_pWorld;
    b2Body* m_pBody;

    //attacks
    b2Body* slashBody;
    float slashWidth;
    float slashHeight;

    Camera* m_pcamera;
    bool IsCameraSet;
    const float SCALE = 30.0f;

    // Member data:
protected:
    Character* m_pEntCharacter;
    Sprite* m_pSprSpriteBody;
    Projectile* m_pEntProjectile;
    Vector2 m_vStartingPos;
    Animations m_sAnimations;
    float m_fAnimateScale;
    float m_fExecutionTime;
    int m_iAttackType;
    float m_fDistToPlayer;
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
