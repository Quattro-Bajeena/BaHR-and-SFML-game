#pragma once

#include "TextBox.h"
#include "DropDownList.h"
class Button;

class PauseMenu
{
private:
	const sf::Font& font;
	sf::Text menuText;
	sf::RectangleShape background;
	sf::RectangleShape container;
	const sf::Vector2f orgScale;

	std::map<std::string, std::unique_ptr<gui::Button>> buttons;
	std::map<std::string, std::unique_ptr<gui::TextBox>> textBoxes;
	std::map<std::string, std::unique_ptr<gui::DropDownList>> dropDownLists;

public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font, sf::Vector2f scale);
	virtual ~PauseMenu();
	

	//Accesor
	std::map<std::string, std::unique_ptr<gui::Button>>& getButtons();
	const std::string getTextBoxString(const std::string key);
	const std::string getListString(const std::string key);
	const short int getListId(const std::string key);

	const bool isButtonReleased(const std::string key);
	const bool isListReleased(const std::string key);

	//Setters
	void setTextBoxString(const std::string key, const std::string text);
	void reloadList(const std::string key, const std::vector<std::string>& list);

	//Functions
	void addButton(const std::string key, float y, const std::string text);
	void addTextBox(const std::string key, float y, std::string default_text);
	void addList(const std::string key, float y, std::vector<std::string> list);
	void updateTextBox(bool& textReceived, sf::Uint32 unicodeText, const sf::Vector2i& mousePosWindow);
	void update(const sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

