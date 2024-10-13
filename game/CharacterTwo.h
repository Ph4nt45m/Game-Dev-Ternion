#pragma once
#ifndef __CHARACTERTWO_H_
#define __CHARACTERTWO_H_

#include "button.h"
#include "sceneManager.h"
#include "sprite.h" // Changes made by Karl
#include "animatedsprite.h"

class CharacterTwo : public Button
{
public:
    CharacterTwo(float x, float y);
    virtual ~CharacterTwo();

    // Override the virtual functions from Button
    bool Initialise(Renderer& renderer) override;
    void Update(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
private:
    Sprite* m_buttonFrameNormal; // Changes made by Karl
    Sprite* m_buttonFrameHovered; // Changes made by Karl
    AnimatedSprite* m_buttonSpriteNormal;   // Sprite for normal state
    AnimatedSprite* m_buttonSpriteHovered;  // Sprite for hovered state
    float m_fBlueTint; // Changes made by Karl
    int m_iCharYOffset; // Offset to shift sprite up to center // Changes made by Karl
};

#endif // !__CHARACTERTWO_H_