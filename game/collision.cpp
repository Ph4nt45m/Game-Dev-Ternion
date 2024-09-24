#include "Collision.h"

// Implementation of the collision check for sprites
bool Collision::checkCollision(Character& character, const Sprite& attackSprite) {
    int characterX = character.GetPosition().x;
    int characterY = character.GetPosition().y;
    int characterWidth = character.GetBodyWidth();
    int characterHeight = character.GetBodyHeight();

    return (characterX < attackSprite.GetX() + attackSprite.GetWidth() &&
        characterX + characterWidth > attackSprite.GetX() &&
        characterY < attackSprite.GetY() + attackSprite.GetHeight() &&
        characterY + characterHeight > attackSprite.GetY());

}

//Collisions for animated sprites
bool Collision::checkPathCollision(Character& character, AnimatedSprite& attackSprite, float startX, float endX, float step, int direction) {
    for (float currentX = startX; (direction > 0 ? currentX < endX : currentX > endX); currentX += (step * direction)) {
        attackSprite.SetX((int)currentX);
        if (checkCollision(character, attackSprite)) {
            return true;  // Collision detected
        }
    }
    return false;  
}


