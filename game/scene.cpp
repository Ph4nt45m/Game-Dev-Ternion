// This include: 
#include "scene.h"

// Local includes: 
#include "renderer.h"

// Library includes: 
#include <cassert>

const int Scene::sm_iSegmentWidth = 402;

Scene::Scene()
{

}
Scene::~Scene()
{

}

bool
Scene::Initialise(Renderer& renderer)
{
	return true;
}
