#ifndef LEVELBUILDER_H
#define LEVELBUILDER_H

#include <vector>
#include "Terrain.h"
#include "renderer.h"
#include "Camera.h"
#include <Box2D.h>

class LevelBuilder {
public:
    LevelBuilder(b2World* world, float levelWidth, float levelHeight, int platformCount);

    void BuildLevel(Renderer& renderer);  // Main function to build the level.
    const std::vector<Terrain*>& GetPlatforms() const; 
    void Draw(Renderer& renderer, Camera& camera);

private:
    void GeneratePlatform(Renderer& renderer, float x, float y, float width, float height);  // Helper to create platforms.

    b2World* m_pWorld;
    float m_levelWidth;
    float m_levelHeight;
    int m_platformCount;
    std::vector<Terrain*> m_platforms;  // Stores all generated platforms.
};

#endif // LEVELBUILDER_H
