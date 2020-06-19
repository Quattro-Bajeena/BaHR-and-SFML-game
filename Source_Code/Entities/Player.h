#pragma once

#include "Enemy.h"
#include "Entity.h"
#include "Guns.h"
#include "Assets.h"

class Assets;

class Player :
	public Entity{
private:
	//Assets
	sf::Text debugText;
	//Variables
	std::string name;

	//Power ups
	//Movement
	sf::Vector2f startingPos;
	bool rolling;
	bool rollingUp;
	float rollTimer;

	//Life
	//int health;
	//bool dead;

	//Initializers
	void initVariables();

	//internal Functions	
	void updateAnimations(const float& dt);
	
public:
	Player(std::string name, const sf::Vector2f& pos, AudioManager& audio);
	~Player();

	//Functions
	void roll(const float& dt);
	void endRoll();
	void shoot(sf::Vector2f mousePos);
	void reload();
	void recievePowerUp(powerUpType type);
	void recieveAmmo(float percent);
	void recieveGun(gunModels model);
	void changeCurrGunAdjacent(int direction);
	void changeCurrGunPos(int pos);

	void looseHealth(int damage) override;
	void looseHealthInv(int damage) override;
	//Accesors
	const std::string getName() const;
	

	//Main functions
	void update(const float& dt);
	void updateBullets(const std::vector<Enemy*>& enemies, const float dt);
	void updateWeapon(const sf::Vector2f& mousePos, const float& dt);
	void render(sf::RenderTarget& target) const;
};

