#ifndef COLLISION_H
#define COLLISION_H

#include "Sprite.h" 
#include "character.h"
#include "animatedsprite.h"

class Collision {
public:
    bool checkCollision(Character& character, Sprite& attackSprite);

    // Check collision between two animated sprites with frame synchronization
    bool checkCollisionWithFrames(Character& character, AnimatedSprite& attackSprite, int currentFrame, int startFrame, int endFrame);

    // Path-based collision check (using frame synchronization)
    bool checkPathCollision(Character& character, AnimatedSprite& attackSprite, float startX, float endX, float step, int direction, int currentFrame, int startFrame, int endFrame);

    // Arc-based collision check (using frame synchronization)
    bool checkArcCollision(Character& character, AnimatedSprite& attackSprite, float arcStartX, float arcEndX, float step, int direction, int currentFrame, int startFrame, int endFrame);
};

#endif // COLLISION_H
