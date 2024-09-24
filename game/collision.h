#ifndef COLLISION_H
#define COLLISION_H

#include "Sprite.h" 
#include "character.h"
#include "animatedsprite.h"

class Collision {
public:
    // Check collision between two sprites (player and attack)
    bool checkCollision(Character& character, const Sprite& attackSprite);


    // Check collision between two animated sprites
    bool checkPathCollision(Character& character, AnimatedSprite& attackSprite, float startX, float endX, float step, int direction);

};

#endif // COLLISION_H
