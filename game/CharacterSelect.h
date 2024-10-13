#ifndef CHARACTER_SELECT_SCENE_H   
#define CHARACTER_SELECT_SCENE_H 

// Local includes: 
#include "scene.h"
#include "inputsystem.h"
#include "sceneManager.h"
#include "button.h"
#include "game.h"
#include <string>

// Forward declarations: Always included.
class Renderer;
class InputSystem;
class Character;
class CharacterSelectScene : public Scene
{
    // Member methods:
public:
    CharacterSelectScene();
    virtual ~CharacterSelectScene();

    virtual bool Initialise(Renderer& renderer) override;
    virtual void Process(float deltaTime, InputSystem& inputSystem) override;
    virtual void Draw(Renderer& renderer) override;

    virtual void DebugDraw() override;

private:
    CharacterSelectScene(const CharacterSelectScene& other) = delete;
    CharacterSelectScene& operator=(const CharacterSelectScene& other) = delete;

    // Member data:
protected:
    Sprite* m_ChrSelectScene;  // Pointer to the splash image sprite
    Button* characterOne;
    Button* characterTwo;
    Button* characterThree;
    Button* MainMenu;

    float m_fElapsedTime;     // To track how long the splash screen has been displayed

private:
};
#endif