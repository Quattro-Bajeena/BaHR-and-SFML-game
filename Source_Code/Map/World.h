#pragma once
#include "Room.h"
class Room;
class Entity;
class Player;
class Enemy;

class World
{
private:
	GameStatistics* const statistics;

	//Room variables
	std::map<std::string, std::string> roomFiles;
	std::vector<Room*> rooms;
	Room* currentRoom;
	sf::Texture& tileSheet;

	bool editing;

	//Gameplay

	//Initliazers
	void initRooms();
	void clear();

public:
	World(sf::Texture& tileSheet);
	World(sf::Texture& tileSheet, GameStatistics& statistics);
	World(std::string file_name, sf::Texture& tileSheet, GameStatistics& statistics);
	~World();

	//Functions
	void addRoom(std::string room_type, int offsetX, int offsetY,
		bool enemy_spawn, float spawn_time, int enemy_limit);
	void removeRoom(sf::Vector2i room_offset);
	void saveToFile(const std::string& file_name);
	void loadFromFile(const std::string& file_name);

	//Accesors
	const sf::IntRect getRoomTileBounds() const;
	const sf::FloatRect getRoomBounds() const;
	const sf::Vector2f randomFreeTile() const;
	const std::vector<Enemy*>& getEnemies();
	const std::vector<Room*>& getRooms() const;

	//Main functions
	void updateCollision(Entity* entity, const float& dt);
	void updateBulletCollisions(std::vector<Bullet>* const bullets);

	void update(Player* player, const float& dt);
	void render(sf::RenderTarget& target, const sf::Vector2i& gridPosition) const;
	void renderDeffered(sf::RenderTarget& target) const;
	void fullRender(sf::RenderTarget& target) const;
};

