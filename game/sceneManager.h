#pragma once

#ifndef   __SCENEMANAGER_H_
#define   __SCENEMANAGER_H_

// Forward declarations:
#include "sprite.h"
#include "scene.h"
#include <string>
#include <vector>

// Class declaration: 
class Renderer;
class scene;
class SceneManager
{
	// Member methods: 
public:
	static SceneManager& GetInstance();
	static void DestroyInstance();
	void LoadImage(Renderer& renderer, Sprite*& m_sceneBackground, std::string filePath);
	bool Initialise(Renderer& renderer);
	void Process(float deltaTime, InputSystem& inputSystem);
	void Draw(Renderer& renderer);

	void ChangeScene(int scene);
	void PerformSceneTransition(); // Actually performs the scene change

protected:
	SceneManager();
	virtual ~SceneManager();

	std::vector<Scene*> m_scenes;
	int m_iCurrentScene;
	Renderer* m_pRenderer;

private:
	// Member data: 
	// Disable copying to ensure only one instance exists.
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator=(const SceneManager& other) = delete;

	Scene* CreateSceneByID(int sceneID);
	bool m_isTransitionPending = false;
	int m_pendingSceneID = -1; // ID of the scene to switch to
public:

protected:
	static SceneManager* sm_pInstance;
	float m_fElapsedTime; // Changes made by Karl
	int sceneId;
	float m_iRed;
	float m_iGreen;
	float m_iBlue;

private:
	std::vector<Sprite*> m_sprites; // Store allocated sprites
};

#endif // !__SCENEMANAGER_H_
