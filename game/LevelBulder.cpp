#include "LevelBuilder.h"
#include <cstdlib>  
#include <ctime>    

LevelBuilder::LevelBuilder(b2World* world, float levelWidth, float levelHeight, int platformCount)
    : m_pWorld(world), m_levelWidth(levelWidth), m_levelHeight(levelHeight), m_platformCount(platformCount)
{
    std::srand(static_cast<unsigned>(std::time(0)));
}

LevelBuilder::~LevelBuilder()
{
    for (Terrain* platform : m_platforms)
    {
        delete platform;
        platform = nullptr;
    }
}

void LevelBuilder::BuildLevel(Renderer& renderer) {
    // Generate random platforms
    for (int i = 0; i < m_platformCount; ++i) {
   
        float width = 200.0f;  // Width between 100-150
        float height = 20.0f;  // Fixed height

        float x = static_cast<float>(std::rand() % static_cast<int>(m_levelWidth)) + 100;
        float y = static_cast<float>(std::rand() % static_cast<int>(m_levelHeight - 10) + 5);  // Only in upper half

        GeneratePlatform(renderer, x, y, width, height);
    }
}

void LevelBuilder::GeneratePlatform(Renderer& renderer, float x, float y, float width, float height) {
    Terrain* platform = new Terrain(m_pWorld, x/SCALE, y, width/SCALE, height*4/SCALE, PLATFORM);
    m_platforms.push_back(platform);
    platform->Initialise(renderer);
}

const std::vector<Terrain*>& LevelBuilder::GetPlatforms() const
{
    return m_platforms;
}

void LevelBuilder::Draw(Renderer& renderer, Camera& camera)
{
    for (Terrain* platform : m_platforms) {
        platform->Draw(renderer, camera);  
    }
}