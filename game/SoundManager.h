#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <string>
#include <map>

class SoundManager {
public:
    // Constructor and Destructor
    SoundManager();
    ~SoundManager();

    // Initialization and cleanup
    bool init();
    void cleanup();

    // Loading sounds and music
    bool loadSound(const std::string& id, const std::string& filePath);
    bool loadMusic(const std::string& id, const std::string& filePath);

    // Playing sounds and music
    void playSound(const std::string& id, int loops = 0); // Loops 0 means play once
    void playMusic(const std::string& id, int loops = -1); // Loops -1 means infinite looping
    void stopMusic();

private:
    std::map<std::string, Mix_Chunk*> soundEffects; // For sound effects
    std::map<std::string, Mix_Music*> musicTracks;  // For music tracks
};

#endif
