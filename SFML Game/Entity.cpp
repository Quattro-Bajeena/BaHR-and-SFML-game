#include "stdafx.h"
#include "Entity.h"

void Entity::initVariables()
{
	this->hitboxComponent = nullptr;
	this->movementComponent = nullptr;
	this->animationComponent = nullptr;
	this->aiComponent = nullptr;

	this->invincibility = false;
	this->invincibilityTimer = 0.f;
	this->invincibilityTimerMax = 0.5f;
}

Entity::Entity()
{
	this->initVariables();

}

Entity::~Entity()
{
	
}

//Comonent functions
void Entity::setTexture(sf::Texture& texture)
{
	this->sprite.setTexture(texture);

}

void Entity::createHitboxComponent(
	float offset_x, float offset_y,
	float width, float height)
{
	this->hitboxComponent = std::make_unique< HitboxComponent>(this->sprite, offset_x, offset_y, width, height);
}

void Entity::createMovementComponent(const float maxVelocity, const float maxRollVelocity, const float acceleration, const float deceleration)
{
	this->movementComponent = std::make_unique< MovementComponent>(this->sprite, maxVelocity, maxRollVelocity, acceleration, deceleration);
}

void Entity::createAnimationComponent(sf::Texture& texture_sheet)
{
	this->animationComponent = std::make_unique < AnimationComponent>(this->sprite, texture_sheet);
}

void Entity::createAIComponent(enemyType type, enemyState& state)
{
	this->aiComponent = std::make_unique < AIComponent>(this->sprite, type, state);
}

void Entity::createShootingComponent(std::unique_ptr<Gun> default_gun)
{
	this->shootingComponent = std::make_unique<ShootingComponent>(std::move(default_gun));
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

const float Entity::getDistance(const sf::Vector2f& position) const
{
	if (this->hitboxComponent) {
		return this->hitboxComponent->getDistance(position);
	}
	else {
		sf::Vector2f vector = position - this->getCenterPosition();
		float distance = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
		return distance;
	}
	
}



//Gameplay Accesors
std::vector<Bullet>* const Entity::getBullets()
{
	if (this->shootingComponent) {
		return this->shootingComponent->getBullets();
	}
	else return nullptr;
}

const int Entity::getHealth() const
{
	return this->health;
}

const int Entity::getMaxHealth() const
{
	return this->healthMax;
}


const bool Entity::isDead() const
{
	return this->dead;
}

const bool Entity::isInvincible() const
{
	return this->invincibility;
}

const float Entity::getReloadTime() const
{
	if (this->shootingComponent)
		return this->shootingComponent->getReloadTime();
	else return INFINITY;
}

const bool Entity::canReload() const
{
	if (this->shootingComponent)
		return this->shootingComponent->canReload();
	else return false;
}

const Magazine::MagazineInfo Entity::getMagazineInfo() const
{
	if(this->shootingComponent)
		return this->shootingComponent->getMagazineInfo();
}

const gunModels Entity::getCurrentGunModel() const
{
	if(this->shootingComponent)
		return this->shootingComponent->getCurrentGunModel();
}

//Functions
const bool Entity::checkCollision(sf::FloatRect object)const
{
	if (this->hitboxComponent) {
		return this->hitboxComponent->checkCollision(object);
	}
	else {
		if (object.intersects(this->getGlobalBounds()))
			return true;
		else return false;
	}
}

const bool Entity::checkContain(sf::FloatRect object)const
{
	if (this->hitboxComponent) {
		return this->hitboxComponent->checkContain(object);
	}
	else {
		if (object.contains(this->getCenterPosition()))
			return true;
		else return false;
	}
	
}

//Functions

void Entity::setPosition(const sf::Vector2f& pos)
{
	if (this->hitboxComponent)
		this->hitboxComponent->setPosition(pos.x, pos.y);
	else
		this->sprite.setPosition(pos);

}

void Entity::setPosition(const float& x, const float& y)
{
	if (this->hitboxComponent)
		this->hitboxComponent->setPosition(x, y);
	else
		this->sprite.setPosition(x,y);
}
//Calculations
sf::Vector2f Entity::calculateDir(const sf::Vector2f& position)
{
	sf::Vector2f aimDir = (position - this->getCenterPosition());
	return util::normalize(aimDir);

}

sf::Vector2f Entity::rotateVector(const sf::Vector2f& org, const float rotation)
{
	sf::Vector2f rotated;
	double rad = (M_PI / 180) * rotation;
	rotated.x = cos(rad) * org.x - sin(rad) * org.y;
	rotated.y = sin(rad) * org.x + cos(rad) * org.y;
	//Algebra is back in town....Rotator Givensa bejbeeeee!!!!!
	return util::normalize(rotated);
}


void Entity::Die()
{
	this->looseHealth(this->health);
}

void Entity::looseHealth(int damage)
{
	if (this->invincibility == false) {
		this->health -= damage;
		this->hitSound.play();
	}
}

void Entity::move(const float dir_x, const float dir_y, const float& dt)
{
	if (this->movementComponent) {
		this->movementComponent->move(dir_x, dir_y, dt); //sets velocity
	}
	else this->sprite.move(dir_x * dt, dir_y * dt);



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

void Entity::render(sf::RenderTarget& target) const
{
	target.draw(this->sprite);
	if (this->hitboxComponent)
		this->hitboxComponent->render(target);

}

