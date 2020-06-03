#include "stdafx.h"
#include "Entity.h"

void Entity::initVariables()
{
	this->hitboxComponent = nullptr;
	this->movementComponent = nullptr;
	this->animationComponent = nullptr;
}

Entity::Entity()
{
	this->initVariables();
	
}

Entity::~Entity()
{
	delete this->movementComponent;
	delete this->animationComponent;
	delete this->hitboxComponent;
}

//Comonent functions
void Entity::setTexture(sf::Texture& texture)
{
	this->sprite.setTexture(texture);
	
}

void Entity::createHitboxComponent(sf::Sprite& sprite,
	float offset_x, float offset_y,
	float width, float height)
{
	this->hitboxComponent = new HitboxComponent(sprite, offset_x, offset_y, width, height);
}

void Entity::createMovementComponent(const float maxVelocity, const float maxRollVelocity, const float acceleration, const float deceleration)
{
	this->movementComponent = new MovementComponent(this->sprite, maxVelocity,  maxRollVelocity, acceleration, deceleration);
}

void Entity::createAnimationComponent(sf::Texture& texture_sheet)
{
	this->animationComponent = new AnimationComponent(this->sprite, texture_sheet);
}

const sf::Vector2f Entity::getPosition() const
{
	if (this->hitboxComponent) {
		return this->hitboxComponent->getPosition();
	}
	else 
		return this->sprite.getPosition();
}

const sf::Vector2f Entity::getCenterPosition() const
{
	if (this->hitboxComponent) {
		return this->hitboxComponent->getCenterPosition();
	}
	else
		return sf::Vector2f(
			this->sprite.getPosition().x + this->sprite.getGlobalBounds().width,
			this->sprite.getPosition().y + this->sprite.getGlobalBounds().height
			);
}

const sf::Vector2i Entity::getGridPosition(const int gridSizeI) const
{
	if (this->hitboxComponent) {
		return sf::Vector2i(
			static_cast<int>(this->hitboxComponent->getPosition().x) / gridSizeI,
			static_cast<int>(this->hitboxComponent->getPosition().y) / gridSizeI
			);
	}
	else {
		return sf::Vector2i(
			static_cast<int>(this->sprite.getPosition().x) / gridSizeI,
			static_cast<int>(this->sprite.getPosition().y) / gridSizeI
			);
	}
		
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	if (this->hitboxComponent)
		return this->hitboxComponent->getGlobalBounds();
	else return this->sprite.getGlobalBounds();
}

const sf::FloatRect Entity::getNextPositionBounds(const float& dt) const
{
	if (this->hitboxComponent && this->movementComponent) {
		return this->hitboxComponent->getNextPosition(this->movementComponent->getVelocity(), dt);
	}
	return sf::FloatRect();
}



//Functions

void Entity::setPosition(const sf::Vector2f& pos)
{
	if (this->hitboxComponent)
		this->hitboxComponent->setPosition(pos.x, pos.y);
	else
		this->sprite.setPosition(pos);

}

void Entity::setPosition(const float x, const float y)
{
	if (this->hitboxComponent)
		this->hitboxComponent->setPosition(x, y);
	else
		this->sprite.setPosition(sf::Vector2f(x,y));
}


void Entity::move( const float dir_x, const float dir_y, const float& dt)
{
	if (this->movementComponent) {
		this->movementComponent->move(dir_x, dir_y,  dt); //sets velocity
	}
	
	
	
}

void Entity::stopVelocity()
{
	if (this->movementComponent)
		this->movementComponent->stopVelocity();
}

void Entity::stopVelocityX()
{
	if (this->movementComponent)
		this->movementComponent->stopVelocityX();
}

void Entity::stopVelocityY()
{
	if (this->movementComponent)
		this->movementComponent->stopVelocityY();
}

void Entity::update(const float& dt)
{
	/*if (this->movementComponent) {
		this->movementComponent->update(dt);
	}*/

}

void Entity::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
	if (this->hitboxComponent)
		this->hitboxComponent->render(target);

}
