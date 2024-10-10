#pragma once
#ifndef __CHARACTERONE_H_
#define __CHARACTERONE_H_

#include "button.h"
#include "sceneManager.h"
#include "animatedsprite.h"

class CharacterOne : public Button
{
public:
    CharacterOne(float x, float y);
    virtual ~CharacterOne();

    // Override the virtual functions from Button
    bool Initialise(Renderer& renderer) override;
    void Update(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
private:
    AnimatedSprite* m_buttonSpriteNormal;   // Sprite for normal state
    AnimatedSprite* m_buttonSpriteHovered;  // Sprite for hovered state
};

#endif // !__CHARACTERONE_H_