#ifndef TERRAIN_H
#define TERRAIN_H

#include <Box2D.h>
#include "renderer.h"
#include "sprite.h"
#include "Camera.h"

enum TerrainType {
    GROUND,
    PLATFORM,
    LEFT_WALL,
    RIGHT_WALL
};

class Terrain 
{
public:
    Terrain(b2World* world, float x, float y, float width, float height);
    ~Terrain();

    // Function to render the terrain
    void Draw(Renderer& renderer, Camera& camera); 


    // Getter for position 
    b2Vec2 GetPosition() const { return m_pBody->GetPosition(); }

    void SetSprite(Renderer& renderer, TerrainType m_type, float width, float height);

private:
    b2Body* m_pBody; 
    TerrainType m_type;
    Sprite* m_sprite;  
};

#endif // TERRAIN_H
