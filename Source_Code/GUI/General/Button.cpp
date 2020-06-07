#include "stdafx.h"
#include "Button.h"

gui::Button::Button(float x, float y, float width, float height, sf::Vector2f scale,
	const sf::Font* font, std::string text, int character_size,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_Color, sf::Color hover_Color, sf::Color active_Color,
	sf::Color outline_idle_Color, sf::Color outline_hover_Color, sf::Color outline_active_Color,
	short unsigned id)
	:id(id), orgPos(x,y), orgSize(width,height), orgFontSize(character_size), font(font),
	textIdleColor( text_idle_color), textHoverColor (text_hover_color), textActiveColor (text_active_color),
	idleColor( idle_Color), hoverColor (hover_Color), activeColor(active_Color),
	outlineIdleColor (outline_idle_Color), outlineHoverColor(outline_hover_Color), outlineActiveColor(outline_active_Color)
{
	this->buttonState = guiStates::IDLE;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_Color);
	this->shape.setOutlineThickness(static_cast<int>(3.f* scale.x));
	this->shape.setOutlineColor(outline_idle_Color);


	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.5f
	);


	this->scale(scale);
}

gui::Button::~Button()
{
}


//Accesors
const bool gui::Button::isPressed() const
{
	return this->buttonState == guiStates::ACTIVE;
}

const bool gui::Button::isReleased() const
{
	return this->buttonState == guiStates::RELEASED;
}

const sf::String gui::Button::getText() const
{
	return this->text.getString();
}

const short unsigned& gui::Button::getId() const
{
	return this->id;
}


//Modifiers

void gui::Button::scale(sf::Vector2f scale)
{

	this->shape.setPosition(sf::Vector2f(this->orgPos.x * scale.x, this->orgPos.y * scale.y));
	this->shape.setScale(scale);


	this->text.setCharacterSize(this->orgFontSize * scale.x);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.5f
		);

	

}

void gui::Button::setText(const sf::String text)
{
	this->text.setString(text);
}

void gui::Button::setId(const short unsigned id)
{
	this->id = id;
}



//Function

void gui::Button::update(const sf::Vector2i& mousePosWindow)
{
	/*Update Booleans for hover and pressed*/
	//inaczej niz w tutorialu zeby przycisk sie "klikal" dopiero po puszczeniu go

	//Hover
	if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {


		//Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {


			this->buttonState = guiStates::ACTIVE;
			//std::cout << "Button pressed" << "\n";

		}

		//Relesing the button
		else if (this->buttonState == guiStates::ACTIVE && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->buttonState = guiStates::RELEASED;
			//std::cout << "Button released" << "\n";
		}
		else this->buttonState = guiStates::HOVER;
	}

	//Idle
	else this->buttonState = guiStates::IDLE;


	switch (this->buttonState) {
	case guiStates::IDLE:
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
		this->shape.setOutlineColor(this->outlineIdleColor);
		break;

	case guiStates::HOVER:
	case guiStates::RELEASED:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		break;

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


}

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}