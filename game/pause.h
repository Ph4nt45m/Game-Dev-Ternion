#pragma once
#ifndef PAUSESCENE_H   
#define PAUSESCENE_H 

// Local includes: 
#include "scene.h"
#include "inputsystem.h"
#include "sprite.h"

#include <string>

// Forward declarations: Always included.
class Renderer;
class InputSystem;
class Game;
class Button;
class PauseScene : public Scene
{
    // Member methods:
public:
    PauseScene();
    virtual ~PauseScene();

    virtual bool Initialise(Renderer& renderer) override;
    virtual void Process(float deltaTime, InputSystem& inputSystem) override;
    virtual void Draw(Renderer& renderer) override;

    virtual void DebugDraw() override;

private:
    PauseScene(const PauseScene& other) = delete;
    PauseScene& operator=(const PauseScene& other) = delete;

    // Member data:
protected:
    Sprite* pauseScreen;  // Pointer to the splash image sprite
    Sprite* transparante;

    Button* m_pExitButton;
    Button* MainMenu;
    Button* resumeButton;
    Button* restartButton;

    float m_fElapsedTime;     // To track how long the splash screen has been displayed

private:
};
#endif