#pragma once
#ifndef __CHARACTERTWO_H_
#define __CHARACTERTWO_H_

#include "button.h"
#include "sceneManager.h"
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
    AnimatedSprite* m_buttonSpriteNormal;   // Sprite for normal state
    AnimatedSprite* m_buttonSpriteHovered;  // Sprite for hovered state
};

#endif // !__CHARACTERTWO_H_