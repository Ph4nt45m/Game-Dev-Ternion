#pragma once
#ifndef CREDITSCENE_H   
#define CREDITSCENE_H 

// Local includes: 
#include "sceneManager.h"
#include "alphabet.h"
//Libs
#include <string>

// Forward declarations: Always included.
class Scene;
class Renderer;
class InputSystem;
class Game;

class CreditScene : public Scene
{
    // Member methods:
public:
    CreditScene();
    virtual ~CreditScene();

    virtual bool Initialise(Renderer& renderer) override;
    virtual void Process(float deltaTime, InputSystem& inputSystem) override;
    virtual void Draw(Renderer& renderer) override;

    virtual void DebugDraw() override;

private:
    CreditScene(const CreditScene& other) = delete;
    CreditScene& operator=(const CreditScene& other) = delete;

    // Member data:
protected:
    Sprite* m_splashScene;  // Pointer to the splash image sprite
    Alphabet* alphabet;
    float m_fElapsedTime;     // To track how long the splash screen has been displayed
    float m_climbTime;
    float m_fmoveUp;

private:
};
#endif