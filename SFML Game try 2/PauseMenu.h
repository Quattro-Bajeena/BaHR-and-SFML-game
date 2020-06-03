#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "Gui.h"
class Button;

class PauseMenu
{
private:
	sf::Font& font;
	sf::Text menuText;
	sf::RectangleShape background;
	sf::RectangleShape container;

	std::map<std::string, gui::Button*> buttons;

public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	virtual ~PauseMenu();

	//Accesor
	std::map<std::string, gui::Button*>& getButtons();

	//Functions
	const bool isButtonReleased(const std::string key);
	void addButton(const std::string key, float y, const std::string text);
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

#endif // !PAUSE_MENU_H