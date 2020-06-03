#pragma once
#include "Room.h"
class Room;
class Entity;
class Player;
class Enemy;

class World
{
private:
	GameStatistics& statistics;

	//Room variables
	std::map<std::string, std::string> roomFiles;
	std::vector<Room*> rooms;
	Room* currentRoom;
	sf::Texture& tileSheet;

	//Gameplay

	//Initliazers
	void initRooms();

public:
	World(sf::Texture& tileSheet, GameStatistics& statistics);
	~World();

	//Functions
	void addRoom(std::string room_type, int offsetX, int offsetY,
		bool enemy_spawn, float spawn_time, int enemy_limit,
		bool spawn_power_ups, float power_up_time_max, int power_up_limit);

	//Accesors
	const sf::IntRect getRoomTileBounds() const;
	const sf::FloatRect getRoomBounds() const;
	const sf::Vector2f randomFreeTile() const;
	const std::vector<Enemy*>& getEnemies();

	//Main functions
	void updateCollision(Entity* entity, const float& dt);
	void updateBulletCollisions(std::vector<Bullet>* const bullets);

	void update(Player* player, const float& dt);
	void render(sf::RenderTarget& target, const sf::Vector2i& gridPosition);
	void renderDeffered(sf::RenderTarget& target);
};

