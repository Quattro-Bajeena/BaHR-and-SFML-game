#pragma once
#include "RNG.h"
class AudioManager
{
private:
	std::string path;
	std::map<std::string, std::vector<sf::SoundBuffer>> sounds;
	std::map<std::string, sf::Music> music;
	std::queue<sf::Sound> currentSounds;

	float lastSoundTime;
	float sweepTime;
	float sweepTimeMax;
	int maxSounds;

public:
	AudioManager(std::string path);

	void addSound(std::string key, std::string file_name);
	void play(std::string key, float volume = 10, float pitch = 1);
	void playRandom(std::string key, float volume = 10, float pitch = 1);

	void update(const float& dt);

};

