#include "stdafx.h"
#include "StationaryEnemy.h"

StationaryEnemy::StationaryEnemy(sf::Vector2f position, AudioManager& audio)
	:Enemy(position, audio)
{
	this->type = enemyType::STATIONARY;


	this->sprite.scale(6, 6);

	this->canShoot = true;
	this->damage = 2;
	this->shootTimerMax = 1.5f;
	this->speed = 0;
	this->health = 500;
	this->maxHealth = this->health;
	this->points  = 8;


	this->createAnimationComponent(Assets::Get().textures.at("STATIONARY_ENEMY"));
	this->animationComponent->addAnimation("IDLE", 1, 0, 0, 1, 32, 32);

	this->createHitboxComponent(
		this->sprite.getGlobalBounds().width * 0.15, this->sprite.getGlobalBounds().height * 0.4,
		this->sprite.getGlobalBounds().width * 0.7, this->sprite.getGlobalBounds().height * 0.6);
	this->createMovementComponent(0, 0, 0, 0);
	this->createAIComponent(this->type, this->state);
}

void StationaryEnemy::shoot(sf::Vector2f targetPos)
{
	if (this->aiComponent->isAggro() && this->shootTimer >= this->shootTimerMax && this->canShoot == true) {
		bullets.emplace_back(Bullet(this->getCenterPosition(), this->calculateDir(targetPos), sf::Color::Yellow,
			this->damage, 30, 250, 2000));
		this->shootTimer = 0.f;
	}
}

void StationaryEnemy::updateAnimations(const float& dt)
{
	this->animationComponent->play("IDLE", dt);
}
