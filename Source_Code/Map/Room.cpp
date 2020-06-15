#include "stdafx.h"
#include "Room.h"



void Room::updateEnemies(Player& player, const float& dt)
{
	
	for (auto enemy = enemies.begin(); enemy != enemies.end(); )
	{
		std::vector<Bullet>& bullets = (*enemy)->getBullets();
		for (auto bullet = bullets.begin(); bullet != bullets.end(); /*bullet++ */   ) {
			bullet->move(dt); //movement of the bullet

			//Damaging the player
			if (bullet->checkCollision(player.getGlobalBounds()) && player.isInvincible() == false) {

				player.looseHealth(bullet->getDamage());
				bullet = bullets.erase(bullet);
			}
			//Collision of enemy bullets with the map
			else if (this->map->checkCollision(bullet->getShape().getGlobalBounds()) == true) {
				bullet = bullets.erase(bullet);
			}
			else bullet++;

		}

		//Erasing bullets that are far from the enemy
		
		bullets.erase(
			std::remove_if(
				bullets.begin(),
				bullets.end(),
				[enemy](Bullet& b) { return b.DistanceLimit((*enemy)->getCenterPosition()); }
				),
			bullets.end()
			);

		(*enemy)->update(player.getCenterPosition(), dt); //Updating movement of the enemy
		if (player.checkCollision((*enemy)->getGlobalBounds()) == true) {
			//Dying from collison with enemies
			player.Die();
		}
		if ((*enemy)->isDead() == true) { //Enemy death
			this->statistics->logKill((*enemy)->getPoints(), (*enemy)->getType());
			this->createAmmo((*enemy)->getType(), (*enemy)->getCenterPosition());
			delete* enemy;
			enemy = enemies.erase(enemy);//erase return iterator to the last element 
		}
		else ++enemy;

	}
	

	if (this->spawnTime < this->spawnTimeMax) {
		this->spawnTime += dt;
	}

	this->currEnemies = this->enemies.size();

	if (this->allowSpawnEnemies == true
		&& this->currEnemies < this->enemyLimit
		&& this->spawnTime >= this->spawnTimeMax) {
		sf::Vector2f enemy_pos = this->map->randomFreeTile();

		if (player.getDistance(enemy_pos) > 700) { //enemies wont spawn closer than x pixels from player
			this->createRandomEnemy(enemy_pos);
		}
		this->spawnTime = 0.f;
	}
}

void Room::updatePowerUps(Player& player, const float& dt){

	for (auto powerUp = powerUps.begin(); powerUp != powerUps.end(); ) {
		(*powerUp)->update(dt);

		if ((*powerUp)->checkCollision(player.getGlobalBounds())) {
			player.recievePowerUp((*powerUp)->getPowerUpType());
			powerUp = powerUps.erase(powerUp);

		}
		else if ((*powerUp)->isActive() == false) {
			powerUp = powerUps.erase(powerUp);
		}
		else ++powerUp;
	}

	if (this->powerUpTime < this->powerUpTimeMax) {
		this->powerUpTime += dt;
	}
	if (this->allowSpawnPowerUps == true
		&& this->currPowerUps < this->powerUpLimit
		&& this->powerUpTime >= this->powerUpTimeMax) {

		this->createRandomPowerUp(this->map->randomFreeTile());
		this->powerUpTime = 0.f;
	}

}

void Room::updateColletables(Player& player, const float& dt)
{

	for (auto ammo = this->ammoPickUps.begin(); ammo != this->ammoPickUps.end(); ) {
		(*ammo)->update(player.getCenterPosition(), dt);

		if ((*ammo)->checkCollision(player.getGlobalBounds())) {
			player.recieveAmmo((*ammo)->getRefillPercent());
			ammo = this->ammoPickUps.erase(ammo);

		}
		else if ((*ammo)->isActive() == false) {
			ammo = this->ammoPickUps.erase(ammo);
		}
		else ++ammo;
	}
}

void Room::createRandomEnemy(const sf::Vector2f& enemy_pos)
{

	int type_num = RNG::get().randomI(0,100);

	if (type_num > 35) {
		enemies.push_back(new RegularEnemy(enemy_pos, *this->audio));
	}
	else if (type_num > 10) {
		enemies.push_back(new CommandoEnemy(enemy_pos, *this->audio));
	}

	else if (type_num > 2) {
		enemies.push_back(new StationaryEnemy(enemy_pos, *this->audio));
	}

	else if (type_num >= 0) {
		enemies.push_back(new BossEnemy(enemy_pos, *this->audio));
	}

	
}

void Room::createRandomPowerUp(const sf::Vector2f& position)
{
	powerUpType type = powerUpType::DEFAULT;
	int type_num = RNG::get().randomI(0,100);

	if (type_num > 70)
		type = powerUpType::FAST;
	else if (type_num > 50)
		type = powerUpType::THREE;
	else if (type_num > 30)
		type = powerUpType::ROUND;
	else if (type_num > 10)
		type = powerUpType::HEAVY;
	else if (type_num >= 0)
		type = powerUpType::RANDOM;


	powerUps.emplace_back( std::make_unique<PowerUp>(position, 3, type, Assets::Get().textures.at("POWER_UP")));
}

void Room::createAmmo(enemyType type, const sf::Vector2f& position)
{
	int quantity = 0;
	switch (type)
	{
	case enemyType::REGULAR:
		quantity = RNG::get().discreteI({ 20,5,1 });
		break;
	case enemyType::COMMANDO:
		quantity = RNG::get().discreteI({ 15,5,2,1 });
		break;
	case enemyType::STATIONARY:
		quantity = RNG::get().discreteI({ 0,5,3,2,1 });
		break;
	case enemyType::BOSS:
		quantity = RNG::get().discreteI({ 0,0,5,4,2,2,1 });
		break;
	default:
		break;
	}

	for (int i = 0; i < quantity; i++) {
		sf::Vector2f offset = sf::Vector2f(RNG::get().randomF(-50, 50), RNG::get().randomF(-50, 50));
		this->ammoPickUps.emplace_back(std::make_unique<Ammo>(position, position + offset));
	}
}

Room::Room( GameStatistics& statistics, AudioManager& audio, const std::string& type,
	const std::string& room_file, sf::Texture& tile_sheet,
	int offsetX, int offsetY)
	:tileSheet(tile_sheet), statistics(&statistics), tileMapType(type), offset(offsetX,offsetY), audio(&audio)
{
	this->map = std::make_unique<TileMap>(room_file, tileSheet, offsetX, offsetY);
	this->spawnTime = 0.f;
	this->powerUpTime = 0.f;

	this->allowSpawnEnemies = false;
	this->spawnTimeMax = 0;
	this->enemyLimit = 0;

	this->allowSpawnPowerUps = false;
	this->powerUpTimeMax = 0;
	this->powerUpLimit = 0;

	this->currEnemies = 0;
	this->currPowerUps = 0;
}

Room::Room(const std::string& type, const std::string& room_file, sf::Texture& tile_sheet)
	:tileSheet(tile_sheet), tileMapType(type), offset(0, 0), statistics(nullptr), audio(nullptr)
{
	this->map = std::make_unique<TileMap>(room_file, tileSheet, 0, 0);
	this->spawnTime = 0.f;
	this->powerUpTime = 0.f;

	this->allowSpawnEnemies = false;
	this->spawnTimeMax = 0;
	this->enemyLimit = 0;

	this->allowSpawnPowerUps = false;
	this->powerUpTimeMax = 0;
	this->powerUpLimit = 0;

	this->currEnemies = 0;
	this->currPowerUps = 0;
}

Room::~Room()
{
	util::purge(this->enemies);

}

//Accesors
const sf::IntRect Room::getTileBounds() const
{
	return this->map->getTileBounds();
}

const sf::Vector2i Room::getSizeInTiles() const
{
	return this->map->getWorldGridSize();
}

const sf::FloatRect Room::getBounds() const
{
	return this->map->getBounds();
}

const sf::Vector2f Room::randomFreeTile() const
{
	return this->map->randomFreeTile();
}

const bool Room::checkCollision(sf::FloatRect bounds) const
{
	return this->map->checkCollision(bounds);
}

const std::vector<Enemy*>& Room::getEnemies()
{
	//TO DO POTRZEBUJE REFERNCJI DO WROGÓW ALE NIE DA SIE DAC VECTORA ENTITY Z ENEMISAMI TERAZ TO WYCHOZDI POZA SCOPE TRZEB ALBO Z POINTERAMI POMYSLEC ALBO NIE UOGALNIAC DO ENTITY
	//tylko wtedy nie ma sensu, że shooting compenent jest dla entity a nie dla gracza i wroga

	//YAY SOLVED by passing sf::vector2f<Entity*>(enemis.begin, enemies.end) in player to shooting compenent function
	return this->enemies;
}

const std::string Room::getAsString() const
{
	std::stringstream ss;
	ss << this->tileMapType << " " << this->offset.x << " " << this->offset.y << " " << this->allowSpawnEnemies << " " << this->spawnTimeMax << " " << this->enemyLimit;
	return ss.str();
}

const EnemyInfo Room::getEnemyInfo() const
{
	return EnemyInfo(this->allowSpawnEnemies, this->spawnTimeMax, this->enemyLimit);
}

const std::string Room::getTileMapType() const
{
	return this->tileMapType;
}

void Room::setEnemies(bool enemy_spawn, float spawn_time, int enemy_limit)
{
	this->allowSpawnEnemies = enemy_spawn;
	this->spawnTimeMax = spawn_time;
	this->enemyLimit = enemy_limit;
}

void Room::setPowerUps(bool spawn_power_ups, float power_up_time_max, int power_up_limit)
{
	this->allowSpawnPowerUps = spawn_power_ups;
	this->powerUpTimeMax = power_up_time_max;
	this->powerUpLimit = power_up_limit;
}

void Room::spawnEnemies()
{
	while (this->enemies.size() < this->enemyLimit) {
		this->createRandomEnemy(this->map->randomFreeTile());
	}
}

void Room::spawnPowerUps()
{

	/*while (this->powerUps.size() < this->powerUpLimit) {
		this->createRandomPowerUp(this->map->randomFreeTile());

	}*/
}

void Room::updateCollision(Entity* entity, const float& dt)
{
	this->map->updateCollision(entity, dt);
}

void Room::update(Player* player, const float& dt)
{
	this->map->update();//nothing for now

	//Updating Enemies collison with the map
	for (Enemy* enemy : this->enemies) {
		this->map->updateCollision(enemy, dt);
	}

	//movement of bullets, bullets collision with player and map, destroying far away bullets
	//movement of the enemis, player collision with enemies, enemies death from player
	//Spawn of new enemies if it's enabled on this map
	this->updateEnemies(*player, dt);


	//Timer of power ups, collision with player, destroying old power ups
	//Spawing new if it;s allowed
	//this->updatePowerUps(*player, dt);

	//Collatebles (ammo) interacting with player, despawing after some time
	this->updateColletables(*player, dt);

	this->currEnemies = this->enemies.size();
	//this->currPowerUps = this->powerUps.size();
}


void Room::render(sf::RenderTarget& target, const sf::Vector2i& gridPosition) const
{
	this->map->render(target, gridPosition);
	/*for (const auto &power_up : this->powerUps) {
		power_up->render(target);
	}*/
	for (const auto& ammo : this->ammoPickUps) {
		ammo->render(target);
	}
	for (const Enemy* enemy : enemies) {
		enemy->render(target);
	}
}

void Room::renderDeffered(sf::RenderTarget& target) const
{
	this->map->renderDeffered(target);
}

void Room::fullRender(sf::RenderTarget& target) const
{
	this->map->fullRender(target);
}
