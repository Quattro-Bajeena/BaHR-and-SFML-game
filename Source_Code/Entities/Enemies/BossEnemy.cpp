#include "stdafx.h"
#include "BossEnemy.h"

BossEnemy::BossEnemy(sf::Vector2f position, AudioManager& audio)
	:Enemy(position, audio)
{
	this->type = enemyType::BOSS;


	this->sprite.scale(9, 9);
	
	this->canShoot = true;
	this->damage = 6;
	this->shootTimerMax = 5;

	this->speed = 20;
	this->health = 1000;
	this->maxHealth = this->health;
	this->points = 10;


	this->createAnimationComponent(Assets::Get().textures.at("BOSS_ENEMY"));
	this->animationComponent->addAnimation("WALK", 1, 0, 0, 1, 32, 32);

	this->createHitboxComponent(
		this->sprite.getGlobalBounds().width * 0.3, this->sprite.getGlobalBounds().height * 0.05,
		this->sprite.getGlobalBounds().width * 0.4, this->sprite.getGlobalBounds().height * 0.9);
	this->createMovementComponent(20, 0, 160, 60);
	this->createAIComponent(this->type, this->state);

}

void BossEnemy::shoot(sf::Vector2f targetPos)
{
	if (this->aiComponent->isAggro() && this->shootTimer >= this->shootTimerMax && this->canShoot == true) {
		this->bullets.emplace_back(Bullet(this->getCenterPosition(), this->calculateDir(targetPos), sf::Color::Black,
			this->damage, 50, 300, 1000));
		this->shootTimer = 0.f;
	}
}

void BossEnemy::updateAnimations(const float& dt)
{
	this->animationComponent->play("WALK", dt);
}
