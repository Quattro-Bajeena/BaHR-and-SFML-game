#pragma once

#include "Player.h"
#include "GameState.h"

class MenuState;
class GameState;
class Player;
class State;

class EndScreenState :
	public State
{
private:
	//Variables
	sf::RectangleShape background;

	GameStatistics& statistics;
	std::map<std::string, gui::Button*> buttons;
	std::map<std::string, sf::Text> texts;

	//initialier Functions
	void initVariables();
	void initBackground();
	void initButtons();
	void initText();
	void initGui();

public:
	EndScreenState(StateData& state_data, GameStatistics& stats, AudioManager& audio);
	virtual ~EndScreenState();

	void updateInput(const float& dt);
	void updateButtons();
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target) const;
	void render(sf::RenderTarget* target = nullptr) const;
};

