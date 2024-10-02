#include "MyContactListener.h"
#include <stdio.h>
#include "golem.h"

void MyContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Retrieve user data
    void* userDataA = bodyA->GetUserData();
    void* userDataB = bodyB->GetUserData();

    if (userDataA == (void*)PLAYER && userDataB == (void*)GOLEM_SLASH) {

        // Player hit by golem's slash
        printf("Player hit by golem's slash!\n");
        // Apply damage or other effects here
    }
    else if (userDataA == (void*)GOLEM_SLASH && userDataB == (void*)PLAYER) {
        // Reverse case
        printf("Player hit by golem's slash!\n");
    }

    /* if (userDataA == (void*)PLAYER && userDataB == (void*)GOLEM_SLAM) {
         printf("Player hit by golem's slam!\n");
     }
     else if (userDataA == (void*)GOLEM_SLAM && userDataB == (void*)PLAYER) {
         printf("Player hit by golem's slam!\n");
     }

     if (userDataA == (void*)PLAYER && userDataB == (void*)GOLEM_PROJECTILE) {
         printf("Player hit by golem's projectile!\n");
     }
     else if (userDataA == (void*)GOLEM_PROJECTILE && userDataB == (void*)PLAYER) {
         printf("Player hit by golem's projectile!\n");
     }*/
}

// EndContact is triggered when the contact between two fixtures ends
void MyContactListener::EndContact(b2Contact* contact) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Retrieve user data
    void* userDataA = bodyA->GetUserData();
    void* userDataB = bodyB->GetUserData();

    // Check for the end of collisions (e.g., stop applying damage)
    if (userDataA == (void*)PLAYER && userDataB == (void*)GOLEM_SLASH) {
        // Player no longer hit by golem's slash
        printf("Player no longer hit by golem's slash!\n");
    }
    else if (userDataA == (void*)GOLEM_SLASH && userDataB == (void*)PLAYER) {
        // Reverse case
        printf("Player no longer hit by golem's slash!\n");
    }

    // You can add similar checks for other game objects
    /* if (userDataA == (void*)PLAYER && userDataB == (void*)GOLEM_PROJECTILE) {
         printf("Player no longer hit by golem's projectile!\n");
     }
     else if (userDataA == (void*)GOLEM_PROJECTILE && userDataB == (void*)PLAYER) {
         printf("Player no longer hit by golem's projectile!\n");
     } */
}