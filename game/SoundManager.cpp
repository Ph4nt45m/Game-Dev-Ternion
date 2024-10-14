#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {
    cleanup(); // Ensure everything is cleaned up on destruction
}

bool SoundManager::init() {
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void SoundManager::cleanup() {
    // Free all loaded sound effects
    for (auto& sound : soundEffects) {
        Mix_FreeChunk(sound.second);
    }
    soundEffects.clear();

    // Free all loaded music tracks
    for (auto& music : musicTracks) {
        Mix_FreeMusic(music.second);
    }
    musicTracks.clear();

    // Close SDL_mixer
    Mix_CloseAudio();
}

bool SoundManager::loadSound(const std::string& id, const std::string& filePath) {
    Mix_Chunk* sound = Mix_LoadWAV(filePath.c_str());
    if (!sound) {
        std::cerr << "Failed to load sound: " << filePath << " SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    soundEffects[id] = sound;
    return true;
}

bool SoundManager::loadMusic(const std::string& id, const std::string& filePath) {
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        std::cerr << "Failed to load music: " << filePath << " SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    musicTracks[id] = music;
    return true;
}

void SoundManager::playSound(const std::string& id, int loops) {
    auto it = soundEffects.find(id);
    if (it != soundEffects.end()) {
        Mix_PlayChannel(-1, it->second, loops);
    }
    else {
        std::cerr << "Sound ID not found: " << id << std::endl;
    }
}

void SoundManager::stopSound(const std::string& id) {
    auto it = soundEffects.find(id);
    if (it != soundEffects.end()) {
        // Halt the sound effect on all channels where it is playing
        for (int i = 0; i < Mix_AllocateChannels(-1); ++i) {
            if (Mix_GetChunk(i) == it->second) {
                Mix_HaltChannel(i); // Stop sound on this channel
            }
        }
    }
    else {
        std::cerr << "Sound ID not found: " << id << std::endl;
    }
}
void SoundManager::stopAllSounds() {
    Mix_HaltChannel(-1); // Stop all sound effects
}

void SoundManager::playMusic(const std::string& id, int loops) {
    auto it = musicTracks.find(id);
    if (it != musicTracks.end()) {
        Mix_PlayMusic(it->second, loops);
    }
    else {
        std::cerr << "Music ID not found: " << id << std::endl;
    }
}

void SoundManager::stopMusic() {
    Mix_HaltMusic(); // Stops the currently playing music
}

void SoundManager::removeMusic(const std::string& id) {
    auto it = musicTracks.find(id);
    if (it != musicTracks.end()) {
        // Free the music track memory and remove it from the map
        Mix_FreeMusic(it->second);
        musicTracks.erase(it);
        std::cout << "Music track with ID: " << id << " has been removed." << std::endl;
    }
    else {
        std::cerr << "Music ID not found: " << id << std::endl;
    }
}

void SoundManager::setSoundVolume(const std::string& id, int volume) {
    auto it = soundEffects.find(id);
    if (it != soundEffects.end()) {
        // Adjust volume on all channels that play this sound
        for (int i = 0; i < Mix_AllocateChannels(-1); ++i) {
            if (Mix_GetChunk(i) == it->second) {
                Mix_Volume(i, volume);  // Set volume for the sound effect's channel
            }
        }
    }
    else {
        std::cerr << "Sound ID not found: " << id << std::endl;
    }
}

void SoundManager::setMusicVolume(int volume) {
    Mix_VolumeMusic(volume);  // Set volume for the music
}

int SoundManager::getMusicVolume() {
    return Mix_VolumeMusic(-1);  // Get the current music volume
}