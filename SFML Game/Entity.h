#ifndef ENTITY_H
#define ENTITY_H
#pragma once

#include "HitboxComponent.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"
#include "ShootingComponent.h"
#include "AIComponent.h"
#include "Assets.h"

class HitboxComponent;
class MovementComponent;
class AnimationComponent;
class AIComponent;
class ShootingComponent;


class Entity
{
private:
	void initVariables();

protected:
	//Assets
	sf::Sprite sprite;
	//Sounds
	sf::Sound hitSound;

	std::unique_ptr<HitboxComponent> hitboxComponent;
	std::unique_ptr<MovementComponent> movementComponent;
	std::unique_ptr<AnimationComponent> animationComponent;
	std::unique_ptr<AIComponent> aiComponent;
	std::unique_ptr<ShootingComponent> shootingComponent;

	//Gameplay Variables
	//Life
	bool invincibility;
	float invincibilityTimer;
	float invincibilityTimerMax;

	int healthMax;
	int health;
	bool dead;

	//Component functions
	void setTexture(sf::Texture& texture);
	void createHitboxComponent(
		float offset_x, float offset_y,
		float width, float height);
	void createMovementComponent(const float maxVelocity, const float maxRollVelocity, const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void createAIComponent(enemyType type, enemyState& state);
	void createShootingComponent(std::unique_ptr<Gun> default_gun);

	//Calculating
	sf::Vector2f calculateDir(const sf::Vector2f& position);
	sf::Vector2f rotateVector(const sf::Vector2f& org, const float rotation);
public:

	Entity();
	virtual ~Entity();

	//Accesors
	virtual const sf::Vector2f getPosition() const;
	virtual const sf::Vector2f getCenterPosition() const;
	virtual const sf::Vector2i getGridPosition(const int gridSizeI) const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const sf::FloatRect getNextPositionBounds(const float& dt) const;

	const bool checkCollision(sf::FloatRect object) const;
	const bool checkContain(sf::FloatRect object) const;
	const float getDistance(const sf::Vector2f& position) const;

	

	//Gameplay accesors
	std::vector<Bullet>* const getBullets();
	const int getHealth() const;
	const int getMaxHealth() const;
	const bool isDead() const;
	const bool isInvincible() const;

	const float getReloadTime() const;
	const bool canReload() const;
	const Magazine::MagazineInfo getMagazineInfo() const;
	const gunModels getCurrentGunModel() const;

	//Mopdifers
	virtual void setPosition(const sf::Vector2f& pos);
	virtual void setPosition(const float&x, const float& y);

	//Functions
	//Gameplay
	virtual void Die();
	virtual void looseHealth(int damage);
	virtual void move(const float dir_x, const float dir_y, const float& dt);

	virtual void stopVelocity();
	virtual void stopVelocityX();
	virtual void stopVelocityY();

	virtual void update(const float& dt);
	virtual void render(sf::RenderTarget& target) const= 0;
};

#endif // !ENTITY_H

