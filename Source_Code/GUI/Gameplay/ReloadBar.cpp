#include "stdafx.h"
#include "ReloadBar.h"

gui::ReloadBar::ReloadBar(int offset_x, int offset_y, int width, int height, 
	sf::Vector2f scale)
	:scale(scale)
{
	this->offset = sf::Vector2f(offset_x, offset_y);
	this->background.setSize(sf::Vector2f(width, height ));
	//this->background.setScale(scale);
	this->background.setFillColor(sf::Color::Transparent);

	this->line.setSize(sf::Vector2f(this->background.getGlobalBounds().width, this->background.getGlobalBounds().height / 5));
	this->line.setFillColor(sf::Color::White);
	this->line.setOutlineColor(sf::Color::Black);
	this->line.setOutlineThickness(static_cast<int>(2.f * scale.x));

	this->verticalBar.setSize(sf::Vector2f(this->background.getGlobalBounds().width / 12.f, this->background.getGlobalBounds().height));
	this->verticalBar.setFillColor(sf::Color::White);

	this->reloadTimer = 0.f;
	this->reloadTimeMax = 0.f;
	this->reloading = false;
	this->distance = 0.f;

}

void gui::ReloadBar::reloadAnimationStart(const float reload_time_max)
{
	if (this->reloading == false) {
		this->reloading = true;
		this->reloadTimeMax = reload_time_max;
		this->speed = (this->background.getGlobalBounds().width - this->verticalBar.getGlobalBounds().width) / reload_time_max;
		this->reloadTimer = 0.f;
	}
	
}

void gui::ReloadBar::update(sf::Vector2f taret_pos, const float& dt)
{
	
	if (this->reloading == true) {
		this->distance += this->speed * dt;

		this->background.setPosition(taret_pos + this->offset);
		this->line.setPosition(
			this->background.getGlobalBounds().left,
			this->background.getGlobalBounds().top + this->background.getGlobalBounds().height / 2 - this->line.getGlobalBounds().height / 2);
		this->verticalBar.setPosition(this->background.getGlobalBounds().left + this->distance, this->background.getGlobalBounds().top);

		this->reloadTimer += dt;
		if (this->reloadTimer >= this->reloadTimeMax) {
			this->reloading = false;
			this->distance = 0;
			this->reloadTimer = 0.f;
		}
	}
}

void gui::ReloadBar::render(sf::RenderTarget& target)
{
	if (this->reloading == true) {

		target.draw(this->background);
		target.draw(this->line);
		target.draw(this->verticalBar);
	}
}
