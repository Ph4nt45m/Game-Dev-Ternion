// MyContactListener.h
#pragma once

#include <box2d/box2d.h>
#include <box2d/collision.h>
#include <box2d/base.h>
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>
#include <box2d/b2_contact.h>
#include <iostream>

class MyContactListener : public b2ContactListener {
public:
    // Constructor and destructor (if needed)
    MyContactListener();
    ~MyContactListener();

    // Overriding the BeginContact method for when two fixtures begin to touch
    void BeginContact(b2Contact* contact) override;

    // Overriding the EndContact method for when two fixtures stop touching
    void EndContact(b2Contact* contact) override;
};
