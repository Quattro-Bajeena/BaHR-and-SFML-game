#pragma once
#include "State.h"
class InfoState :
    public State
{
	sf::RectangleShape background;
	sf::RectangleShape controlsImg;

	std::map<std::string, std::unique_ptr<gui::Button>> buttons;


	float fontSize;
	sf::Text infoText;
	sf::Text controlsText;
	//sf::Text creatorInfoText;

	//Functions

	void initBackground();
	void initGui();
	void initText();

public:
	InfoState(StateData& state_data, AudioManager& audio);
	


	//Functions
	void updateInput(const float& dt);
	void updateGui();
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr) const;
};

