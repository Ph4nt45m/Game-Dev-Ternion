#include "Collision.h"


//middlewere libs
#include <box2d/box2d.h>

// Simple collision check
bool Collision::checkCollision(Character& character, Sprite& attackSprite) {
    int characterX = character.GetPosition().x;
    int characterY = character.GetPosition().y;
    int characterWidth = character.GetBodyWidth();
    int characterHeight = character.GetBodyHeight();

    return (characterX < attackSprite.GetX() + attackSprite.GetWidth() &&
        characterX + characterWidth > attackSprite.GetX() &&
        characterY < attackSprite.GetY() + attackSprite.GetHeight() &&
        characterY + characterHeight > attackSprite.GetY());
}

// Frame-based collision check
bool Collision::checkCollisionWithFrames(Character& character, AnimatedSprite& attackSprite, int currentFrame, int startFrame, int endFrame) {
    // Only check for collision during the active frames
    if (currentFrame >= startFrame && currentFrame <= endFrame) {
        return checkCollision(character, attackSprite);  // Perform the collision check
    }
    return false; // No collision outside the active frames
}

// Path-based collision check using frame synchronization
bool Collision::checkPathCollision(Character& character, AnimatedSprite& attackSprite, float startX, float endX, float step, int direction, int currentFrame, int startFrame, int endFrame) {
    for (float currentX = startX; (direction > 0 ? currentX < endX : currentX > endX); currentX += (step * direction)) {
        attackSprite.SetX((int)currentX);

        // Check collision only during the specified frame range
        if (checkCollisionWithFrames(character, attackSprite, currentFrame, startFrame, endFrame)) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision
}

// Arc-based collision check using frame synchronization
bool Collision::checkArcCollision(Character& character, AnimatedSprite& attackSprite, float arcStartX, float arcEndX, float step, int direction, int currentFrame, int startFrame, int endFrame) {
    for (float currentX = arcStartX; (direction > 0 ? currentX < arcEndX : currentX > arcEndX); currentX += (step * direction)) {
        attackSprite.SetX((int)currentX);

        // Check collision only during the specified frame range
        if (checkCollisionWithFrames(character, attackSprite, currentFrame, startFrame, endFrame)) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision detected along the arc
}
