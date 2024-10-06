#include "Terrain.h"
#include "MyContactListener.h"
#include "renderer.h"

Terrain::Terrain(b2World* world, float x, float y, float width, float height)
    : m_type(GROUND)
    , m_sprite(nullptr)
{
   
    // Define the body as static (doesn't move)
    b2BodyDef bodyDef;
    bodyDef.position.Set(x, y);
    bodyDef.type = b2_staticBody;

    // Create the Box2D body
    m_pBody = world->CreateBody(&bodyDef);

    // Define the shape (box) of the terrain
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width/2, height/2);

    // Create a fixture for the body (attaches the shape)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.friction = 0.6f;  
    m_pBody->CreateFixture(&fixtureDef);

    // set user data to recognize the terrain
    m_pBody->SetUserData((void*)TERRAIN);


}

Terrain::~Terrain() {
    if (m_pBody && m_pBody->GetWorld()) {
        m_pBody->GetWorld()->DestroyBody(m_pBody);
    }
}

void Terrain::Draw(Renderer& renderer)
{
    // Retrieve the Box2D body's position in meters
    b2Vec2 position = m_pBody->GetPosition();

    // Convert the position from meters to pixels
    const float SCALE = 30.0f;  // Conversion factor (meters to pixels)
    int xPos = (int)(position.x * SCALE);
    int yPos = (int)(position.y * SCALE);

    // Set the sprite's position based on the Box2D body
    m_sprite->SetX(xPos);
    m_sprite->SetY(yPos);

    // Draw the terrain sprite
    m_sprite->Draw(renderer, false, false);
}

void Terrain::SetSprite(Renderer& renderer, TerrainType m_type, float width, float height)
{
    switch (m_type) {
    case TerrainType::GROUND:
        m_sprite = renderer.CreateSprite("Sprites\\terrainbase3D\\platform.png");
        break;
    case TerrainType::PLATFORM:
        m_sprite = renderer.CreateSprite("Sprites\\terrainbase3D\\platform.png");
        break;
    case TerrainType::LEFT_WALL:
        m_sprite = renderer.CreateSprite("Sprites\\terrainbase3D\\leftwall.png");
        break;
    case TerrainType::RIGHT_WALL:
        m_sprite = renderer.CreateSprite("Sprites\\terrainbase3D\\rightwall.png");
        break;
    }
    m_sprite->SetWidth((int)width);
    m_sprite->SetHeight((int)height);
}
