#include "stdafx.h"
#include "GraphicSettings.h"

//Constuctort
GraphicSettings::GraphicSettings()
{
	this->title = "DEFAULT";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->vsync = false;
	this->frameRateLimit = 60;
	this->contexSettings.antialiasingLevel = 0;
	this->videoModes = sf::VideoMode::getFullscreenModes();
}


//Functions
void GraphicSettings::saveToFile(const std::string path)
{
	std::ofstream ofs(path);

	if (ofs.is_open()) {
		ofs << this->title<<"\n";
		ofs << this->resolution.width<<" "<< this->resolution.height << "\n";
		ofs << this->fullscreen << "\n";
		ofs << this->frameRateLimit << "\n";
		ofs << this->vsync << "\n";
		ofs << this->contexSettings.antialiasingLevel << "\n";
	}
	ofs.close();
}

void GraphicSettings::loadFromFile(const std::string path)
{
	std::ifstream ifs(path);


	if (ifs.is_open()) {
		std::getline(ifs, this->title);
		ifs >> this->resolution.width >> this->resolution.height;
		ifs >> this->fullscreen;
		ifs >> this->frameRateLimit;
		ifs >> this->vsync;
		ifs >> this->contexSettings.antialiasingLevel;
	}
	ifs.close();
}
