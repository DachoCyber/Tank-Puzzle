#define SFML_STATIC
#include "../include/loadSound.h"
#include <iostream>
#include "loadSound.h"

sf::SoundBuffer mirrorSoundBuffer;
sf::Sound mirrorSound;

sf::SoundBuffer hitWallSoundBuffer;
sf::Sound hitWallSound;

sf::SoundBuffer levelWonSoundBfr;
 sf::Sound levelWonSound;

 sf::SoundBuffer gameOverSoundBfr;
 sf::Sound gameOverSound;

void loadMirrorBubbleSound(const std::string& path) {
    if (!mirrorSoundBuffer.loadFromFile(path)) {
        std::cerr << "Failed to load sound: " << path << std::endl;
        return;
    }
    mirrorSound.setBuffer(mirrorSoundBuffer);
}

void loadHitWallSoundBuffer(const std::string& path) {
    if (!hitWallSoundBuffer.loadFromFile(path)) {
        std::cerr << "Failed to load sound: " << path << std::endl;
        return;
    }
    hitWallSound.setBuffer(hitWallSoundBuffer);
}

void loadWonSound(const std::string &path) {
    if (!levelWonSoundBfr.loadFromFile(path)) {
        std::cerr << "Failed to load sound: " << path << std::endl;
        return;
    }
    levelWonSound.setBuffer(levelWonSoundBfr);

}

void loadGameOverSound(const std::string& path) {
    if(!gameOverSoundBfr.loadFromFile(path)) {
        std::cerr << "Failed to load sound: " << path << std::endl;
        return;
    }
    gameOverSound.setBuffer(gameOverSoundBfr);
}
