#include "button.h"
#include <cassert>
#include <string>
#include "logmanager.h"
#include <sstream>

Button::Button(float x, float y) :
    m_x(x),
    m_y(y),
    m_isHovered(false),
    m_isPressed(false),
    m_isClicked(false),
    m_isReleased(false),
    m_wasReleased(false),
    m_isHeld(false),
    m_wasHeld(false)
    , linkBoolFlag(false)
    , m_wasClicked(false)
    , m_width(0), //should become equal to the sprite width
    m_height(0) //should become equal to the sprite height
    , initialized(false)
    , m_fElapsedTime(0.0f)
    , m_fElapsedTime2(0.0f)
    , m_angle(0.0f)
{
}

Button::~Button()
{
}

bool Button::Initialise(Renderer& renderer)
{
    initialized = true;
    // Load textures or sprites
    // Example: m_buttonTexture = renderer.LoadTexture("button.png");
    return true;
}

void Button::Update(float deltaTime, InputSystem& inputSystem)
{
    // Calculate the center of the button
    float centerX = m_x;
    float centerY = m_y;

    // Calculate the unrotated corners of the button
    float halfWidth = m_width / 2.0f;
    float halfHeight = m_height / 2.0f;
    Vector2 topLeft(centerX - halfWidth, centerY - halfHeight);
    Vector2 topRight(centerX + halfWidth, centerY - halfHeight);
    Vector2 bottomLeft(centerX - halfWidth, centerY + halfHeight);
    Vector2 bottomRight(centerX + halfWidth, centerY + halfHeight);

    // Apply rotation if angle is not zero
    if (m_angle != 0.0f)
    {
        // Convert angle to radians
        float radians = m_angle * (3.14159f / 180.0f);

        // Rotate each corner around the center
        RotatePoint(topLeft, centerX, centerY, radians);
        RotatePoint(topRight, centerX, centerY, radians);
        RotatePoint(bottomLeft, centerX, centerY, radians);
        RotatePoint(bottomRight, centerX, centerY, radians);
    }

    // Check if the mouse position is inside the rotated bounding box
    const Vector2& mousePos = inputSystem.GetMousePosition();
    m_isHovered = IsPointInPolygon(mousePos, topLeft, topRight, bottomRight, bottomLeft);

    // Check if the button is pressed or released
    ButtonState mouseButtonState = inputSystem.GetMouseButtonState(SDL_BUTTON_LEFT); // Assuming left mouse button

    if (!m_isHovered && mouseButtonState == BS_PRESSED)
    {
        m_isClicked = true;
    }
    if (m_isClicked || m_wasClicked)
    {
        m_fElapsedTime2 += deltaTime;
    }
    // This doesn't work perfectly, HOWEVER it's close enough
    // But if I need to do more adjustments it's here.
    if (m_fElapsedTime2 > 1.0000f) // 1 frame
    {
        m_wasClicked = true;
        m_isClicked = false;
        if (m_fElapsedTime2 > 1.0005f) // 1 frame
        {
            m_wasClicked = false;
            m_fElapsedTime2 = 0;
        }
    }
    if (m_wasHeld)
    {
        if (mouseButtonState == BS_RELEASED)
        {
            m_wasHeld = false;
            m_wasReleased = true;
        }
    }
    if (m_fElapsedTime > 0.0005f) // 1 frame
    {
        m_fElapsedTime = 0;
        m_wasReleased = false;
        m_isClicked = false;
    }
    if (m_wasReleased)
    {
        m_fElapsedTime += deltaTime;
    }

    if (m_isHovered && !m_isReleased)
    {
        if (mouseButtonState == BS_PRESSED)
        {
            m_isPressed = true;
        }
        else if (mouseButtonState == BS_HELD)
        {
            m_isHeld = true; // Button is being held down
            m_wasHeld = true;
            m_isReleased = false;
        }
        else if (mouseButtonState == BS_RELEASED)
        {
            m_isHeld = false; // Button is released
            m_wasHeld = false;
            m_isReleased = true; // Mark as released for one frame
        }
        //gonna hide this just so I can use it for things later
//        LogManager::GetInstance().Log("being hovered!");

    }
    else
    {
        m_isPressed = false;
        m_isHeld = false;
        m_isReleased = false;
    }
}

bool Button::getInitialized() const
{
    return initialized;
}

void Button::Draw(Renderer& renderer)
{
    // Render button based on its state
    // Example:
    // if (m_isPressed) renderer.DrawTexture(m_buttonPressedTexture, m_x, m_y);
    // else if (m_isHovered) renderer.DrawTexture(m_buttonHoveredTexture, m_x, m_y);
    // else renderer.DrawTexture(m_buttonNormalTexture, m_x, m_y);
}

bool Button::IsClicked() const
{
    return m_isPressed;
}

bool Button::IsReleased() const
{
    return m_isReleased;
}

// Button.cpp
void Button::SetAngle(float angle) {
    m_angle = angle;
}

float Button::GetAngle() const {
    return m_angle;
}

//tester
// Helper function to rotate a point around the center by an angle in radians
void Button::RotatePoint(Vector2& point, float centerX, float centerY, float radians)
{
    float cosTheta = cosf(radians);
    float sinTheta = sinf(radians);

    float x = point.x - centerX;
    float y = point.y - centerY;

    point.x = centerX + (x * cosTheta - y * sinTheta);
    point.y = centerY + (x * sinTheta + y * cosTheta);
}

// Helper function to check if a point is inside a polygon (in this case, a rotated rectangle)
bool Button::IsPointInPolygon(const Vector2& point, const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4)
{
    // Use a point-in-polygon algorithm like crossing number or winding number
    // Here, we'll use a simple method that works for convex polygons like rectangles
    // Cross product method: check if point is on the correct side for all edges
    return (IsOnLeftSide(point, v1, v2) && IsOnLeftSide(point, v2, v3) &&
        IsOnLeftSide(point, v3, v4) && IsOnLeftSide(point, v4, v1));
}

// Check if a point is on the left side of a line segment formed by two points
bool Button::IsOnLeftSide(const Vector2& point, const Vector2& lineStart, const Vector2& lineEnd)
{
    return ((lineEnd.x - lineStart.x) * (point.y - lineStart.y) - (lineEnd.y - lineStart.y) * (point.x - lineStart.x)) >= 0;
}

void Button::PrintRotatedButtonArea()
{
    // Define the four corners of the button before rotation
    Vector2 topLeft(m_x - m_width / 2, m_y - m_height / 2);
    Vector2 topRight(m_x + m_width / 2, m_y - m_height / 2);
    Vector2 bottomLeft(m_x - m_width / 2, m_y + m_height / 2);
    Vector2 bottomRight(m_x + m_width / 2, m_y + m_height / 2);

    // Angle in radians
    float radians = GetAngle() * (3.14159f / 180.0f);
    float angle = GetAngle();
    // Rotate each corner around the button's center (m_x, m_y)
    RotatePoint(topLeft, m_x, m_y, radians);
    RotatePoint(topRight, m_x, m_y, radians);
    RotatePoint(bottomLeft, m_x, m_y, radians);
    RotatePoint(bottomRight, m_x, m_y, radians);

    // Print out the rotated points
    std::string words = "Rotated Button Area:\nTop Left: (" +
        std::to_string(topLeft.x) + ", " + std::to_string(topLeft.y) + ")\n" +
        "Top Right: (" + std::to_string(topRight.x) + ", " + std::to_string(topRight.y) + ")\n" +
        "Bottom Left: (" + std::to_string(bottomLeft.x) + ", " + std::to_string(bottomLeft.y) + ")\n" +
        "Bottom Right: " + std::to_string(bottomRight.x) + ", " + std::to_string(bottomRight.y) + ")\n" +
        "Angle: " + std::to_string(angle) + " degrees";
    LogManager::GetInstance().Log(words.c_str());
}

