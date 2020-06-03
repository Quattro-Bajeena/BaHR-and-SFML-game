#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font) :
	font(font)
{
	this->background.setSize(static_cast<sf::Vector2f>(window.getSize()));
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	//init container
	this->container.setSize(sf::Vector2f(
		static_cast<float>(window.getSize().x / 4),
		static_cast<float>(window.getSize().y) - 120
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
	this->menuText.setCharacterSize(70);
	this->menuText.setString("PAUSED");
	this->menuText.setPosition(this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText.getGlobalBounds().width/2.f,
		this->container.getPosition().y + 40
		);
}

PauseMenu::~PauseMenu()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}

std::map<std::string, gui::Button*>& PauseMenu::getButtons()
{
	return this->buttons;
}


//Functions

const bool PauseMenu::isButtonReleased(const std::string key)
{
	return this->buttons[key]->isReleased();
}

void PauseMenu::addButton(const std::string key, float y, const std::string text)
{
	float width = 200.f;
	float height = 150.f;
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width/2.f;
	this->buttons[key] = new gui::Button(
		x, y, width, height,
		&this->font, text, 60,
		sf::Color(200, 200, 200, 200), sf::Color(250, 250, 250, 255), sf::Color(100, 100, 100, 200),
		sf::Color(170, 170, 170, 0), sf::Color(200, 200, 200, 0), sf::Color(100, 100, 100, 0)
		);
}

void PauseMenu::update(const sf::Vector2i& mousePosWindow)
{
	for (auto& i : this->buttons) {
		i.second->update(mousePosWindow);
	}
}

void PauseMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);
	target.draw(this->menuText);

	for (auto& i : this->buttons) {
		i.second->render(target);
	}
}
