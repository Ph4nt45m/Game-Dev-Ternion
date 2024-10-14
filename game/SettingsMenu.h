#pragma once
#ifndef SETTINGSSCENE_H   
#define SETTINGSSCENE_H 

// Local includes: 
#include "scene.h"
#include "inputsystem.h"
#include "sceneManager.h"
#include "button.h"
#include "alphabet.h"

//Libs
#include <string>

// Forward declarations: Always included.
class Game;
class Renderer;
class InputSystem;
class Forest;
class Character;
class Golem;

class SettingScene : public Scene
{
    // Member methods:
public:
    SettingScene();
    virtual ~SettingScene();

    virtual bool Initialise(Renderer& renderer) override;
    virtual void Process(float deltaTime, InputSystem& inputSystem) override;
    virtual void Draw(Renderer& renderer) override;

    virtual void DebugDraw() override;

private:
    SettingScene(const SettingScene& other) = delete;
    SettingScene& operator=(const SettingScene& other) = delete;

    // Member data:
protected:
    Sprite* m_BackGroundScene;  // Pointer to the background image sprite

    Button* MainMenu;
    Button* increase;
    Button* decrease;
    Button* increaseSound;
    Button* decreaseSound;

    Alphabet* alphabet;

    float m_fElapsedTime;     // To track how long the splash screen has been displayed

private:
};
#endif