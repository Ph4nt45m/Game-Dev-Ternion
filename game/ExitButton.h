#pragma once
#ifndef __EXITBUTTON_H_
#define __EXITBUTTON_H_

#include "button.h"
#include "sceneManager.h"
#include "sprite.h" // Assuming you'll use a sprite for the button

class ExitButton : public Button
{
public:
    ExitButton(float x, float y);
    virtual ~ExitButton();

    // Override the virtual functions from Button
    bool Initialise(Renderer& renderer) override;
    void Update(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;
private:
    Sprite* m_buttonSpriteNormal;   // Sprite for normal state
    Sprite* m_buttonSpriteHovered;  // Sprite for hovered state
    Sprite* m_buttonSpritePressed;  // Sprite for pressed state
};

#endif // !__EXITBUTTON_H_