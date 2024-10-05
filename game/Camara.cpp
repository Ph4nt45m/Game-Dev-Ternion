#include "Camera.h"
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

// Update the camera to follow the player
void Camera::Update(Character& player) {
    // Center the camera horizontally on the player
   

    m_position.x = player.GetPosition().x - (m_screenWidth / 2);

    // If you want vertical movement, uncomment the following line:
    //m_position.y = player.GetPosition().y - (m_screenHeight / 2);

    // Clamp the camera position to ensure it doesn't go outside the world boundaries
    if (m_position.x < 0) {
        m_position.x = 0;
    }
    if (m_position.x > m_worldWidth - m_screenWidth) {
        m_position.x = m_worldWidth - m_screenWidth;
    }

    // Uncomment this block if you want to clamp the camera vertically
    /*
    if (m_position.y < 0) {
        m_position.y = 0;
    }
    if (m_position.y > m_worldHeight - m_screenHeight) {
        m_position.y = m_worldHeight - m_screenHeight;
    }
    */
}

// Get the camera offset (used for adjusting the rendering positions)
Vector2* Camera::GetOffset() {
    return &m_position;
}
