#include "MyContactListener.h"
#include <stdio.h>
#include "golem.h"
#include "character.h"  
#include "terrain.h"    

void MyContactListener::BeginContact(b2Contact* contact)
{
    printf("Started contact\n");

    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Retrieve user data
    void* userDataA = bodyA->GetUserData();
    void* userDataB = bodyB->GetUserData();

    if (!userDataA || !userDataB) {
        return;  // If user data is missing, just return early
    }

    // Check for terrain collisions
    if ((int)userDataA == PLAYER && (int)userDataB == TERRAIN) {
        // Player collided with terrain
        printf("Player collided with terrain!\n");
    }
    else if ((int)userDataA == TERRAIN && (int)userDataB == PLAYER) {
        // Reverse case: terrain collided with player
        printf("Player collided with terrain (reverse case)!\n");
    }

    // Golem's slash attack collision with player
    if ((int)userDataA == PLAYER && (int)userDataB == GOLEM_SLASH) {
        printf("Player hit by golem's slash!\n");
    }
    else if ((int)userDataA == GOLEM_SLASH && (int)userDataB == PLAYER) {
        printf("Player hit by golem's slash (reverse case)!\n");
    }

    // Handle other collisions, like GOLEM_SLAM, PROJECTILE, etc.
}

void MyContactListener::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Retrieve user data
    void* userDataA = bodyA->GetUserData();
    void* userDataB = bodyB->GetUserData();

    if (userDataA && userDataB) {
        // Check for end of player and terrain collision
        if ((int)userDataA == PLAYER && (int)userDataB == TERRAIN) {
            printf("Player no longer colliding with terrain!\n");
        }
        else if ((int)userDataA == TERRAIN && (int)userDataB == PLAYER) {
            printf("Player no longer colliding with terrain (reverse case)!\n");
        }

        // Handle golem's slash ending
        if ((int)userDataA == PLAYER && (int)userDataB == GOLEM_SLASH) {
            printf("Player no longer hit by golem's slash!\n");
        }
        else if ((int)userDataA == GOLEM_SLASH && (int)userDataB == PLAYER) {
            printf("Player no longer hit by golem's slash (reverse case)!\n");
        }
    }
}
