#ifndef MYCONTACTLISTENER_H
#define MYCONTACTLISTENER_H

#include <Box2D.h>

// Define identifiers for game objects
enum GameObjectType {
    PLAYER = 1,
    GOLEM = 2,
    GOLEM_SLASH = 3,
    GOLEM_SLAM = 4,
    GOLEM_PROJECTILE = 5,
    TERRAIN = 6,
    GROUND_COLLISION = 7,
    ENEMY_PROJECTILE = 8,
    PLAYER_SP_ATTACK = 9, 
    PLAYER_PROJECTILE = 10,
    MUSHROOM = 11
};

// MyContactListener class inheriting from b2ContactListener
class MyContactListener : public b2ContactListener {
public:
   
    // Override the BeginContact method from b2ContactListener
    void BeginContact(b2Contact* contact) override;

    // You can add other Box2D event methods here if needed
     void EndContact(b2Contact* contact) override;
    // void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
    // void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};

#endif // MYCONTACTLISTENER_H
