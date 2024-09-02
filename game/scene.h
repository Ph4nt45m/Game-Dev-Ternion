#pragma once

#ifndef   __SCENE_H_
#define   __SCENE_H_

// Forward declarations:
class Renderer;
class InputSystem;

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

protected:

private:
	Scene(const Scene& scene);
	Scene& operator=(const Scene& scene);

	// Member data: 
public:

protected:

};

#endif // !__SCENE_
