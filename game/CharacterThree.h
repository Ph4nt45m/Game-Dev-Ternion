#pragma once
#ifndef __CHARACTERTHREE_H_
#define __CHARACTERTHREE_H_

#include "button.h"
#include "sceneManager.h"
#include "sprite.h" // Changes made by Karl
#include "animatedsprite.h"

class CharacterThree : public Button
{
public:
    CharacterThree(float x, float y);
    virtual ~CharacterThree();

    // Override the virtual functions from Button
    bool Initialise(Renderer& renderer) override;
    void Update(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
private:
    Sprite* m_buttonFrame; // Changes made by Karl
    AnimatedSprite* m_buttonSpriteNormal;   // Sprite for normal state
    AnimatedSprite* m_buttonSpriteHovered;  // Sprite for hovered state
};

#endif // !__CHARACTERTHREE_H_