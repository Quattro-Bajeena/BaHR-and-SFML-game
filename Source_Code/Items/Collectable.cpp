#include "stdafx.h"
#include "Collectable.h"

Collectable::Collectable()
{
	this->magnet = false;
	this->active = true;
	this->timer = 0.f;
	this->timerMax = 0.f;
	this->type = collectableType::DEFAULT;
}

const bool Collectable::checkCollision(sf::FloatRect object) const
{
	if (this->sprite.getGlobalBounds().intersects(object)) {
		return true;
	}
	else return false;
}

const collectableType Collectable::getCollectableType() const
{
	return this->type;
}

const bool Collectable::isActive() const
{
	return this->active;
}
