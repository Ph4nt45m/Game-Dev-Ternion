#include "alphabet.h"
#include <iostream>

#include "logmanager.h"

Alphabet::Alphabet()
{
}

Alphabet::~Alphabet()
{
}

bool Alphabet::Initialize(Renderer& renderer) {
    std::string basePath = "..\\Sprites\\alphabet\\";  // Base path to your alphabet images
    std::string extension = ".png";                    // File extension for your images

    // Loop over the characters 0-9 and A-Z to load each corresponding sprite
    for (char c = '0'; c <= '9'; ++c) {
        std::string filePath = basePath + c + extension;
        letterMap[c] = renderer.CreateSprite(filePath.c_str());  // Use renderer to create sprite
        if (!letterMap[c]) {
            std::cerr << "Failed to load number: " << c << std::endl;
            return false;
        }
    }

    for (char c = 'a'; c <= 'z'; ++c) {
        std::string filePath = basePath + c + extension;  // Assuming lowercase files are named 'a.png', 'b.png', etc.
        letterMap[c] = renderer.CreateSprite(filePath.c_str());
        if (!letterMap[c]) {
            std::cerr << "Failed to load letter: " << c << std::endl;
            return false;
        }
    }

    // Set letter dimensions (adjust based on your image sizes)
    letterWidth = 30;  // Adjust this value according to your sprite size
    letterHeight = 30;

    return true;
}

void Alphabet::Process(const std::string& text, float red, float green, float blue)
{
    for (char c : text) {
        //Setting thing
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';  // Convert uppercase to lowercase
        }

        if (letterMap.find(c) != letterMap.end()) {

            letterMap[c]->SetRedTint(red);
            letterMap[c]->SetBlueTint(green);
            letterMap[c]->SetGreenTint(blue);
        }
    }
}

void Alphabet::DrawTextW(Renderer& renderer, const std::string& text, float x, float y, float size)
{
    // Set letter dimensions (adjust based on your image sizes)
    letterWidth = 10*size;  // Adjust this value according to your sprite size
    letterHeight = 10*size;

    int xOffset = 0;
    int yOffset = 0;
    int windowWidth = renderer.GetWidth();
    int windowHeight = renderer.GetHeight();

    // Calculate the total width of the text
    int totalTextWidth = 0;
    for (char c : text) {
        if (c == ' ') {
            totalTextWidth += letterWidth;
        }
        else if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';  // Convert uppercase to lowercase
        }

        if (letterMap.find(c) != letterMap.end()) {
            totalTextWidth += letterWidth;
        }
    }

    // Calculate the starting x position so the text is centered
    float posX = windowWidth * x;
    float posY = windowHeight * y;
    float startX = posX - (totalTextWidth / 2.0f); // Center the text around posX

    // Reset xOffset to start from the centered position
    xOffset = 0;

    // Loop through each character in the text
    for (char c : text) {
        if (c == ' ') {
            xOffset += letterWidth;  // Just move the xOffset for spaces
            continue;
        }
        if (c == '\n') {
            yOffset += letterHeight;
            xOffset = 0;
        }
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';  // Convert uppercase to lowercase
        }
        if (letterMap.find(c) != letterMap.end()) {
            // Draw the letter at the specified position with the xOffset
            letterMap[c]->SetX(startX + xOffset);
            letterMap[c]->SetY(posY + yOffset);
            letterMap[c]->SetScale(size);

            letterMap[c]->Draw(renderer, false, true);  // Call Draw on the sprite

            xOffset += letterWidth;  // Move the x position for the next letter
        }
    }
}
