#include "stdafx.h"
#include "TextBox.h"

gui::TextBox::TextBox(float x, float y, float width, float height, const sf::Vector2f& scale,
    const sf::Font& font, const std::string default_text, int character_size, int character_limit,
    sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
    sf::Color idle_Color, sf::Color hover_Color, sf::Color active_Color,
    sf::Color outline_idle_Color, sf::Color outline_hover_Color, sf::Color outline_active_Color,
    short unsigned id)
    :font(font), fontSize(character_size), orgPos(x,y), orgSize(width,height), characterLimit(character_limit),string(default_text), defaultString(default_text),
	textIdleColor(text_idle_color), textHoverColor(text_hover_color), textActiveColor(text_active_color),
	idleColor(idle_Color), hoverColor(hover_Color), activeColor(active_Color),
	outlineIdleColor(outline_idle_Color), outlineHoverColor(outline_hover_Color), outlineActiveColor(outline_active_Color)
{
	this->state = guiStates::IDLE;
	this->pressed = false;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_Color);
	this->shape.setOutlineThickness(static_cast<int>(-3.f * scale.x));
	this->shape.setOutlineColor(outline_idle_Color);


	this->text.setFont(this->font);
	this->text.setString(default_text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.5f
		);

	this->scale(scale);
}

gui::TextBox::~TextBox()
{
}

void gui::TextBox::scale(const sf::Vector2f& scale)
{
	this->shape.setPosition(sf::Vector2f(this->orgPos.x * scale.x, this->orgPos.y * scale.y));
	this->shape.setScale(scale);

	this->text.setCharacterSize(this->fontSize * scale.x);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.5f
		);
}

void gui::TextBox::setString(const std::string string)
{
	this->string = string;
	this->text.setString(this->string);
}

const sf::String gui::TextBox::getText() const
{
	return  sf::String(this->string);
}

const bool gui::TextBox::isStringDefault() const
{
	return this->string == this->defaultString;
}



const bool gui::TextBox::isActive() const
{
	return this->state == guiStates::ACTIVE;
}


void gui::TextBox::typeText(sf::Uint32 unicode_text)
{
	if (this->state == guiStates::ACTIVE) {
		
		if (unicode_text > 32 && unicode_text < 128 && this->string.length() < this->characterLimit) {
			this->string += static_cast<char>(unicode_text);
		}
		else if (unicode_text == 8 && this->string.length() > 0) {
			this->string.pop_back();
		}
		this->text.setString(this->string);
	}

}

void gui::TextBox::update(const sf::Vector2i& mousePosWindow)
{

	if (this->state == guiStates::ACTIVE) {
		if (this->string == this->defaultString) {
			this->string = "";
			this->text.setString(this->string);
		}
		

		//Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			this->pressed = true;

		}
		else if (this->pressed == true && sf::Mouse::isButtonPressed(sf::Mouse::Left) == false) {
			this->state = guiStates::HOVER;
			this->pressed = false;


		}
		
	}
	else if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {


		//Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
	
			this->pressed = true;
		}
		else if (this->pressed == true && sf::Mouse::isButtonPressed(sf::Mouse::Left) == false) {
			this->state = guiStates::ACTIVE;
			this->pressed = false;

		}
		else this->state = guiStates::HOVER;

	}
	//Idle
	else this->state = guiStates::IDLE;


	switch (this->state) {
	case guiStates::IDLE:
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
		this->shape.setOutlineColor(this->outlineIdleColor);
		break;

	case guiStates::HOVER:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		break;

	case guiStates::RELEASED:
	case guiStates::ACTIVE:
		this->shape.setFillColor(this->activeColor);
		this->text.setFillColor(this->textActiveColor);
		this->shape.setOutlineColor(this->outlineActiveColor);
		break;

	default:
		this->shape.setFillColor(sf::Color::Cyan);
		this->text.setFillColor(sf::Color::Yellow);
		this->shape.setOutlineColor(sf::Color::Magenta);
		break;
	}

	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.5f
		);
}

void gui::TextBox::render(sf::RenderTarget& target) const
{
	target.draw(this->shape);
	target.draw(this->text);
}


