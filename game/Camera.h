#ifndef CAMERA_H
#define CAMERA_H

#include "vector2.h"
#include <Box2D.h>


class Character;

class Camera {
public:
    Camera();

   void SetCamera(int screenWidth, int screenHeight, int worldWidth, int worldHeight);

    void Update(Character& player);
    Vector2* GetOffset();
public:
    Vector2 m_position;  // The camera's position in the world

private:
    int m_screenWidth;
    int m_screenHeight;
    int m_worldWidth;
    int m_worldHeight;
};

#endif // CAMERA_H
