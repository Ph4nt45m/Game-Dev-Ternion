#pragma once
#ifndef DEATHSCENE_H   
#define DEATHSCENE_H 

// Local includes: 
#include "sceneManager.h"
#include "button.h"

//Libs
#include <string>

// Forward declarations: Always included.
class Scene;
class Renderer;
class InputSystem;
class Game;

class DeathScene : public Scene
{
    // Member methods:
public:
    DeathScene();
    virtual ~DeathScene();

    virtual bool Initialise(Renderer& renderer) override;
    virtual void Process(float deltaTime, InputSystem& inputSystem) override;
    virtual void Draw(Renderer& renderer) override;

    virtual void DebugDraw() override;

private:
    DeathScene(const DeathScene& other) = delete;
    DeathScene& operator=(const DeathScene& other) = delete;

    // Member data:
protected:
    Sprite* m_splashScene;  // Pointer to the splash image sprite
    float m_fElapsedTime;     // To track how long the splash screen has been displayed

    Button* m_MenuButton;
    Button* m_RestartButton;
    Button* m_RollCredits;
    Button* m_ExitButton;
private:
};
#endif