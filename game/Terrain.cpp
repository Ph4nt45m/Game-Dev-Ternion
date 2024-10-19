#include "Terrain.h"
#include "MyContactListener.h"
#include "renderer.h"

Terrain::Terrain(b2World* world, float x, float y,float width, float height, TerrainType type)
    : m_type(GROUND)
    , m_sprite(nullptr)
    , m_pWorld(world)
    , xPos(x)
    , yPos(y)
    , m_fwidth(width)
    , m_fheight(height)
    , m_pBody(nullptr)
{
}

Terrain::~Terrain() {
    if (m_pBody && m_pBody->GetWorld()) {
        m_pBody->GetWorld()->DestroyBody(m_pBody);
    }
}

bool Terrain::Initialise(Renderer& renderer)
{

    SetSprite(renderer, m_type);

    // Define the body as static (doesn't move)
    b2BodyDef bodyDef;
    bodyDef.position.Set(xPos, yPos);
    bodyDef.type = b2_staticBody;

    // Create the Box2D body
    m_pBody = m_pWorld->CreateBody(&bodyDef);

    // Define the shape (box) of the terrain
    b2PolygonShape boxShape;
    boxShape.SetAsBox(m_fwidth, m_fheight);

    // Create a fixture for the body (attaches the shape)
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.friction = 0.6f;
    m_pBody->CreateFixture(&fixtureDef);

    // set user data to recognize the terrain
    userData* terrData = new userData{ TERRAIN, static_cast<void*>(this) };
    m_pBody->SetUserData(static_cast<void*>(terrData));

    m_sprite->SetWidth((int)(m_fwidth * 2 * SCALE));
    m_sprite->SetHeight((int)(m_fheight * 2  * SCALE));

    return true;
}


void Terrain::Draw(Renderer& renderer, Camera& camera)
{
    // Retrieve the Box2D body's position in meters
    b2Vec2 position = m_pBody->GetPosition();

    // Convert the position from meters to pixels
    int xPos = (int)(position.x * SCALE);
    int yPos = (int)(position.y * SCALE +25);

    // Adjust the position by subtracting the camera's offset
    Vector2* cameraOffset = camera.GetOffset(); // Assuming GetOffset returns a Vector2 with x and y offsets
    xPos -= (int)cameraOffset->x;
    yPos -= (int)cameraOffset->y;

    // Set the sprite's position based on the adjusted position
    m_sprite->SetX(xPos);
    m_sprite->SetY(yPos);

    // Draw the terrain sprite
    m_sprite->Draw(renderer, false, true);
}


void Terrain::SetSprite(Renderer& renderer, TerrainType m_type)
{
    switch (m_type) {
    case TerrainType::GROUND:
        m_sprite = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\ground.png");
        break;
    case TerrainType::PLATFORM:
        m_sprite = renderer.CreateSprite("..\\Sprites\\terrainforest2D\\platform.png");
        break;
    }
}
