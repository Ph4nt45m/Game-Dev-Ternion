#pragma once
#ifndef __INCREASEBUTTON_H_
#define __INCREASEBUTTON_H_

#include "button.h"
#include "sceneManager.h"
#include "sprite.h" // Assuming you'll use a sprite for the button

class IncreaseButton : public Button
{
public:
    IncreaseButton(float x, float y);
    virtual ~IncreaseButton();

    // Override the virtual functions from Button
    bool Initialise(Renderer& renderer) override;
    void Update(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;

    void soundVolume();
private:
    Sprite* m_buttonSpriteNormal;   // Sprite for normal state
};

#endif // !__INCREASEBUTTON_H_