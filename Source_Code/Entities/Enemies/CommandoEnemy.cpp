#include "stdafx.h"
#include "CommandoEnemy.h"

CommandoEnemy::CommandoEnemy(sf::Vector2f position, AudioManager& audio)
	:Enemy(position, audio)
{
	this->type = enemyType::COMMANDO;

	this->sprite.scale(5, 5);
	
	this->shootTimerMax = 0.8f;
	this->canShoot = true;
	this->damage = 1;

	this->health = 70;
	this->maxHealth = this->health;
	this->points = 4;

	this->createAnimationComponent(Assets::Get().textures.at("COMMANDO_ENEMY"));
	this->animationComponent->addAnimation("WALK", 1, 0, 0, 1, 32, 32);

	this->createHitboxComponent(
		this->sprite.getGlobalBounds().width * 0.25, this->sprite.getGlobalBounds().height * 0.2,
		this->sprite.getGlobalBounds().width * 0.5, this->sprite.getGlobalBounds().height * 0.8);
	this->createMovementComponent(60, 0, 400, 150);
	this->createAIComponent(this->type, this->state);

	

}

void CommandoEnemy::shoot(sf::Vector2f targetPos)
{
	if (this->aiComponent->isAggro() && this->shootTimer >= this->shootTimerMax && this->canShoot == true) {
		int spray_angle = RNG::get().randomF(-5,5);
		sf::Vector2f spray = this->rotateVector(this->calculateDir(targetPos), spray_angle);
		bullets.emplace_back(Bullet(this->getCenterPosition(), spray, sf::Color::Green,
			this->damage, 15, 400, 1500));
		this->shootTimer = 0;
	}

}
	

void CommandoEnemy::updateAnimations(const float& dt)
{
	this->animationComponent->play("WALK", dt);
}
