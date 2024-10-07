#pragma once
#ifndef MENUSCENE_H   
#define MENUSCENE_H 

// Local includes: 
#include "scene.h"
#include "inputsystem.h"
#include "sceneManager.h"
#include "button.h"
#include "game.h"

//Libs
#include <string>

// Forward declarations: Always included.
class Renderer;
class InputSystem;
class Forest;
class Character;
class Golem;

class MenuScene : public Scene
{
    // Member methods:
public:
    MenuScene();
    virtual ~MenuScene();

    virtual bool Initialise(Renderer& renderer) override;
    virtual void Process(float deltaTime, InputSystem& inputSystem) override;
    virtual void Draw(Renderer& renderer) override;

    virtual void DebugDraw() override;

private:
    MenuScene(const MenuScene& other) = delete;
    MenuScene& operator=(const MenuScene& other) = delete;

    // Member data:
protected:
    Sprite* m_BackGroundScene;  // Pointer to the background image sprite

    Button* m_pStartButton;  // Pointer to the start button
    //will come a settings button
    Button* m_pExitButton;   // Pointer to the exit button

    float m_fElapsedTime;     // To track how long the splash screen has been displayed

private:
};
#endif