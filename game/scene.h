#pragma once

#ifndef   __SCENE_H_
#define   __SCENE_H_

// Forward declarations:
class Renderer;
class InputSystem;
class Character;
class Vector2;

// Class declaration: 
class Scene
{
	// Member methods: 
public:
	Scene();
	virtual ~Scene();
	virtual bool Initialise(Renderer& renderer) = 0;
	virtual void Process(float deltaTime, InputSystem& inputSystem) = 0;
	virtual void Draw(Renderer& renderer) = 0;
	
	virtual void DebugDraw() = 0;

	virtual void SetCharPos(Vector2& characterPos) = 0;
	virtual void SetCharFeetPos(Vector2& feetPos) = 0;
	virtual void SetCharWidth(int width) = 0;
	virtual bool SetEnemies(Character& character, Renderer& renderer) = 0;

protected:

private:
	Scene(const Scene& scene);
	Scene& operator=(const Scene& scene);

	// Member data: 
public:

protected:
	int m_iSurge;
	int m_iShowCount;
	float m_fVelocity;

	Character* m_pEntCharacter;
	Vector2* m_vCharPos;
	Vector2* m_vFeetPos;
	int m_iCharWidth;

	static const int sm_iSegmentWidth;

};

#endif // !__SCENE_
