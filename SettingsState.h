#pragma once
#include "State.h"


class SettingsState :
	public State
{
private:
	//Variables
	sf::RectangleShape background;

	std::map<std::string, std::unique_ptr<gui::Button>> buttons;
	std::map<std::string, std::unique_ptr<gui::DropDownList>> dropDownLists;

	float fontSize;
	sf::Text optionsText;
	std::vector<sf::VideoMode> modes;
	std::vector<int> framerates;
	std::vector<int> antiAliasing;
	
	//Functions
	void initVariables();
	void initBackground();
	void initGui();
	void initText();

public:
	SettingsState(StateData& state_data);
	virtual ~SettingsState();

	void refreshGui();
	//Accesors


	//Functions
	void updateInput(const float& dt);
	void updateGui();
	void update(const float& dt);
	void renderGui(sf::RenderTarget& target) const;
	void render(sf::RenderTarget* target = nullptr) const;

};

