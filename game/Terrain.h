#ifndef TERRAIN_H
#define TERRAIN_H

#include <Box2D.h>
#include "renderer.h"
#include "sprite.h"
#include "Camera.h"

//scale

#define SCALE 100.0f

enum TerrainType {
    GROUND,
    PLATFORM,
    LEFT_WALL,
    RIGHT_WALL
};

class Terrain 
{
public:
    Terrain(b2World* world, float x, float y, float width, float height, TerrainType type);
    ~Terrain();


    // Function to render the terrain
    void Draw(Renderer& renderer, Camera& camera); 

    bool Initialise(Renderer& renderer);

    // Getter for position 
    b2Vec2 GetPosition() const { return m_pBody->GetPosition(); }

    void SetSprite(Renderer& renderer, TerrainType m_type);
    
    float GetWidth() { return m_fwidth; }
    float GetHeight() { return m_fheight; }

private:
    b2Body* m_pBody; 
    b2World* m_pWorld; 
    TerrainType m_type;
    Sprite* m_sprite; 
    float xPos;
    float yPos;
    float m_fwidth;
    float m_fheight;
};

#endif // TERRAIN_H
