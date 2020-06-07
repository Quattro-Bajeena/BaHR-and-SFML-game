
#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(sf::Vector2f startPos, sf::Vector2f dir,
	sf::Color color, float damage,
	float radius, float max_speed, float max_distance)

	:direction(dir), maxSpeed(max_speed), maxDistance(max_distance),
	damage(damage)
{
	this->shape.setPosition(startPos);
	this->shape.setRadius(radius);
	this->shape.setFillColor(color);
	this->currentVelocity = this->direction * this->maxSpeed;

}

void Bullet::move(const float dt)
{
	this->shape.move(this->currentVelocity * dt);
}

const bool Bullet::checkCollision(sf::FloatRect object) const
{
	if (object.intersects(this->shape.getGlobalBounds()))
		return true;
	else return false;
}

const bool Bullet::DistanceLimit(const sf::Vector2f& position) const
{
	sf::Vector2f vector = position - this->shape.getPosition();
	float distance = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	if (distance > this->maxDistance) return true;
	else return false;
}

const sf::CircleShape Bullet::getShape() const
{
	return this->shape;
}

const int Bullet::getDamage() const
{
	return this->damage;
}


