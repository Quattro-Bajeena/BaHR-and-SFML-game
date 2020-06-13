#include "stdafx.h"
#include "AudioManager.h"

AudioManager::AudioManager(std::string path)
	:path(path)
{
	this->lastSoundTime = 0.f;
	this->sweepTime = 0.f;
	this->sweepTimeMax = 5.f;
	this->maxSounds = 50;
	this->currentMusic = nullptr;
}

void AudioManager::addMusic(std::string key, std::string file_name, bool loop)
{
	this->music[key].openFromFile(this->path + file_name);
	if (loop == true) {
		this->music[key].setLoop(true);
	}

}

void AudioManager::playMusic(std::string key, float volume)
{
	if (currentMusic != nullptr) {
		this->currentMusic->stop();
	}
	this->music.at(key).setVolume(volume);
	this->music.at(key).play();
	this->currentMusic = &this->music.at(key);
}

void AudioManager::stopMusic()
{
	this->currentMusic->stop();
}

void AudioManager::addSound(std::string key, std::string file_name)
{
	this->sounds[key].push_back(sf::SoundBuffer());
	this->sounds[key].back().loadFromFile(this->path + file_name);
}

void AudioManager::play(std::string key, float volume, float pitch)
{
	sf::Sound sound;
	sound.setBuffer(this->sounds.at(key).front());
	sound.setPitch(pitch);
	sound.setVolume(volume);
	this->currentSounds.push(sound);
	this->currentSounds.back().play();
	this->lastSoundTime = 0.f;
}

void AudioManager::playRandom(std::string key, float volume, float pitch)
{
	sf::Sound sound;
	int i = RNG::get().randomI(0, this->sounds.at(key).size()-1);

	sound.setBuffer(this->sounds.at(key)[i]);
	sound.setPitch(pitch);
	sound.setVolume(volume);
	this->currentSounds.push(sound);
	this->currentSounds.back().play();
	this->lastSoundTime = 0.f;
}

void AudioManager::update(const float& dt)
{
	this->lastSoundTime += dt;
	if (this->lastSoundTime >= this->sweepTimeMax) {

		this->currentSounds = {};
		this->lastSoundTime = 0.f;
		
	}

	if (this->currentSounds.size() > this->maxSounds) {
		this->currentSounds.pop();
		this->lastSoundTime = 0.f;
	}

	//std::cout <<(int)this->lastSoundTime <<" | " <<this->currentSounds.size() << "\n";
}

	
