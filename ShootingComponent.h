#pragma once
#include "Guns.h"
#include "Entity.h"


class Entity;
class Enemy;
class Player;

class ShootingComponent
{
private:
	sf::Text debugText;

	std::vector<std::unique_ptr<Gun>> guns;
	Gun* currentGun;
	int currentGunPos;
	float swapTime;
	float swapTimeMax;

	std::vector<Bullet> bullets;

	//Functions
	std::unique_ptr<Gun> createGun(gunModels model);

public:
	ShootingComponent(std::unique_ptr<Gun> default_gun);
	~ShootingComponent();


	//Accesors
	std::vector<Bullet>* const getBullets();
	const Magazine::MagazineInfo getMagazineInfo() const;
	const gunModels getCurrentGunModel() const;
	const bool canReload() const;
	const float getReloadTime() const;

	//Functions
	void shoot();
	void reload();
	void changeCurrGunAdjacent(int direction);
	void changeCurrGunPos(int pos);
	void refillAmmo(float percent);
	void addGun(gunModels model);
	void updateBullets(const std::vector<Entity*>& enemies, sf::Vector2f center_pos, const float& dt);;

	void updateGun(sf::Vector2f start_pos, sf::Vector2f dir, const float& dt);
	void render(sf::RenderTarget& target);
	void update(sf::Vector2f curr_pos, const float& dt);
};


