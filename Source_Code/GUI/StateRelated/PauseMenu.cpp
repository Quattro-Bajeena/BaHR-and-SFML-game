#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font, sf::Vector2f scale) :
	font(font), orgScale(scale)
{
	this->background.setSize(static_cast<sf::Vector2f>(window.getSize()));
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	//init container
	this->container.setSize(sf::Vector2f(
		static_cast<float>(window.getSize().x / 4),
		static_cast<float>(window.getSize().y) - 120 * scale.y
		)
		);
	this->container.setFillColor(sf::Color(20, 20, 20, 200));
	this->container.setPosition(
		static_cast<float>(window.getSize().x) / 2.f - this->container.getSize().x / 2.f,
		60.f
		);

	//Init text
	this->menuText.setFont(font);
	this->menuText.setFillColor(sf::Color(255, 255, 255, 200));
	this->menuText.setCharacterSize(70 * scale.x);
	this->menuText.setString("PAUSED");
	this->menuText.setPosition(
		this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText.getGlobalBounds().width / 2.f,
		this->container.getPosition().y + 20
		);


}

PauseMenu::~PauseMenu()
{


}


std::map<std::string, std::unique_ptr<gui::Button>>& PauseMenu::getButtons()
{
	return this->buttons;
}

const std::string PauseMenu::getTextBoxString(const std::string key)
{
	return this->textBoxes.at(key)->getText();
}

const std::string PauseMenu::getListString(const std::string key)
{
	return this->dropDownLists.at(key)->getActiveText();
}

const short int PauseMenu::getListId(const std::string key)
{
	return this->dropDownLists.at(key)->getActiveElementId();
}


const bool PauseMenu::isButtonReleased(const std::string key)
{
	return this->buttons.at(key)->isReleased();
}

const bool PauseMenu::isListReleased(const std::string key)
{
	return this->dropDownLists.at(key)->isReleased();
}

void PauseMenu::setTextBoxString(const std::string key, const std::string text)
{
	this->textBoxes.at(key)->setString(text);
}

void PauseMenu::reloadList(const std::string key, const std::vector<std::string>& list)
{
	this->dropDownLists.at(key)->reload(list);
}


//Functions
void PauseMenu::addButton(const std::string key, float y, const std::string text)
{
	float width = 190.f;
	float height = 80.f;
	float x = (this->container.getPosition().x + this->container.getSize().x / 2.f - width* orgScale.x / 2.f) / this->orgScale.x;
	this->buttons[key] = std::make_unique< gui::Button>(
		x, y, width, height, this->orgScale,
		&this->font, text, 60,
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 255), sf::Color(100, 100, 100, 200),
		sf::Color(170, 170, 170, 0), sf::Color(200, 200, 200, 0), sf::Color(100, 100, 100, 0)
		);
}

void PauseMenu::addTextBox(const std::string key, float y, std::string default_text)
{
	float width = 400.f;
	float height = 70.f;
	float x = (this->container.getPosition().x + this->container.getSize().x / 2.f - width * orgScale.x / 2.f) / this->orgScale.x;
	this->textBoxes[key] = std::make_unique< gui::TextBox>(
		x, y, width, height, this->orgScale,
		this->font, default_text, 40, 17,
		sf::Color(40, 40, 40, 255), sf::Color(20, 20, 20, 255), sf::Color(5, 5, 5, 255),
		sf::Color(200, 200, 200, 200), sf::Color(230, 230, 230, 230), sf::Color(240, 240, 240, 255)
		);
}

void PauseMenu::addList(const std::string key, float y, std::vector<std::string> list)
{
	float width = 320.f;
	float height = 60.f;
	float x = (this->container.getPosition().x + this->container.getSize().x / 2.f - width * orgScale.x / 2.f) / this->orgScale.x;


	this->dropDownLists[key] = std::make_unique<gui::DropDownList>(
		x, y, width, height, this->orgScale,
		this->font, list, 0
		);
}

void PauseMenu::updateTextBox(bool& textReceived, sf::Uint32 unicodeText, const sf::Vector2i& mousePosWindow)
{
	for (const auto& [key, text_box] : this->textBoxes) {
		text_box->update(mousePosWindow);

		if (textReceived == true) {
			text_box->typeText(unicodeText);
			textReceived = false;
		}
	}
}

void PauseMenu::update(const sf::Vector2i& mousePosWindow)
{
	if (std::any_of(this->dropDownLists.begin(), this->dropDownLists.end(),
		[](std::pair<const std::string, std::unique_ptr<gui::DropDownList>>& list) {return list.second->isActive() == true; }) == false) {
		//Buttons 
		for (auto& [key, button] : this->buttons) {
			button->update(mousePosWindow);
		}

		//Dropdown lists
		for (auto& [key, list] : this->dropDownLists) {
			list->update(mousePosWindow);
		}
	}
	for (auto& [key, list] : this->dropDownLists) {
		if (list->isActive() == true) {
			list->update(mousePosWindow);
		}

	}
	
}

void PauseMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);
	target.draw(this->menuText);

	for (const auto& [key, button] : this->buttons) {
		button->render(target);
	}
	for (const auto& [key, text_box] : this->textBoxes) {
		text_box->render(target);
	}
	for (const auto& [key, list] : this->dropDownLists) {
		list->render(target);
	}
}

