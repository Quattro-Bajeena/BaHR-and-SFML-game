#pragma once

#include "MenuState.h"

class Game
{
private:
	GraphicSettings gfxSettings;
	StateData stateData;

	sf::RenderWindow* window;
	sf::Event sfEvent;//
	sf::Clock dtClock; //
	float dt;//

	std::stack<State*> states;
	std::unique_ptr<GameStatistics> statistics;

	//GUI

	int gridSize;

	//Initialization
	void initVariables();
	void initWindow();
	void initGraphicSettings();
	void initStateData();
	void initStates();
	void initStatistics();
	void initAssets();

	//Functions

	//Regular
	void endApplication();

	//Update
	void updateDt();
	void updateSFMLEvents();
	void update();

	//Render
	void render() const;

public:
	//Constructors/Destructors
	Game();
	virtual ~Game();

	//Core
	void run();
};

