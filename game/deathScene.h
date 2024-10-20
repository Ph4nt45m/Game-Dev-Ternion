#pragma once
#ifndef DEATHSCENE_H   
#define DEATHSCENE_H 

// Local includes: 
#include "sceneManager.h"
#include "button.h"
#include <Box2D.h>
#include "Camera.h"
#include "alphabet.h"

//Libs
#include <string>

// Forward declarations: Always included.
class Scene;
class Renderer;
class InputSystem;
class Game;
class Player;

class DeathScene : public Scene
{
    // Member methods:
public:
    DeathScene(b2World* world, Player* character);
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
    b2World* m_pWorld;
    Player* m_pCharacter;
    Camera m_camera;

    Alphabet* alphabet;

    Sprite* m_splashScene;  // Pointer to the splash image sprite
    float m_fElapsedTime;     // To track how long the splash screen has been displayed

    Button* m_MenuButton;
    Button* m_RestartButton;
    Button* m_RollCredits;
    Button* m_ExitButton;
private:
};
#endif