#include "stdafx.h"
#include "World.h"

//Initliaziers
void World::initRooms()
{
	namespace fs = std::filesystem;
	std::string path = "Assets/World/TileMaps/";
	for (const auto& entry : fs::directory_iterator(path)) {
		
		std::string filename = entry.path().filename().stem().string();
		this->roomFiles[filename] = entry.path().string();
	}
		

	//roomFiles["TEST"] = "Assets/World/TileMaps/default_map.txt";
}

void World::clear()
{
	this->currentRoom = nullptr;
	util::purge(this->rooms);
}

World::World(sf::Texture& tileSheet)
	:tileSheet(tileSheet), statistics(nullptr), audio(nullptr)
{
	this->currentRoom = nullptr;
	this->initRooms();
	this->editing = true;
}

//Constructor / destructor
World::World(sf::Texture& tileSheet, GameStatistics& statistics, AudioManager& audio)
	:tileSheet(tileSheet), statistics(&statistics), audio(&audio)
{
	this->currentRoom = nullptr;
	this->initRooms();
	this->editing = false;
}

World::World(std::string file_name, sf::Texture& tileSheet, GameStatistics& statistics, AudioManager& audio)
	:tileSheet(tileSheet), statistics(&statistics), audio(&audio)
{
	this->currentRoom = nullptr;
	this->initRooms();
	this->editing = false;

	this->loadFromFile(file_name);
}

World::~World()
{
	util::purge(this->rooms);
	util::purge(this->players);
}

void World::addPlayer(std::string name, const sf::Vector2f& pos)
{
	this->players.emplace_back(new Player(name, pos, *this->audio));
	this->player = this->players.back();
}

//Functions
void World::addRoom(std::string room_type, int offsetX, int offsetY,
	bool enemy_spawn, float spawn_time, int enemy_limit
)
{
	for (const auto& room : this->rooms) {
		if (room->getTileBounds().left == offsetX && room->getTileBounds().top == offsetY) {
			std::cout << "Can't place 2 rooms int the same place\n";
			return;
		}
	}

	this->rooms.emplace_back(new Room( *this->statistics, *this->audio, room_type,
		this->roomFiles.at(room_type), this->tileSheet, offsetX, offsetY));

	this->rooms.back()->setEnemies(enemy_spawn, spawn_time, enemy_limit);
	//this->rooms.back()->setPowerUps(spawn_power_ups, power_up_time_max, power_up_limit);
	//this->rooms.back()->spawnPowerUps();
	this->rooms.back()->spawnEnemies();

	if(this->currentRoom == nullptr)
		this->currentRoom = this->rooms.back();
	
}

void World::removeRoom(sf::Vector2i room_offset)
{
	for (auto room = this->rooms.begin(); room != this->rooms.end(); ++room) {
		if ((*room)->getTileBounds().left == room_offset.x && (*room)->getTileBounds().top == room_offset.y) {
			delete (*room);
			rooms.erase(room);
			break;
		}
	}
}

void World::saveToFile(const std::string& file_name)
{
	/*
	FORMAT:
	tile map type, offsetX, offsetY, enemy spawn (bool), spawn time max, enemy limit
	*/

	std::ofstream outfile(file_name);
	if (outfile.is_open()) {
		for (const Room* room : this->rooms) {
			outfile << room->getAsString() << "\n";
		}
	}
	else std::cout << "couldn't save world to a file\n";
	outfile.close();
	
}

void World::loadFromFile(const std::string& file_name)
{
	this->clear();
	/*
	FORMAT:
	tile map type, offsetX, offsetY, enemy spawn (bool), spawn time max, enemy limit
	*/
	std::ifstream infile(file_name);

	std::string type;
	sf::Vector2i offset;
	bool spawn;
	float time_max;
	int limit;

	if (infile.is_open()) {
		while (infile >> type >> offset.x >> offset.y >> spawn >> time_max >> limit) {
			this->addRoom(type, offset.x, offset.y, spawn, time_max, limit);
		}
	}
	else std::cout << "Couldn't load the world\n";

	infile.close();
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

const std::vector<Room*>& World::getRooms() const
{
	return this->rooms;
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

void World::render(sf::RenderTarget& target, const sf::Vector2i& gridPosition) const
{
	for (Room* room : this->rooms) {
		room->render(target, gridPosition);
	}
}

void World::renderDeffered(sf::RenderTarget& target) const
{
	for (Room* room : this->rooms) {
		room->renderDeffered(target);
	}
}

void World::fullRender(sf::RenderTarget& target) const
{
	for (Room* room : this->rooms) {
		room->fullRender(target);
	}

}
