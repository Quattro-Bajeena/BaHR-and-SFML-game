#include "stdafx.h"
#include "TextureSelector.h"

gui::TextureSelector::TextureSelector(float x, float y,
	float width, float height, sf::Vector2f scale,
	int grid_size,
	const sf::Texture* texture_sheet,
	sf::Font& font, std::string buttonText)
	:orgScale(scale)
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
	this->bounds.setOutlineThickness(static_cast<int>(2.f*scale.x));
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
	this->selector.setOutlineThickness(static_cast<int>(3.f * scale.x));
	this->selector.setOutlineColor(sf::Color::Red);

	this->textureRect.width = gridSize;
	this->textureRect.height = gridSize;
	this->textureRect.top = 0;
	this->textureRect.left = 0;

	this->hideBtn = new gui::Button(
		x - offset, y, 100.f, 50.f, scale,
		&font, buttonText, 30,
		sf::Color(15, 15, 15, 255), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 255),
		sf::Color(140, 140, 140, 190), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 50));

	this->scale(scale);
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

void gui::TextureSelector::scale(const sf::Vector2f& scale)
{
	

	this->hideBtn->scale(scale);
	this->bounds.setPosition(this->bounds.getPosition().x * scale.x, this->bounds.getPosition().y * scale.y);
	this->bounds.scale(scale);

	this->selector.setPosition(this->selector.getPosition().x * scale.x, this->selector.getPosition().y * scale.y);
	this->selector.scale(scale);

	this->sheet.setPosition(this->sheet.getPosition().x * scale.x, this->sheet.getPosition().y * scale.y);
	this->sheet.scale(scale);

	//this->textureRect.width = this->gridSize * scale.x;
	//this->textureRect.height = this->gridSize * scale.y;

	this->gridSize *= scale.x;
}




//Functions

void gui::TextureSelector::switchHidden()
{
	if (this->hidden == true)
	{
		this->hidden = false;
	}
	else {
		this->hidden = true;
		this->active = false;
	}
}

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
		this->textureRect.left = static_cast<int>((this->selector.getPosition().x - this->bounds.getPosition().x) / this->orgScale.x);
		this->textureRect.top = static_cast<int>((this->selector.getPosition().y - this->bounds.getPosition().y) / this->orgScale.x);
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