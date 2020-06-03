#ifndef GRAPHICSETTINGS_H
#define GRAPHICSETTINGS_H


class GraphicSettings {

public:
	GraphicSettings();
	//Variables

	std::string title;
	sf::VideoMode resolution;
	bool fullscreen;
	bool vsync;
	int frameRateLimit;
	sf::ContextSettings contexSettings;
	std::vector<sf::VideoMode> videoModes;

	//Functions
	void saveToFile(const std::string path);
	void loadFromFile(const std::string path);
};

#endif // !GRAPHICSETTINGS_H