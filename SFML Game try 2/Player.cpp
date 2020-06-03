#include "..\SFML Game\Player.h"
#include "stdafx.h"
#include "Player.h"


//Initlizer functions
void Player::initVariables()
{
	this-> rolling = false;
	this->rollingUp = false;
	this->rollTimer = 1.f;
}

void Player::initComponents()
{
	

}


//Constuctors / Destructors
Player::Player(float x, float y, sf::Texture& texture_sheet)
{
	this->initVariables();
	this->initComponents();

	this->setPosition(x, y);

	this->createHitboxComponent( this->sprite, 50.f, 18.f, 90.f, 180.f);
	this->createMovementComponent(500.f, 1200.f, 4800.f, 1500.f);
	this->createAnimationComponent(texture_sheet);

	this->animationComponent->addAnimation("IDLE", 3.f, 0, 4, 3, 4, 192, 192);
	this->animationComponent->addAnimation("WALK_STRAIGHT", 12.f, 0, 1, 12, 1, 192, 192);
	this->animationComponent->addAnimation("WALK_LEFT", 12.f, 0, 3, 5, 3, 192, 192);
	this->animationComponent->addAnimation("WALK_RIGHT", 12.f, 0, 2, 5, 2, 192, 192);
	this->animationComponent->addAnimation("ROLL", 12.f, 0, 5, 7, 5, 192, 192);
	this->animationComponent->addAnimation("ROLL_BACK", 12.f, 0, 6, 7, 6, 192, 192);
}

Player::~Player()
{
}

//Functions

void Player::roll()
{
	if (this->rolling == false && this->rollTimer >= 1 && this->movementComponent->getState(movement_states::IDLE) == false) {
		this->rolling = true;
		this->rollTimer = 0;
		this->movementComponent->startRoll();
		this->hitboxComponent->changeHitbox(60.f,100.f,70.f,110.f );

		if (this->movementComponent->getState(movement_states::MOVING_UP) && rollingUp == false) {
			rollingUp = true;
		}
		else rollingUp = false;
	}
	
}

void Player::updateAnimations(const float& dt) //and movement/hitbox for rolling
{
	if (this->rolling) {
		
		if (rollingUp == true) {
			if (this->animationComponent->play("ROLL_BACK", dt, 1, 1, true)) {
				this->rolling = false;
				this->rollingUp = false;
				this->movementComponent->endRoll();
				this->hitboxComponent->reset();
			}
		}
		else {
			if (this->animationComponent->play("ROLL", dt, 1, 1, true)) {
				this->rolling = false;
				this->movementComponent->endRoll();
				this->hitboxComponent->reset();
			}
		}
		
		if (this->movementComponent->getState(movement_states::IDLE)) {
			this->movementComponent->endRoll();
			this->animationComponent->resetPriority();
			//this->animationComponent->play("IDLE", dt);
		}
	}

	else if (this->movementComponent->getState(movement_states::IDLE))
		this->animationComponent->play("IDLE", dt);
	else if (this->movementComponent->getState(movement_states::MOVING_UP))
		this->animationComponent->play("WALK_STRAIGHT", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	else if (this->movementComponent->getState(movement_states::MOVING_DOWN))
		this->animationComponent->play("WALK_STRAIGHT", dt, this->movementComponent->getVelocity().y, this->movementComponent->getMaxVelocity());
	else if (this->movementComponent->getState(movement_states::MOVING_LEFT))
		this->animationComponent->play("WALK_LEFT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
	else if (this->movementComponent->getState(movement_states::MOVING_RIGHT))
		this->animationComponent->play("WALK_RIGHT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());
}


void Player::update(const float& dt)
{
	if (rolling == false)
		rollTimer += dt;

	Entity::update(dt);
	this->movementComponent->update(dt);
	this->hitboxComponent->update();
	this->updateAnimations(dt);
}



void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
	this->hitboxComponent->render(target);
}
