#ifndef CHARACTER_H
#define CHARACTER_H

// Local includes:
#include "entity.h"
#include "vector2.h"
#include "inputsystem.h"

// Forward declarations:
class Renderer;
class Sprite;
class InputSystem;

// Represents the states of a set of keys used to control an entity
typedef struct {
    ButtonState MoveForward;
    ButtonState MoveBackward;
    ButtonState MoveUp;
    ButtonState MoveDown;
    ButtonState Jump;
} MotionKeys;

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

    Vector2& GetPosition();

    //void DebugDraw() override;

protected:
    void GetInputs(InputSystem& inputSystem);
    void HandleInput(float deltaTime);
    void ComputeBounds(int width, int height);
    void HandleLegs(float deltaTime);

private:
    Character(const Character& character);
    Character& operator=(const Character& character);

    // Member data:
protected:
    Vector2 m_vPosition;
    Vector2 m_vFeetPos;
    Vector2 m_vStandingPos;
    Sprite* m_pSprSpriteHead;
    Sprite* m_pSprSpriteBody;
    Sprite* m_pSprSpriteLegLeft;
    Sprite* m_pSprSpriteLegRight;
    Sprite* m_pSprSpriteShadow;
    bool m_bAlive;
    int m_iFacingDirection;
    float m_fTotalHeight;

    static float sm_fBoundaryWidth;
    static float sm_fBoundaryHeight;

private:
    MotionKeys m_sMotionKeyStates;
    Motions m_sKeyboardMotions;

    Vector2 m_velocityBody;
    Vector2 m_velocityPos;
    Vector2 m_velocityJump;
    Vector2 m_vBoundaryLow;
    Vector2 m_vBoundaryHigh;
    Vector2 m_vLegBoundaryLow;
    Vector2 m_vLegBoundaryHigh;
    bool m_bMovingX;
    bool m_bMovingY;
    bool m_bJumping;
    float m_fHeadBodyOffset;
    float m_fLengthFootToBody;
    float m_fLegBodyOffset;
    float m_fStepTimer;
    float m_fStepDuration;
};

#endif // !CHARACTER_H

