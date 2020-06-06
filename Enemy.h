#pragma once
#include "Bullet.h"
#include "Entity.h"

//enum class enemyState { FULL = 0, SCRATCHED, DAMAGED, GRAVE, DEAD };
//enum class enemyType { REGULAR = 0, COMMANDO, STATIONARY, BOSS };
class Entity;
class Bullet;
class ShootingComponent;

class Enemy :
	public Entity{
protected:
	std::vector<Bullet> bullets;

	enemyType type;
	enemyState state;


	int maxHealth;

	float size;
	float speed;
	int points;
	int damage;
	bool canShoot;

	float shootTimer;
	float shootTimerMax;
	float animationSwitchTimer;
	float animationSwitchTimerMax;
	
	sf::Text text;

	//Initializers
	void initVariables();

	//Functions
	void updateState();
	void updateColor();

public:
	Enemy(sf::Vector2f position);
		
	//Accesors
	bool isDead();

	//Function
	void move(const sf::Vector2f& target, const float& dt);
	virtual void shoot(sf::Vector2f targetPos);
	void updateBullets(const float dt);
	virtual void updateAnimations(const float& dt);
	void update(const sf::Vector2f& target, const float& dt);
	void render(sf::RenderTarget& target) const;

	//Accesors
	const int getPoints()const;
	const enemyType getType() const;
	std::vector<Bullet>& getBullets();

};

