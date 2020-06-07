#include "stdafx.h"
#include "Ammo.h"

void Ammo::startingAnimtion(const float& dt)
{
	float distance = util::distance(this->sprite.getPosition(), this->position);
	if (distance > 10.f) {
		this->sprite.move(util::normalize(this->position - this->sprite.getPosition()) * this->speed * 0.8f * dt);
	}
	else this->ready = true;
	
	
}

Ammo::Ammo(enemyType type, sf::Vector2f pos)
	:Collectable()
{
	this->type = collectableType::AMMO;
	this->sprite.setTexture(Assets::Get().textures.at("BULLET"));
	this->sprite.scale(3, 3);
	this->percent = 0.f;

	float scale = 1;
	switch (type)
	{
	case enemyType::REGULAR:
		this->percent = 0.2;
		scale = 1;
		break;
	case enemyType::COMMANDO:
		this->percent = 0.4;
		scale = 1.2;
		break;
	case enemyType::STATIONARY:
		this->percent = 0.6;
		scale = 1.5;
		break;
	case enemyType::BOSS:
		this->percent = 0.8;
		scale = 2;
		break;
	default:
		break;
	}

	this->sprite.scale(scale, scale);
	this->sprite.setPosition(pos);
	this->magnet = true;
	this->active = true;

	this->speed = 400.f;
	this->magnetThreshold = 200.f;

	this->timer = 0;
	this->timerMax = 10;
}

Ammo::Ammo(float percent, sf::Vector2f pos)
	:Collectable()
{
	this->type = collectableType::AMMO;
	this->sprite.setTexture(Assets::Get().textures.at("BULLET"));
	this->sprite.scale(3, 3);
	this->sprite.setPosition(pos);
	this->magnet = true;
	this->active = true;
	this->percent = percent;

	this->speed = 400.f;
	this->magnetThreshold = 200.f;

	this->timer = 0;
	this->timerMax = 10;
}

Ammo::Ammo(sf::Vector2f start_pos, sf::Vector2f end_pos)
	:Collectable()
{
	this->position = end_pos;
	this->type = collectableType::AMMO;
	this->sprite.setTexture(Assets::Get().textures.at("BULLET"));
	this->sprite.scale(3, 3);
	this->sprite.setPosition(start_pos);
	this->magnet = true;
	this->active = true;
	this->ready = false;

	this->percent = 0.2;

	this->speed = 400.f;
	this->magnetThreshold = 320.f;

	this->timer = 0;
	this->timerMax = 10;
}

const float Ammo::getRefillPercent() const
{
	return this->percent;
}

void Ammo::update(sf::Vector2f target, const float dt)
{
	this->timer += dt;
	if (this->ready == false) {
		this->startingAnimtion(dt);
	}
	if (this->magnet == true) {
		if (util::distance(target, this->sprite.getPosition()) < this->magnetThreshold) {
			this->sprite.move(util::normalize(target - this->sprite.getPosition()) * this->speed * dt);
		}
	}
	if (this->timer >= this->timerMax) {
		this->active = false;
	}
	
}

void Ammo::render(sf::RenderTarget& target) const
{
	target.draw(this->sprite);
}
