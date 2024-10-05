#ifndef CAMERA_H
#define CAMERA_H

#include "Character.h"  // Assuming you have a Player class
#include "vector2.h"

class Camera {
public:
    Camera();

   void SetCamera(int screenWidth, int screenHeight, int worldWidth, int worldHeight);

    void Update(Character& player);
    Vector2* GetOffset();

private:
    Vector2 m_position;  // The camera's position in the world
    int m_screenWidth;
    int m_screenHeight;
    int m_worldWidth;
    int m_worldHeight;
};

#endif // CAMERA_H
