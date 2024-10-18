#include "MyContactListener.h"
#include <stdio.h>
#include "player.h"
#include "game.h"

//Enemies
#include "golem.h"
#include "mushroom.h"

#include "character.h"  
#include "terrain.h"

void MyContactListener::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    if (!fixtureA || !fixtureB) {
        printf("No fixture\n");
        return;
    }

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();
    
    if (!bodyA || !bodyB) {
        printf("No body\n");
        return;
    }

    // Retrieve user data
    userData* userDataA = static_cast<userData*>(fixtureA->GetBody()->GetUserData());
    userData* userDataB = static_cast<userData*>(fixtureB->GetBody()->GetUserData());

    if (!userDataA || !userDataB) {
        printf("No userData\n");
        return; 
    }

    bool fixtureAIsSensor = fixtureA->IsSensor();
    bool fixtureBIsSensor = fixtureB->IsSensor();

    if (!fixtureAIsSensor && !fixtureBIsSensor)
    {
        printf("No sensor %d, %d\n", fixtureAIsSensor, fixtureBIsSensor);
        return;
    }

    printf("Check contact\n");

    //Player Attacking enemies.
    if (fixtureBIsSensor && userDataA->type == GOLEM && userDataB->type == PLAYER_SP_ATTACK)
    {
        printf("Golem hit with special\n");
        static_cast<Golem*>(userDataA->object)->getEnemyHealth()->Damage(20.0f);
    }
    else if (fixtureAIsSensor && userDataA->type == PLAYER_SP_ATTACK && userDataB->type == GOLEM)
    {
        printf("Golem hit with special (reverse)\n");
        static_cast<Golem*>(userDataB->object)->getEnemyHealth()->Damage(20.0f);
    }
    if (fixtureBIsSensor && userDataA->type == MUSHROOM && userDataB->type == PLAYER_SP_ATTACK)
    {
        printf("Golem hit with special\n");
        static_cast<Mushroom*>(userDataA->object)->getEnemyHealth()->Damage(20.0f);
    }
    else if (fixtureAIsSensor && userDataA->type == PLAYER_SP_ATTACK && userDataB->type == MUSHROOM)
    {
        printf("Golem hit with special (reverse)\n");
        static_cast<Mushroom*>(userDataB->object)->getEnemyHealth()->Damage(20.0f);
    }




    if (fixtureAIsSensor && userDataA->type == GOLEM_SLASH && userDataB->type == PLAYER)
    {
        //Game::GetInstance().GetCharacter()->IsGodmode(); // Changes made by Karl - Check for godmode
        printf("Player hit with slash\n");
        //Changes made by Kyle
        //Just to test
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(10);
        //Changes ended
    }
    else if (fixtureBIsSensor && userDataA->type == PLAYER && userDataB->type == GOLEM_SLASH)
    {
        printf("Player hit with slash (reverse)\n");
        //Changes made by Kyle
//Just to test
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(10);
        //Changes ended

    }

    if (fixtureAIsSensor && userDataA->type == GOLEM_SLAM && userDataB->type == PLAYER)
    {
        printf("Player hit with slam\n");
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(10);
    }
    else if (fixtureBIsSensor && userDataA->type == PLAYER && userDataB->type == GOLEM_SLAM)
    {
        printf("Player hit with slam (reverse)\n");
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(10);
    }

    //mushroom Body
    if (fixtureAIsSensor && userDataA->type == MUSHROOM && userDataB->type == PLAYER)
    {
        printf("Player hit with mushroom\n");
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(5);
    }
    else if (fixtureBIsSensor && userDataA->type == PLAYER && userDataB->type == MUSHROOM)
    {
        printf("Player hit with mushroom (reverse)\n");
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(5);
    }
    
    if (userDataA->type == MUSHROOM && userDataB->type == PLAYER)
    {
        printf("Player hit with mushroom\n");
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(5);
    }
    else if (userDataA->type == PLAYER && userDataB->type == MUSHROOM)
    {
        printf("Player hit with mushroom (reverse)\n");
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(5);
    }

    // Check for terrain collisions
    if (userDataA->type == PLAYER && userDataB->type == TERRAIN) {
        // Player collided with terrain
        //printf("Player collided with terrain!\n");
    }
    else if (userDataA->type == TERRAIN && userDataB->type == PLAYER) {
        // Reverse case: terrain collided with player
        //printf("Player collided with terrain (reverse case)!\n");
        
    }
    if (userDataA->type == PLAYER && userDataB->type == GOLEM) {
        //if we want contact damage
        printf("Touch\n");
    }
    else if (userDataA->type == GOLEM && userDataB->type == PLAYER) {
        //if we want contact damage
        printf("Touch\n");
    }
    // Check for terrain collisions
    if (userDataA->type == GOLEM && userDataB->type == TERRAIN) {
        // Player collided with terrain
    }
    else if (userDataA->type == TERRAIN && userDataB->type == GOLEM) {
        // Reverse case: terrain collided with player
    }
}

void MyContactListener::EndContact(b2Contact* contact)
{
    
   
}
