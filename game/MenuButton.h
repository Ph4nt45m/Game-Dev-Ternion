#pragma once
#ifndef __MENUBUTTON_H_
#define __MENUBUTTON_H_

#include "button.h"
#include "sceneManager.h"
#include "sprite.h" // Assuming you'll use a sprite for the button

class MenuButton : public Button
{
public:
    MenuButton(float x, float y);
    virtual ~MenuButton();

    // Override the virtual functions from Button
    bool Initialise(Renderer& renderer) override;
    void Update(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
private:
    Sprite* m_buttonSpriteNormal;   // Sprite for normal state
    Sprite* m_buttonSpriteHovered;  // Sprite for hovered state
    Sprite* m_buttonSpritePressed;  // Sprite for pressed state
    Sprite* m_buttonSpriteHighlight;    // Sprite for highlighting in hovered state // Changes made by Karl
    float m_fBlueTint;  // Adjustable value to handle button highlighting // Changes made by Karl
};

#endif // !__STARTBUTTON_H_