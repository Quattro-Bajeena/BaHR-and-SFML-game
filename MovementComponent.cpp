#include "stdafx.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent(sf::Sprite& sprite,
	float maxVelocity, float maxRollVelocity, float acceleration, float deceleration) :
	sprite(sprite), maxVelocity(maxVelocity), maxRollVelocity(maxRollVelocity),
	acceleration(acceleration), deceleration(deceleration), rolling(false), maxDefaultVelocity(maxVelocity)
{
	this->maxVelocityMultiplier = this->maxRollVelocity / this->maxDefaultVelocity;
}

MovementComponent::~MovementComponent()
{
}

//Accesors 

const float MovementComponent::getMaxVelocity() const
{
	return this->maxVelocity;
}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return this->velocity;
}


//Functions

const bool MovementComponent::getState(const movement_states state) const
{

	switch (state) {
	case movement_states::IDLE:
		return (this->velocity.x == 0.f && this->velocity.y == 0.f);
		break;
	case movement_states::MOVING:
		return (this->velocity.x == 0.f || this->velocity.y == 0.f);
		break;
	case movement_states::MOVING_LEFT:
		return (this->velocity.x < 0.f && abs(this->velocity.x) > abs(this->velocity.y));
		break;
	case movement_states::MOVING_RIGHT:
		return (this->velocity.x > 0.f && abs(this->velocity.x) > abs(this->velocity.y));
		break;
	case movement_states::MOVING_UP:
		return (this->velocity.y < 0.f && abs(this->velocity.y) >= abs(this->velocity.x) );
		break;
	case movement_states::MOVING_DOWN:
		return (this->velocity.y > 0.f && abs(this->velocity.y) >= abs(this->velocity.x));
		break;

	}
	return false;
}

void MovementComponent::stopVelocity()
{
	//resert velocity to 0
	this->velocity = sf::Vector2f(0, 0);
}

void MovementComponent::stopVelocityX()
{
	this->velocity.x = 0;
}

void MovementComponent::stopVelocityY()
{
	this->velocity.y = 0;
}

void MovementComponent::startRoll()
{
	this->rolling = true;
	this->maxVelocity = this->maxRollVelocity;
	this->velocity.x = util::sgn(this->velocity.x) * this->maxRollVelocity;
	this->velocity.y = util::sgn(this->velocity.y) * this->maxRollVelocity;
}

void MovementComponent::endRoll()
{
	this->rolling = false;
	this->maxVelocity = this->maxDefaultVelocity;
}


void MovementComponent::move(const float dir_x, const float dir_y, const float& dt)
{
	//accelerating a spirt unity itreachis max velocity

	//Acceleration
	if (this->rolling == false) {
		this->velocity.x += this->acceleration * dir_x * dt;
		this->velocity.y += this->acceleration * dir_y * dt;
	}
	else { //character is less responsive when rolling


		this->velocity.x += (util::sgn(this->velocity.x) == util::sgn(dir_x)) ? this->acceleration * dir_x * dt * (1 / this->maxVelocityMultiplier) : 0;
		this->velocity.y += (util::sgn(this->velocity.y) == util::sgn(dir_y)) ? this->acceleration * dir_y * dt * (1 / this->maxVelocityMultiplier) : 0;
	}


}


void MovementComponent::update(const float& dt)
{
	//Decelareate the sprite and controls the amximum velocity moves the sprite
	if (this->velocity.x > 0.f) { //checkh for positive x

		//deceleration x positive
		this->velocity.x -= this->deceleration * dt;
		if (this->velocity.x < 0.f)
			this->velocity.x = 0.f;

		//max velocity check
		if (this->velocity.x > this->maxVelocity)
			this->velocity.x = maxVelocity;
	}
	else if (this->velocity.x < 0.f) { //chechk for negative x

		//decelaretion x negative
		this->velocity.x += deceleration * dt;
		if (this->velocity.x > 0.f)
			this->velocity.x = 0.f;

		//max velocity chechk x negative
		if (this->velocity.x < -this->maxVelocity)
			this->velocity.x = -this->maxVelocity;
	}

	if (this->velocity.y > 0.f) { //checkh for positive y
		//deceleration y positive
		this->velocity.y -= this->deceleration * dt;
		if (this->velocity.y < 0.f)
			this->velocity.y = 0.f;

		//max velocity check
		if (this->velocity.y > this->maxVelocity)
			this->velocity.y = maxVelocity;
	}
	else if (this->velocity.y < 0.f) { //chechk for negative y
		//decelaretion y negative
		this->velocity.y += deceleration * dt;
		if (this->velocity.y > 0.f)
			this->velocity.y = 0.f;

		//max velocity chechk y negative
		if (this->velocity.y < -this->maxVelocity)
			this->velocity.y = -this->maxVelocity;
	}

	//std::cout << this->velocity.x << "|" << this->velocity.y << " | " << this->maxVelocity << "\n";

	//final move
	this->sprite.move(this->velocity * dt); //uses velocity



}

