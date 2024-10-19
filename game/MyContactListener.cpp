#include "MyContactListener.h"
#include <stdio.h>
#include "player.h"
#include "game.h"

//Enemies
#include "golem.h"
#include "mushroom.h"
#include "skeleton.h"
#include "spider.h"

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
        return;
    }

    // Retrieve user data
    userData* userDataA = static_cast<userData*>(fixtureA->GetBody()->GetUserData());
    userData* userDataB = static_cast<userData*>(fixtureB->GetBody()->GetUserData());

    if (!userDataA || !userDataB) {
        return; 
    }

    bool fixtureAIsSensor = fixtureA->IsSensor();
    bool fixtureBIsSensor = fixtureB->IsSensor();

    if (!fixtureAIsSensor && !fixtureBIsSensor)
    {
        return;
    }

    float PlayerDamage;
    //Player Attacking enemies.
    if (fixtureBIsSensor && userDataA->type == GOLEM && userDataB->type == PLAYER_SP_ATTACK)
    {
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 0)
        {
            PlayerDamage = 20.0f;
        }
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 1)
        {
            PlayerDamage = 50.0f;
        }
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 2)
        {
            PlayerDamage = 15.0f;
        }
        static_cast<Golem*>(userDataA->object)->getEnemyHealth()->Damage(PlayerDamage);
    }
    else if (fixtureAIsSensor && userDataA->type == PLAYER_SP_ATTACK && userDataB->type == GOLEM)
    {
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 0)
        {
            PlayerDamage = 20.0f;
        }
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 1)
        {
            PlayerDamage = 50.0f;
        }
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 2)
        {
            PlayerDamage = 15.0f;
        }
        static_cast<Golem*>(userDataB->object)->getEnemyHealth()->Damage(PlayerDamage);
    }
    if (fixtureBIsSensor && userDataA->type == MUSHROOM && userDataB->type == PLAYER_SP_ATTACK)
    {
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 0)
        {
            PlayerDamage = 20.0f;
        }
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 1)
        {
            PlayerDamage = 50.0f;
        }
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 2)
        {
            PlayerDamage = 15.0f;
        }
        static_cast<Mushroom*>(userDataA->object)->getEnemyHealth()->Damage(PlayerDamage);
    }
    else if (fixtureAIsSensor && userDataA->type == PLAYER_SP_ATTACK && userDataB->type == MUSHROOM)
    {
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 0)
        {
            PlayerDamage = 20.0f;
        }
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 1)
        {
            PlayerDamage = 50.0f;
        }
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 2)
        {
            PlayerDamage = 15.0f;
        }
        static_cast<Mushroom*>(userDataB->object)->getEnemyHealth()->Damage(PlayerDamage);
    }
    if (fixtureBIsSensor && userDataA->type == SPIDER && userDataB->type == PLAYER_SP_ATTACK)
    {
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 0)
        {
            PlayerDamage = 20.0f;
        }
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 1)
        {
            PlayerDamage = 50.0f;
        }
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 2)
        {
            PlayerDamage = 15.0f;
        }
        static_cast<Spider*>(userDataA->object)->getEnemyHealth()->Damage(PlayerDamage);
    }
    else if (fixtureAIsSensor && userDataA->type == PLAYER_SP_ATTACK && userDataB->type == SPIDER)
    {
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 0)
        {
            PlayerDamage = 20.0f;
        }
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 1)
        {
            PlayerDamage = 50.0f;
        }
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 2)
        {
            PlayerDamage = 15.0f;
        }
        static_cast<Spider*>(userDataB->object)->getEnemyHealth()->Damage(PlayerDamage);
    }
    if (fixtureBIsSensor && userDataA->type == SKELETON && userDataB->type == PLAYER_SP_ATTACK)
    {
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 0)
        {
            PlayerDamage = 20.0f;
        }
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 1)
        {
            PlayerDamage = 50.0f;
        }
        if (static_cast<Player*>(userDataB->object)->GetCharactertype() == 2)
        {
            PlayerDamage = 15.0f;
        }
        static_cast<Skeleton*>(userDataA->object)->getEnemyHealth()->Damage(PlayerDamage);
    }
    else if (fixtureAIsSensor && userDataA->type == PLAYER_SP_ATTACK && userDataB->type == SKELETON)
    {
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 0)
        {
            PlayerDamage = 20.0f;
        }
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 1)
        {
            PlayerDamage = 50.0f;
        }
        if (static_cast<Player*>(userDataA->object)->GetCharactertype() == 2)
        {
            PlayerDamage = 15.0f;
        }
        static_cast<Skeleton*>(userDataB->object)->getEnemyHealth()->Damage(PlayerDamage);
    }

    //Player takes Boss Battle Damage
    if (fixtureAIsSensor && userDataA->type == GOLEM_SLASH && userDataB->type == PLAYER)
    {
        //Game::GetInstance().GetCharacter()->IsGodmode(); // Changes made by Karl - Check for godmode
        //Changes made by Kyle
        //Just to test
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(10);
        //Changes ended
    }
    else if (fixtureBIsSensor && userDataA->type == PLAYER && userDataB->type == GOLEM_SLASH)
    {
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(10);

    }
    if (fixtureAIsSensor && userDataA->type == GOLEM_SLAM && userDataB->type == PLAYER)
    {
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(20);
    }
    else if (fixtureBIsSensor && userDataA->type == PLAYER && userDataB->type == GOLEM_SLAM)
    {
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(20);
    }

    //Minion walking damage
    if (fixtureAIsSensor && userDataA->type == MUSHROOM && userDataB->type == PLAYER)
    {
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(3);
    }
    else if (fixtureBIsSensor && userDataA->type == PLAYER && userDataB->type == MUSHROOM)
    {
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(3);
    }
    if (fixtureAIsSensor && userDataA->type == SKELETON && userDataB->type == PLAYER)
    {
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(3);
    }
    else if (fixtureBIsSensor && userDataA->type == PLAYER && userDataB->type == SKELETON)
    {
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(3);
    }
    if (fixtureAIsSensor && userDataA->type == SPIDER && userDataB->type == PLAYER)
    {
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(5);
    }
    else if (fixtureBIsSensor && userDataA->type == PLAYER && userDataB->type == SPIDER)
    {
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(5);
    }

    //Minion attack damage
    if (userDataA->type == MUSHROOM_HEAD && userDataB->type == PLAYER)
    {
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(5);
    }
    else if (userDataA->type == PLAYER && userDataB->type == MUSHROOM_HEAD)
    {
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(5);
    }
    if (userDataA->type == SPIDER_BITE && userDataB->type == PLAYER)
    {
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(7);
    }
    else if (userDataA->type == PLAYER && userDataB->type == SPIDER_BITE)
    {
        static_cast<Player*>(userDataA->object)->getPlayerHealthbar()->Damage(7);
    }
    if (userDataA->type == SKELETON_ATTACK && userDataB->type == PLAYER)
    {
        static_cast<Player*>(userDataB->object)->getPlayerHealthbar()->Damage(5);
    }
    else if (userDataA->type == PLAYER && userDataB->type == SKELETON_ATTACK)
    {
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
