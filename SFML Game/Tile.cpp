#include "stdafx.h"
#include "Tile.h"

Tile::Tile()
{
	this->collision = false;
	this->type = 0;
}

Tile::Tile(int grid_x, int grid_y, float gridSizeF, const sf::Texture& texture, const sf::IntRect& rect,
	bool collision, short int type)
{
	this->shape.setSize(sf::Vector2f(gridSizeF, gridSizeF));
	this->shape.setFillColor(sf::Color::White);

	this->shape.setPosition(grid_x * gridSizeF, grid_y * gridSizeF);
	this->shape.setTexture(&texture);
	this->shape.setTextureRect(rect);

	this->collision = collision;
	this->type = type;


	if (this->collision) {
		this->shape.setOutlineColor(sf::Color::Red);
		this->shape.setFillColor(sf::Color(255, 200, 200, 255));
		this->shape.setOutlineThickness(-2.f);
	}



}

Tile::~Tile()
{
}


//Accesors

const sf::Vector2f& Tile::getPosition() const
{
	return this->shape.getPosition();
}

const bool& Tile::getCollision() const
{
	return this->collision;
}

const sf::FloatRect Tile::getGlobalBounds() const
{
	return this->shape.getGlobalBounds();
}

const short& Tile::getType() const
{
	return this->type;
}



const std::string Tile::getAsString() const
{
	std::stringstream ss;
	ss << this->shape.getTextureRect().left << " " << this->shape.getTextureRect().top << " " << this->collision << " " << this->type;

	return ss.str();
}


//Functions
const bool Tile::intersects(const sf::FloatRect bounds) const
{
	return this->shape.getGlobalBounds().intersects(bounds);
}

void Tile::update()
{

}

void Tile::render(sf::RenderTarget& target) const
{
	target.draw(this->shape);
}