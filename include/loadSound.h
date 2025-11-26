#pragma once
#include <SFML/Audio.hpp>
#include <string>

extern sf::SoundBuffer mirrorSoundBuffer;
extern sf::Sound mirrorSound;

extern sf::SoundBuffer hitWallSoundBuffer;
extern sf::Sound hitWallSound;

extern sf::SoundBuffer levelWonSoundBfr;
extern sf::Sound levelWonSound;

extern sf::SoundBuffer gameOverSoundBfr;
extern sf::Sound gameOverSound;

void loadMirrorBubbleSound(const std::string& path);
void loadHitWallSoundBuffer(const std::string& path);
void loadWonSound(const std::string& path);
void loadGameOverSound(const std::string& path);