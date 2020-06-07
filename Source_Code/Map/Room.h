#pragma once
#include "TileMap.h"
#include "Player.h"
#include "PowerUp.h"
#include "State.h"
#include "Enemies.h"
#include "Ammo.h"


class Player;
class Enemy;
class Entity;
class GameStatistics;

class Room
{
private:
	sf::Texture& tileSheet;
	GameStatistics* const statistics;
	const std::string tileMapType;
	
	std::vector<Enemy*> enemies;
	std::vector<std::unique_ptr<PowerUp>> powerUps;
	std::vector<std::unique_ptr<Ammo>> ammoPickUps;
	std::unique_ptr<TileMap> map;

	sf::Vector2f offset;

	bool allowSpawnEnemies;
	int enemyLimit;
	int currEnemies;
	float spawnTime;
	float spawnTimeMax;

	bool allowSpawnPowerUps;
	int powerUpLimit;
	int currPowerUps;
	float powerUpTime;
	float powerUpTimeMax;

	//Initlizers

	//Function
	void updateEnemies(Player& player, const float& dt);
	void updatePowerUps( Player& player, const float& dt);
	void updateColletables(Player& player, const float& dt);

	void createRandomEnemy(const sf::Vector2f& enemy_pos);
	void createRandomPowerUp(const sf::Vector2f& position);
	void createAmmo(enemyType type, const sf::Vector2f& position);

public:
	//Constuctor/Destrucor
	Room( GameStatistics& statistics, const std::string& type,
		const std::string& room_file, sf::Texture& tile_sheet,
		int offsetX, int offsetY);
	Room(const std::string& type, const std::string& room_file, sf::Texture& tile_sheet);
	~Room();

	//Accesors
	const sf::IntRect getTileBounds() const;
	const sf::FloatRect getBounds() const;
	const sf::Vector2f randomFreeTile() const;
	const bool checkCollision(sf::FloatRect bounds) const;
	const std::vector<Enemy*>& getEnemies();
	const std::string getAsString() const;

	//Setters
	void setEnemies(bool enemy_spawn, float spawn_time, int enemy_limit);
	void setPowerUps(bool spawn_power_ups, float power_up_time_max, int power_up_limit);

	//Functions
	void spawnEnemies();
	void spawnPowerUps();
	void updateCollision(Entity* entity, const float& dt);
	void update( Player* player, const float& dt);
	void render(sf::RenderTarget& target, const sf::Vector2i& gridPosition) const;
	void renderDeffered(sf::RenderTarget& target) const;
	void fullRender(sf::RenderTarget& target) const;

};

