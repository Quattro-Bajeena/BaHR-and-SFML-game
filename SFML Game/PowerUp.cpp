
#include "stdafx.h"
#include "PowerUp.h"

PowerUp::PowerUp(sf::Vector2f position, float size, powerUpType type, const sf::Texture& texture)
	:texture(texture), type(type)
{
	this->shape.setPosition(position);
	this->shape.setSize(sf::Vector2f(50.f, 50.f));
	this->shape.scale(size, size);


	this->shape.setTexture(&texture);


	this->text.setFont(Assets::Get().font);
	this->text.setCharacterSize(50);
	this->text.setPosition(this->shape.getPosition());
	this->text.setFillColor(sf::Color::Black);

	this->timer = 0.f;
	this->timerMax = 10.f;

	active = true;
	
	switch (type) {
	case powerUpType::FAST:
		this->text.setString("F");
		this->rect = sf::IntRect(0, 0, 50, 50);
		this->shape.setTextureRect(this->rect);
		break;

	case powerUpType::THREE:
		this->text.setString("3");
		this->rect = sf::IntRect(50, 0, 50, 50);
		this->shape.setTextureRect(this->rect);
		break;

	case powerUpType::ROUND:
		this->text.setString("O");
		this->rect = sf::IntRect(100, 0, 50, 50);
		this->shape.setTextureRect(this->rect);
		break;

	case powerUpType::HEAVY:
		this->text.setString("H");
		this->rect = sf::IntRect(0, 50, 50, 50);
		this->shape.setTextureRect(this->rect);
		break;

	case powerUpType::RANDOM:
		this->text.setString("*");
		this->rect = sf::IntRect(50, 50, 50, 50);
		this->shape.setTextureRect(this->rect);
		break;
	case powerUpType::DEFAULT:
		std::cout << "error in powerup constructor type is default" << "\n";
		break;
	}

}

PowerUp::~PowerUp()
{
}

void PowerUp::update(const float dt)
{
	this->timer += dt;
	if (this->timer >= this->timerMax) {
		this->active = false;
	}
}

void PowerUp::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	//target.draw(this->text);
}

const bool PowerUp::checkCollision(sf::FloatRect object) const
{
	if (object.intersects(this->shape.getGlobalBounds()))
		return true;
	else return false;
}

const powerUpType PowerUp::getPowerUpType() const
{
	return this->type;
}

const bool PowerUp::isActive() const
{
	return this->active;
}
