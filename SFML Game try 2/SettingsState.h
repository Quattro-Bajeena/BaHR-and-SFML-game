#ifndef SETTINGSTATES_H
#define SETTINGSTATES_H


#include "State.h"
#include "Gui.h"
class SettingsState :
	public State
{
private:
	//Variables
	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Font font;

	std::map<std::string, gui::Button*> buttons;
	std::map<std::string, gui::DropDownList*> dropDownLists;

	sf::Text optionsText;
	std::vector<sf::VideoMode> modes;

	//Functions
	void initVariables();
	void initBackground();
	void initFonts();
	void initKeybinds();
	void initGui();
	void initText();
	
public:
	SettingsState(StateData* state_data);
	virtual ~SettingsState();

	//Accesors


	//Functions
	void updateInput(const float& dt);
	void updateGui();
	void update(const float& dt);
	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);

};

#endif // !SETTINGSTATES_H