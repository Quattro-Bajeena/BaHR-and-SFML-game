#include "stdafx.h"
#include "CheckBox.h"

gui::CheckBox::CheckBox(float x, float y, float width, float height,
	sf::Vector2f scale,
	sf::Color check_idle_Color, sf::Color check_hover_Color, sf::Color check_active_Color,
	sf::Color outline_idle_Color, sf::Color outline_hover_Color, sf::Color outline_active_Color,
	sf::Color box_idle_Color, sf::Color box_hover_Color, sf::Color box_active_Color,
	int id)
	: orgPos(x,y),orgSize(width,height),
	checkIdleColor(check_idle_Color), checkHoverColor(check_hover_Color), checkActiveColor(check_active_Color),
	outlineIdleColor(outline_idle_Color), outlineHoverColor(outline_hover_Color), outlineActiveColor(outline_active_Color),
	boxIdleColor(box_idle_Color), boxHoverColor(box_hover_Color), boxActiveColor(box_active_Color),
	id(id)
{
	this->checked = false;
	this->state = guiStates::IDLE;

	this->boxShape.setPosition(sf::Vector2f(x, y));
	this->boxShape.setSize(sf::Vector2f(width, height));
	this->boxShape.setFillColor(box_idle_Color);
	
	this->boxShape.setOutlineColor(outline_idle_Color);

	sf::Vector2f center_pos = sf::Vector2f(
		this->boxShape.getGlobalBounds().left + this->boxShape.getGlobalBounds().width / 2,
		this->boxShape.getGlobalBounds().top + this->boxShape.getGlobalBounds().height / 2);


	this->ratio = 0.8f;
	this->checkShape.setSize(sf::Vector2f(this->boxShape.getSize().x * this->ratio, this->boxShape.getSize().y * 0.75));
	this->checkShape.setPosition(
		this->boxShape.getPosition().x + (1 - this->ratio) / 2 * this->boxShape.getSize().x,
		this->boxShape.getPosition().y + (1 - this->ratio) / 2 * this->boxShape.getSize().y);
	this->checkShape.setFillColor(check_idle_Color);

	this->boxShape.setOutlineThickness(static_cast<int>(10.f * scale.x));
	
	this->scale(scale);
}

const bool gui::CheckBox::isChecked() const
{
	return this->checked;
}

const int gui::CheckBox::getId() const
{
	return this->id;
}



void gui::CheckBox::check()
{
	this->checked = !this->checked;
}

void gui::CheckBox::setCheck(bool condition)
{
	this->checked = condition;
}

void gui::CheckBox::scale(sf::Vector2f scale)
{
	this->boxShape.setPosition(sf::Vector2f(this->orgPos.x * scale.x, this->orgPos.y * scale.y));
	this->boxShape.setScale(scale);

	this->checkShape.setSize(sf::Vector2f(this->boxShape.getSize().x * this->ratio, this->boxShape.getSize().y * 0.75));
	this->checkShape.setPosition(
		this->boxShape.getPosition().x + (1 - this->ratio) / 2 * this->boxShape.getSize().x,
		this->boxShape.getPosition().y + (1 - this->ratio) / 2 * this->boxShape.getSize().y);
}

void gui::CheckBox::setId(const int id)
{
	this->id = id;
}

void gui::CheckBox::update(const sf::Vector2i& mousePosWindow)
{
	/*Update Booleans for hover and pressed*/
	//inaczej niz w tutorialu zeby przycisk sie "klikal" dopiero po puszczeniu go

	//Hover
	if (this->boxShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {


		//Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->state = guiStates::ACTIVE;

		}

		//Relesing the button
		else if (this->state == guiStates::ACTIVE && sf::Mouse::isButtonPressed(sf::Mouse::Left) == false) {
			this->state = guiStates::RELEASED;
			this->checked = !this->checked;
		}
		else this->state = guiStates::HOVER;
	}

	//Idle
	else this->state = guiStates::IDLE;

	
	switch (this->state) {
	case guiStates::IDLE:
		this->boxShape.setFillColor(this->boxIdleColor);
		this->boxShape.setOutlineColor(this->outlineIdleColor);
		this->checkShape.setFillColor(this->checkIdleColor);
		break;

	case guiStates::HOVER:
	case guiStates::RELEASED:
		this->boxShape.setFillColor(this->boxHoverColor);
		this->boxShape.setOutlineColor(this->outlineHoverColor);
		this->checkShape.setFillColor(this->checkHoverColor);
		break;

	case guiStates::ACTIVE:
		this->boxShape.setFillColor(this->boxActiveColor);
		this->boxShape.setOutlineColor(this->outlineActiveColor);
		this->checkShape.setFillColor(this->checkActiveColor);
		break;

	default:
		this->boxShape.setFillColor(sf::Color::Cyan);
		this->boxShape.setOutlineColor(sf::Color::Magenta);
		break;
	}
}

void gui::CheckBox::render(sf::RenderTarget& target)
{
	target.draw(this->boxShape);
	if (this->checked == true) {
		target.draw(this->checkShape);
	}
}
