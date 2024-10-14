#include "MyContactListener.h"
#include <stdio.h>
#include "golem.h"
#include "character.h"  
#include "terrain.h"    

void MyContactListener::BeginContact(b2Contact* contact)
{
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
    //// Check for terrain collisions
    //if ((int)userDataA == PLAYER && (int)userDataB == GROUND) {
    //    // Player collided with terrain
    //    //printf("Player collided with terrain!\n");
    //    //static_cast<Player*>(userDataB)
    //}
    //else if ((int)userDataA == GROUND && (int)userDataB == PLAYER) {
    //    // Reverse case: terrain collided with player
    //    //printf("Player collided with terrain (reverse case)!\n");

    //}

    // Check for terrain collisions
    if ((int)userDataA == PLAYER && (int)userDataB == TERRAIN) {
        // Player collided with terrain
        //printf("Player collided with terrain!\n");
    }
    else if ((int)userDataA == TERRAIN && (int)userDataB == PLAYER) {
        // Reverse case: terrain collided with player
        //printf("Player collided with terrain (reverse case)!\n");
        
    }
    if ((int)userDataA == PLAYER && (int)userDataB == GOLEM) {
        //if we want contact damage
        //printf("Touch\n"); // Changes made by Karl
    }
    else if ((int)userDataA == GOLEM && (int)userDataB == PLAYER) {
        //if we want contact damage
        //printf("Touch\n"); // Changes made by Karl

    }
    // Check for terrain collisions
    if ((int)userDataA == GOLEM && (int)userDataB == TERRAIN) {
        // Player collided with terrain
    }
    else if ((int)userDataA == TERRAIN && (int)userDataB == GOLEM) {
        // Reverse case: terrain collided with player
    }
    // Golem's slash attack collision with player
    if ((int)userDataA == PLAYER && (int)userDataB == GOLEM_SLASH) {
        //printf("Player hit by golem's slash!\n");
    }
    else if ((int)userDataA == GOLEM_SLASH && (int)userDataB == PLAYER) {
        //("Player hit by golem's slash (reverse case)!\n");
    }
    //Slam
    if ((int)userDataA == PLAYER && (int)userDataB == GOLEM_SLAM) {
        //printf("Player hit by golem's slam!\n");
    }
    else if ((int)userDataA == GOLEM_SLAM && (int)userDataB == PLAYER) {
        //("Player hit by golem's slam (reverse case)!\n");
    }
    // Player's special attack collision with golem - // Changes made by Karl
    if ((int)userDataA == PLAYER_SP_ATTACK && (int)userDataB == GOLEM) {
        printf("Golem hit by player's special!\n");
    }
    else if ((int)userDataA == GOLEM && (int)userDataB == PLAYER_SP_ATTACK) {
        printf("Golem hit by player's special (reverse case)!\n");
    }
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
            //printf("Player no longer colliding with terrain!\n");
        }
        else if ((int)userDataA == TERRAIN && (int)userDataB == PLAYER) {
            //printf("Player no longer colliding with terrain (reverse case)!\n");
        }

        // Handle golem's slash ending
        if ((int)userDataA == PLAYER && (int)userDataB == GOLEM_SLASH) {
         //   printf("Player no longer hit by golem's slash!\n");
        }
        else if ((int)userDataA == GOLEM_SLASH && (int)userDataB == PLAYER) {
            //printf("Player no longer hit by golem's slash (reverse case)!\n");
        }
    }
}
