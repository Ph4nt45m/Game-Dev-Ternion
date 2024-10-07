#include "Camera.h"
#include "character.h"
#include <stdio.h>


Camera::Camera()
    : m_screenWidth(0)
    , m_screenHeight(0)
    , m_worldWidth(0)
    , m_worldHeight(0)
{
    m_position.x = 0;
    m_position.y = 0;
}

// Set up the camera's screen size and world boundaries
void Camera::SetCamera(int screenWidth, int screenHeight, int worldWidth, int worldHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_worldWidth = worldWidth;
    m_worldHeight = worldHeight;

}

void Camera::Update(Character& player)
{
    const float SCALE = 30.0f;  // Scaling factor from Box2D meters to pixels

    // Get the player's Box2D body position in meters, no need to adjust
    b2Vec2 playerPosition = player.GetPosition();

    // Convert Box2D meters to pixel space using the scale factor
    float playerXInPixels = playerPosition.x * SCALE;

    // Center the camera on the player's position
    m_position.x = playerXInPixels - (m_screenWidth / 2);

    // Clamp the camera within the world boundaries (in pixels)
    if (m_position.x < 0) {
        m_position.x = 0;
        printf("Begin\n");
    }
    if (m_position.y < 0) {
        m_position.y = 0;
    }
    if (m_position.x > m_worldWidth) {
        m_position.x = m_worldWidth;
        printf("End\n");
    }
    if (m_position.y > m_worldHeight) {
        m_position.y = m_worldHeight;
    }
}


// Get the camera offset (used for adjusting the rendering positions)
Vector2* Camera::GetOffset() {
    return &m_position;
}
