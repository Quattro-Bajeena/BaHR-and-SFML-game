#pragma once

#include "State.h"
#include "GameState.h"
#include "EditorState.h"
#include "SettingsState.h"
#include "WorldEditorState.h"

class MenuState :
	public State
{
private:
	//Variables
	sf::RectangleShape background;
	sf::Text mouseText;

	GameStatistics& statistics;
	std::map<std::string, std::unique_ptr<gui::Button>> buttons;
	std::map<std::string, sf::Text> texts;
	std::map<std::string, sf::Vector2f> textsOrgPos;

	std::unique_ptr<gui::TextBox> textBox;
	float fontSize1;

	//Functions
	void initVariables();
	void initBackground();
	void initGui();
	void initButtons();
	void initText();

public:
	MenuState(StateData& state_data, GameStatistics& statistics);
	virtual ~MenuState();

	//Function
	void refreshGui();
	void updateInput(const float& dt);
	void updateButtons();
	void updateGui();
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target) const;
	void render(sf::RenderTarget* target = nullptr) const;
};

