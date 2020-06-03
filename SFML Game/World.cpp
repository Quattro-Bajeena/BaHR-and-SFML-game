#include "stdafx.h"
#include "World.h"

//Initliaziers
void World::initRooms()
{
	roomFiles["TEST"] = "maps/default_map.txt";
}

//Constructor / destructor
World::World(sf::Texture& tileSheet, GameStatistics& statistics)
	:tileSheet(tileSheet), statistics(statistics)
{
	this->currentRoom = nullptr;
	this->initRooms();
}

World::~World()
{
	for (Room* room : this->rooms) {
		delete room;
	}
}

//Functions
void World::addRoom(std::string room_type, int offsetX, int offsetY,
	bool enemy_spawn, float spawn_time, int enemy_limit,
	bool spawn_power_ups, float power_up_time_max, int power_up_limit)
{
	this->rooms.emplace_back(new Room( this->statistics,
		this->roomFiles.at(room_type), this->tileSheet, offsetX, offsetY));

	this->rooms.back()->setEnemies(enemy_spawn, spawn_time, enemy_limit);
	this->rooms.back()->setPowerUps(spawn_power_ups, power_up_time_max, power_up_limit);
	this->rooms.back()->spawnPowerUps();
	this->rooms.back()->spawnEnemies();

	if(this->currentRoom == nullptr)
		this->currentRoom = this->rooms.back();
	
}

const sf::IntRect World::getRoomTileBounds() const
{
	return this->currentRoom->getTileBounds();
}

//Accesors
const sf::FloatRect World::getRoomBounds() const
{
	return this->currentRoom->getBounds();
}

const sf::Vector2f World::randomFreeTile() const
{
	return this->currentRoom->randomFreeTile();
}

const std::vector<Enemy*>& World::getEnemies()
{
	return this->currentRoom->getEnemies();
}

void World::updateCollision(Entity* entity, const float& dt)
{
	this->currentRoom->updateCollision(entity, dt);
}

void World::updateBulletCollisions(std::vector<Bullet>* const bullets)
{
	for (auto bullet = bullets->begin(); bullet != bullets->end();) {

		if (this->currentRoom->checkCollision(bullet->getShape().getGlobalBounds()) == true) {
			bullet = bullets->erase(bullet);
		}
		else bullet++;
	}
}



//Main 
void World::update(Player* player, const float& dt)
{
	//Determing in which room the player is
	for (Room* room : this->rooms) {
		if (room->getBounds().contains(player->getCenterPosition())) {
			this->currentRoom = room;
			break;
		}
	}
	//Updating that room (enemies, power ups in it)
	this->currentRoom->update(player, dt);
}

void World::render(sf::RenderTarget& target, const sf::Vector2i& gridPosition)
{
	for (Room* room : this->rooms) {
		room->render(target, gridPosition);
	}
}

void World::renderDeffered(sf::RenderTarget& target)
{
	for (Room* room : this->rooms) {
		room->renderDeffered(target);
	}
}
