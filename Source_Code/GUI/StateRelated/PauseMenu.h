#pragma once
#include "Button.h"
#include "TextBox.h"
class Button;

class PauseMenu
{
private:
	const sf::Font& font;
	sf::Text menuText;
	sf::RectangleShape background;
	sf::RectangleShape container;
	const sf::Vector2f orgScale;

	std::map<std::string, gui::Button*> buttons;
	std::map<std::string, gui::TextBox*> textBoxes;

public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font, sf::Vector2f scale);
	virtual ~PauseMenu();
	

	//Accesor
	std::map<std::string, gui::Button*>& getButtons();
	const std::string getTextBoxString(const std::string key);

	//Functions
	const bool isButtonReleased(const std::string key);
	void addButton(const std::string key, float y, const std::string text);
	void addTextBox(const std::string key, float y, std::string default_text);
	void updateTextBox(bool& textReceived, sf::Uint32 unicodeText, const sf::Vector2i& mousePosWindow);
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

