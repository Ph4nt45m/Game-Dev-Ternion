#pragma once
#ifndef __DECREASEBUTTON_H_
#define __DECREASEBUTTON_H_

#include "button.h"
#include "sceneManager.h"
#include "sprite.h" // Assuming you'll use a sprite for the button

class DecreaseButton : public Button
{
public:
    DecreaseButton(float x, float y);
    virtual ~DecreaseButton();

    // Override the virtual functions from Button
    bool Initialise(Renderer& renderer) override;
    void Update(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;

    void soundVolume();
private:
    Sprite* m_buttonSpriteNormal;   // Sprite for normal state
};

#endif // !__DECREASEBUTTON_H_