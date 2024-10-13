#pragma once
#ifndef ALPHABET_h
#define ALPHABET_h

#include "renderer.h"
#include "sprite.h"
#include <string>
#include <map>

class Alphabet {
public:
    Alphabet();
    ~Alphabet();

    bool Initialize(Renderer& renderer); // Load all the images
    void DrawTextW(Renderer& renderer, const std::string& text, int x, int y); // Draw a string at specified position

private:
    std::map<char, Sprite*> letterMap;  // Map to store the letter images
    int letterWidth;                    // Width of each letter (for spacing)
    int letterHeight;                   // Height of each letter
};
#endif // ALPHABET_h
