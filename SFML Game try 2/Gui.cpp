#include "stdafx.h"
#include "Gui.h"
using namespace gui;

Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string text, int character_size,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_Color, sf::Color hover_Color, sf::Color active_Color,
	sf::Color outline_idle_Color, sf::Color outline_hover_Color, sf::Color outline_active_Color,
	short unsigned id)
	:id(id)
{
	this->buttonState = button_states::BTN_IDLE;

	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setFillColor(idle_Color);
	this->shape.setOutlineThickness(2.f);
	this->shape.setOutlineColor(outline_idle_Color);
	

	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	this->text.setPosition(
		this->shape.getPosition().x +( this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.5f
	);
	

	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color;

	this->idleColor = idle_Color;
	this->hoverColor = hover_Color;
	this->activeColor = active_Color;

	this->outlineIdleColor = outline_idle_Color;
	this->outlineHoverColor = outline_hover_Color;
	this->outlineActiveColor = outline_active_Color;

	
}

Button::~Button()
{
}


//Accesors
const bool Button::isPressed() const
{
	return this->buttonState == button_states::BTN_ACTIVE;
}

const bool Button::isReleased() const
{
	return this->buttonState == button_states::BTN_RELEASED;
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

void gui::Button::setText(const sf::String text)
{
	this->text.setString(text);
}

void gui::Button::setId(const short unsigned id)
{
	this->id = id;
}



//Function

void Button::update(const sf::Vector2i & mousePosWindow)
{
	/*Update Booleans for hover and pressed*/
	//inaczej niz w tutorialu zeby przycisk sie "klikal" dopiero po puszczeniu go

	//Hover
	if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
		

		//Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			
			
			this->buttonState = button_states::BTN_ACTIVE;
			//std::cout << "Button pressed" << "\n";
			
		}

		//Relesing the button
		else if (this->buttonState == button_states::BTN_ACTIVE && !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->buttonState = button_states::BTN_RELEASED;
			//std::cout << "Button released" << "\n";
		}
		else this->buttonState = button_states::BTN_HOVER;
	}

	//Idle
	else this->buttonState = button_states::BTN_IDLE;


	switch (this->buttonState) {
	case button_states::BTN_IDLE:
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
		this->shape.setOutlineColor(this->outlineIdleColor);
		break;

	case button_states::BTN_HOVER:
	case button_states::BTN_RELEASED:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		break;

	case button_states::BTN_ACTIVE:
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

void Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}


//DROPDOWN LIST====================

gui::DropDownList::DropDownList(float x, float y,
	float width, float height,
	sf::Font& font, std::string list[],
	int nrOfElements, const int default_index)
	:font(font), showList(false),keytimeMax(1.f) ,keytime(0.f)
{

	this->activeElement = new Button(
		x, y, width, height,
		&this->font, list[default_index], 32,
		sf::Color(240, 240, 15, 200), sf::Color(255, 255, 230, 255), sf::Color(10, 10, 10, 255),
		sf::Color(100, 100, 100, 250), sf::Color(180, 180, 130, 200), sf::Color(20, 20, 20, 50),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 50)
		);

	for (int i = 0; i < nrOfElements; i++) {
		this->list.push_back(
			new Button(
				x, y + 2 +(height*(i+1)), width, height,
				&this->font, list[i], 32,
				sf::Color(15, 15, 15, 250), sf::Color(250, 250, 250, 200), sf::Color(10, 10, 10, 255),
				sf::Color(100, 100, 100, 250), sf::Color(150, 150, 150, 200), sf::Color(10, 10, 10, 50),
				sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
				i
				)
			);
	}

	
}

gui::DropDownList::~DropDownList()
{
	delete this->activeElement;
	for (auto &i : this->list) {
		delete i;
	}
}




//Accesors
const bool gui::DropDownList::getKeytime()
{
	if (this->keytime >= this->keytimeMax) {
		this->keytime = 0.f;
		return true;
	}
	else return false;
}


const unsigned short gui::DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}

//Functions
void gui::DropDownList::updateKeytime(const float& dt)
{
	//useless for me 
	if (this->keytime < this->keytimeMax)
		this->keytime += dt;
}

void gui::DropDownList::update(const sf::Vector2i& mousePosWindow)
{
	this->activeElement->update(mousePosWindow);

	//Show and hide the list
	if (this->activeElement->isReleased()) {
		if (this->showList)
			this->showList = false;
		else
			this->showList = true;
	}

	if (this->showList) {
		for (auto& i : this->list) {
			i->update(mousePosWindow);
			if (i->isReleased()) {
				this->showList = false;
				this->activeElement->setText(i->getText());
				this->activeElement->setId(i->getId());
				
			}
		}
	}
	

	
}

void gui::DropDownList::render(sf::RenderTarget& target)
{
	this->activeElement->render(target);
	if (this->showList) {
		for (auto& i : this->list) {
			i->render(target);
		}
	}
}


//Texture Selector===============================================

gui::TextureSelector::TextureSelector(float x, float y,
	float width, float height,
	int grid_size,
	const sf::Texture* texture_sheet,
	sf::Font& font, std::string buttonText)
{
	this->active = false;
	this->hidden = false;
	this->gridSize = grid_size;
	float offset = 150.f;
	this->keytimeMax = 0.3f; //dont really need those keytimes
	this->keytime = 0.f;


	this->bounds.setSize(sf::Vector2f(width, height));
	this->bounds.setPosition(x, y);
	this->bounds.setFillColor(sf::Color(50, 50, 50, 100));
	this->bounds.setOutlineThickness(2.f);
	this->bounds.setOutlineColor(sf::Color(255, 255, 255, 200));

	
	this->sheet.setTexture(*texture_sheet);
	this->sheet.setPosition(x, y);
	if (this->sheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width) {
		this->sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->bounds.getGlobalBounds().width), static_cast<int>(this->sheet.getGlobalBounds().height)));
	}
	if (this->sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height) {
		this->sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->sheet.getGlobalBounds().width), static_cast<int>(this->bounds.getGlobalBounds().height)));
	}

	this->selector.setPosition(x, y);
	this->selector.setSize(sf::Vector2f(
		static_cast<float>(gridSize),
		static_cast<float>(gridSize))
		);
	this->selector.setFillColor(sf::Color::Transparent);
	this->selector.setOutlineThickness(3.f);
	this->selector.setOutlineColor(sf::Color::Red);

	this->textureRect.width = gridSize;
	this->textureRect.height = gridSize;
	this->textureRect.top = 0;
	this->textureRect.left = 0;

	this->hideBtn = new gui::Button(
		x - offset , y, 100.f, 50.f,
		&font, buttonText, 30,
		sf::Color(15, 15, 15, 255), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 255),
		sf::Color(100, 100, 100, 150), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 50));
}

gui::TextureSelector::~TextureSelector()
{
	delete this->hideBtn;
}

//Accesors
const bool& gui::TextureSelector::getActive()
{
	return this->active;
}

const sf::IntRect& gui::TextureSelector::getTextureRect() const
{
	return this->textureRect;
}


const bool gui::TextureSelector::GetKeytime()
{
	if (this->keytime >= this->keytimeMax) {
		this->keytime = 0.f;
		return true;
	}
	else return false;
}




//Functions

void gui::TextureSelector::updateKeytime(const float& dt)
{
	//useless for me 
	if (this->keytime < this->keytimeMax)
		this->keytime += dt;
}

void gui::TextureSelector::update(const sf::Vector2i& mousePosWindow, const float& dt)
{
	this->updateKeytime(dt);
	this->hideBtn->update(mousePosWindow);
	if (this->hideBtn->isReleased()) {
		if (this->hidden == true)
			this->hidden = false;
		else this->hidden = true;
	}

	if (this->hidden == false) {
		if (this->bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
			this->active = true;
		else this->active = false;

		if (this->active) {
			this->mousePosGrid.x = (mousePosWindow.x - static_cast<int>(this->bounds.getPosition().x)) / this->gridSize;
			this->mousePosGrid.y = (mousePosWindow.y - static_cast<int>(this->bounds.getPosition().y)) / this->gridSize;
			this->selector.setPosition(
				this->bounds.getPosition().x + this->mousePosGrid.x * this->gridSize,
				this->bounds.getPosition().y + this->mousePosGrid.y * this->gridSize
				);
		}
		//Update texture recatngle
		this->textureRect.left = static_cast<int>(this->selector.getPosition().x - this->bounds.getPosition().x);
		this->textureRect.top = static_cast<int>(this->selector.getPosition().y - this->bounds.getPosition().y);
	}
	
}

void gui::TextureSelector::render(sf::RenderTarget& target)
{
	

	if (this->hidden == false) {
		target.draw(this->bounds);
		target.draw(this->sheet);

		if (this->active)
			target.draw(this->selector);
	}
	this->hideBtn->render(target);
}
