#pragma once
#ifndef __INCREASESOUNDEFFBUTTON_H_
#define __INCREASESOUNDEFFBUTTON_H_

#include "button.h"
#include "sceneManager.h"
#include "sprite.h" // Assuming you'll use a sprite for the button

class IncreaseSoundButton : public Button
{
public:
    IncreaseSoundButton(float x, float y);
    virtual ~IncreaseSoundButton();

    // Override the virtual functions from Button
    bool Initialise(Renderer& renderer) override;
    void Update(float deltaTime, InputSystem& inputSystem) override;
    void Draw(Renderer& renderer) override;

    void soundVolume();
private:
    Sprite* m_buttonSpriteNormal;   // Sprite for normal state
};

#endif // !__INCREASESOUNDEFFBUTTON_H_