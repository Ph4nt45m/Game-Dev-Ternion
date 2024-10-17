#include "LevelBuilder.h"
#include <cstdlib>  
#include <ctime>    

LevelBuilder::LevelBuilder(b2World* world, float levelWidth, float levelHeight, int platformCount)
    : m_pWorld(world), m_levelWidth(levelWidth), m_levelHeight(levelHeight), m_platformCount(platformCount) {
    std::srand(static_cast<unsigned>(std::time(0)));
}

void LevelBuilder::BuildLevel() {
    // Generate random platforms
    for (int i = 0; i < m_platformCount; ++i) {
        float x = static_cast<float>(std::rand() % static_cast<int>(m_levelWidth));
        float y = static_cast<float>(std::rand() % static_cast<int>(m_levelHeight / 2));  // Only in upper half

        float width = 100.0f + static_cast<float>(std::rand() % 50);  // Width between 100-150
        float height = 20.0f;  // Fixed height

        GeneratePlatform(x, y, width, height);
    }
}

void LevelBuilder::GeneratePlatform(float x, float y, float width, float height) {
    Terrain* platform = new Terrain(m_pWorld, x, y, width, height, PLATFORM);
    m_platforms.push_back(platform);
}
