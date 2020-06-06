#include "stdafx.h"
#include "RegularEnemy.h"

RegularEnemy::RegularEnemy(sf::Vector2f position)
	:Enemy(position)
{
	this->type = enemyType::REGULAR;
	this->shootTimerMax = 0;
	this->canShoot = false;
	this->damage = 0;

	this->health = 100;
	this->maxHealth = this->health;
	this->points = 2;
	
	
	this->sprite.scale(6, 6);

	this->createAnimationComponent(Assets::Get().textures.at("REGULAR_ENEMY"));
	
	this->animationComponent->addAnimation("WALK_FORWARD", 1, 0, 0, 1, 64, 32);
	this->animationComponent->addAnimation("WALK_LEFT", 1, 1, 0, 1, 64, 32);
	this->animationComponent->addAnimation("WALK_RIGHT", 1, 2, 0, 1, 64, 32);

	this->createHitboxComponent(0, 0, this->sprite.getGlobalBounds().width, this->sprite.getGlobalBounds().height);
	this->createMovementComponent(50, 0, 400, 150);
	this->createAIComponent(this->type, this->state);

	this->hitboxComponent->addHitboxPreset("SIDE",
		this->sprite.getGlobalBounds().width * 0.15, this->sprite.getGlobalBounds().height * 0.4,
		this->sprite.getGlobalBounds().width * 0.7, this->sprite.getGlobalBounds().height * 0.6);

	this->hitboxComponent->addHitboxPreset("FRONT",
		this->sprite.getGlobalBounds().width * 0.35, this->sprite.getGlobalBounds().height * 0.4,
		this->sprite.getGlobalBounds().width * 0.3, this->sprite.getGlobalBounds().height*0.6);

	this->hitboxComponent->setHitbox("SIDE");
	
}

void RegularEnemy::shoot(sf::Vector2f targetPos)
{
}

void RegularEnemy::updateAnimations(const float& dt)
{
	if (this->animationSwitchTimer >= this->animationSwitchTimerMax) {
		if (this->movementComponent->getState(movement_states::IDLE) == true) {
			this->animationComponent->play("WALK_FORWARD", dt);
			this->hitboxComponent->setHitbox("FRONT");
		}
		else if (this->movementComponent->getState(movement_states::MOVING_UP) == true) {
			this->animationComponent->play("WALK_FORWARD", dt);
			this->hitboxComponent->setHitbox("FRONT");
		}
		else if (this->movementComponent->getState(movement_states::MOVING_DOWN) == true) {
			this->animationComponent->play("WALK_FORWARD", dt);
			this->hitboxComponent->setHitbox("FRONT");
		}
		else if (this->movementComponent->getState(movement_states::MOVING_LEFT) == true) {
			this->animationComponent->play("WALK_LEFT", dt);
			this->hitboxComponent->setHitbox("SIDE");
		}
		else if (this->movementComponent->getState(movement_states::MOVING_RIGHT) == true) {
			this->animationComponent->play("WALK_RIGHT", dt);
			this->hitboxComponent->setHitbox("SIDE");
		}
		this->animationSwitchTimer = 0.f;
	}
	
}
