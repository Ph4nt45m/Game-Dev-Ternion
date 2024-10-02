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
    boxShape.SetAsBox(width, height);

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

void Terrain::Draw(Renderer& renderer) {
   
    // Set the sprite's position and scale based on the Box2D body's position
        // Set up the sprite for rendering
    m_sprite->SetX((int)m_pBody->GetPosition().x);
    m_sprite->SetY((int)m_pBody->GetPosition().y);

    //Draw tarrian
    renderer.DrawSprite(*m_sprite, false, false);

}

void Terrain::SetSprite(Renderer& renderer, TerrainType m_type, float width, float height)
{
    switch (m_type) {
    case TerrainType::GROUND:
        m_sprite = renderer.CreateSprite("Sprites\\terrainbase3D\\boundary.png");
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
    Draw(renderer);
}
