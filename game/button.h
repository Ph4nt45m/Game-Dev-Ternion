#pragma once

#ifndef __BUTTON_H_
#define __BUTTON_H_

#include "renderer.h"
#include "inputSystem.h" // Assuming this header exists and has necessary input handling

class Button
{
public:
    Button(float x, float y);
    virtual ~Button();

    // Initializes the button (e.g., load textures)
    virtual bool Initialise(Renderer& renderer) = 0;

    // Updates button state based on input
    virtual void Update(float deltaTime, InputSystem& inputSystem) = 0;

    // Renders the button
    virtual void Draw(Renderer& renderer) = 0;

    // Set the width and height of the button
    void SetWidth(float width) { m_width = width; }
    void SetHeight(float height) { m_height = height; }
    // Checks if the button was clicked
    bool IsClicked() const;
    bool IsReleased() const;
    float GetX() { return m_x; }
    float GetY() { return m_y; }
    void SetAngle(float angle);  // Add this
    float GetAngle() const;      // Add this
    void PrintRotatedButtonArea();

    bool getInitialized() const;
protected:
    // Button state
    bool m_isHovered;
    bool m_isPressed;
    bool m_isClicked;
    bool m_isReleased;
    bool m_wasReleased;
    bool m_isHeld;
    bool m_wasHeld;
    bool m_wasClicked;
    // Button properties
    float m_x, m_y;
    float m_width, m_height;
    float m_angle;

    bool initialized;

    bool linkBoolFlag; //this was my scuff workaround don't know don't ask
    //time
    float m_fElapsedTime;    // To track time or handle animations, if needed
    float m_fElapsedTime2;
    // Visual assets (e.g., button textures)
    // Add texture or sprite member variables as needed

private:
    // Prevent copying
    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;

    void RotatePoint(Vector2& point, float centerX, float centerY, float radians);
    bool IsPointInPolygon(const Vector2& point, const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4);
    bool IsOnLeftSide(const Vector2& point, const Vector2& lineStart, const Vector2& lineEnd);

};

#endif // !__BUTTON_H_
