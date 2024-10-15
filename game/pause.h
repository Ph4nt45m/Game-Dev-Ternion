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
class SplashScene : public Scene
{
    // Member methods:
public:
    SplashScene();
    virtual ~SplashScene();

    virtual bool Initialise(Renderer& renderer) override;
    virtual void Process(float deltaTime, InputSystem& inputSystem) override;
    virtual void Draw(Renderer& renderer) override;

    virtual void DebugDraw() override;

private:
    SplashScene(const SplashScene& other) = delete;
    SplashScene& operator=(const SplashScene& other) = delete;

    // Member data:
protected:
    Sprite* m_splashScene;  // Pointer to the splash image sprite
    float m_fElapsedTime;     // To track how long the splash screen has been displayed

private:
};
#endif