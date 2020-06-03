#include "stdafx.h"
#include "TileMap.h"

void TileMap::clear()
{

	for (int x = 0; x < this->maxSizeWolrdGrid.x; x++) {

		for (int y = 0; y < this->maxSizeWolrdGrid.y; y++) {

			for (int z = 0; z < this->layers; z++) {

				for (size_t k = 0; k < this->map[x][y][z].size(); ++k)
				{
					delete this->map[x][y][z][k];
					this->map[x][y][z][k] = nullptr;
				}
				
				this->map[x][y][z].clear();
			}
			this->map[x][y].clear();
		}
		this->map[x].clear();
	}

	this->map.clear();
	//std::cout<<"Map cleared, size: " << this->map.size() << "\n";
}

TileMap::TileMap(float grid_size, int width, int height, std::string texture_file)
	:gridSizeF(grid_size), maxSizeWolrdGrid(width, height), layers(1), textureFile(texture_file)
{
	this->gridSizeI = static_cast<int>(this->gridSizeF);
	this->maxSizeWorldF.x = static_cast<float>(width) * grid_size;
	this->maxSizeWorldF.y = static_cast<float>(height) * grid_size;

	this->fromX = 0;
	this->fromY = 0;
	this->toX = 0;
	this->toY = 0;
	this->layer = 0;

	this->map.resize(this->maxSizeWolrdGrid.x, std::vector<std::vector<std::vector<Tile*>>>());
	for (int x = 0; x < this->maxSizeWolrdGrid.x; x++) {


		for (int y = 0; y < this->maxSizeWolrdGrid.y; y++) {
			this->map[x].resize(this->maxSizeWolrdGrid.y, std::vector<std::vector<Tile*>>());

			for (int z = 0; z < this->layers; z++) {
				this->map[x][y].resize(this->layers, std::vector<Tile*>());
			}
		}
	}

	if (!this->tileSheet.loadFromFile(texture_file)) 
		std::cout << "TILEMAP failed to load tile texture sheet" << "\n";

	//this->collisionBox.setSize(sf::Vector2f(gridSizeF, gridSizeF));
	//this->collisionBox.setFillColor(sf::Color(255, 0, 0, 50));

}

TileMap::~TileMap()
{
	this->clear();
}

//Accesors
const sf::Texture* TileMap::getTileSheet() const
{
	return &this->tileSheet;
}

const sf::Vector2f TileMap::getSize() const
{
	return this->maxSizeWorldF;
}

const int TileMap::getLayerSize(const int x, const int y, const int z) const
{
	if (x >= 0 && x < this->map.size()) {
		if (y >= 0 && y < this->map[x].size()) {
			if (z >= 0 && z < this->map[x][y].size()) {
				return this->map[x][y][layer].size();
			}
		}
	}
}


//Functions





void TileMap::saveToFile(const std::string file_name)
{
	/*Saves the entire tilemap as a text-tile
	Format:
	Size x y
	gridSize
	layers
	texture_file

	All tiles:
	GridPos x y layer, Texturerect x y, collision, type

	*/


	std::ofstream out_file(file_name);
	if (out_file.is_open()) {
		out_file << this->maxSizeWolrdGrid.x << " " << this->maxSizeWolrdGrid.y << "\n";
		out_file << this->gridSizeI<<"\n";
		out_file << this->layers << "\n";
		out_file << this->textureFile << "\n";

		for (int x = 0; x < this->maxSizeWolrdGrid.x; x++) {

			for (int y = 0; y < this->maxSizeWolrdGrid.y; y++) {

				for (int z = 0; z < this->layers; z++) {

					if (this->map[x][y][z].empty() == false) {
						for (size_t k = 0; k < this->map[x][y][z].size(); k++)
						{
							out_file << x << " " << y << " " << z << " "
								<< this->map[x][y][z][k]->getAsString() << " ";//make sure this last space is not saved
						}
					}
						
				}
			}
		}
	}
	else std::cout << "coulnd not save tilemap to file: " << file_name << "\n";

	out_file.close();

}

void TileMap::loadFromFile(const std::string file_name)
{

	/*Loads the entire tilemap as a text-tile
	Format:
	Size x y
	gridSize
	layers
	texture_file

	All tiles:
	GridPos x y layer, Texturerect x y, collision, type

	*/

	std::ifstream in_file(file_name);
	if (in_file.is_open()) {

		sf::Vector2i size;
		int gridSize = 0;
		int layers = 0;
		std::string texture_file = "";
		int x = 0;
		int y = 0;
		int z = 0;
		int trX = 0;
		int trY = 0;
		bool collision = false;
		int short type = TileTypes::DEFAULT;

		in_file >> size.x >> size.y >> gridSize >> layers >> texture_file;

		this->gridSizeF = static_cast<float>(gridSize);
		this->gridSizeI = gridSize;
		this->maxSizeWolrdGrid = size;
		this->layers = layers;
		this->textureFile = texture_file;

		//Clears and rebuilds the tilemap to default state
		this->clear();
		this->map.resize(this->maxSizeWolrdGrid.x, std::vector<std::vector<std::vector<Tile*>>>());
		for (int x = 0; x < this->maxSizeWolrdGrid.x; x++) {


			for (int y = 0; y < this->maxSizeWolrdGrid.y; y++) {
				this->map[x].resize(this->maxSizeWolrdGrid.y, std::vector<std::vector<Tile*>>());

				for (int z = 0; z < this->layers; z++) {
					this->map[x][y].resize(this->layers, std::vector<Tile*>());
				}
			}
		}

		//Loading texture
		if (!this->tileSheet.loadFromFile(texture_file))
			std::cout << "TILEMAP failed to load tile texture sheet" << "\n";

		//Getting inforamtions about individual new tiles and creating them
		while (in_file >> x >> y >> z >> trX >> trY >> collision >> type) {
			this->map[x][y][z].push_back(
				new Tile(
					x, y,
					this->gridSizeF, this->tileSheet,
					sf::IntRect(trX, trY,
						this->gridSizeI,
						this->gridSizeI),
					collision,type)
				);
		}

	}
	else std::cout << "coulnd not load tilemap from file: " << file_name << "\n";

	in_file.close();
}


void TileMap::addTile(const int x, const int y, const int z,
	const sf::IntRect& texture_rect, bool collision, short int type)
{
	//Take two indicies from the mouse position in the gird
	//and add a tile to that postion if the interal tilemap array allows it

	//if the mouse is outside it wont do
	if (x < this->maxSizeWolrdGrid.x && y < this->maxSizeWolrdGrid.y && z < layers
		&& x >=0 && y>=0) { 
		//ok to add tile
		this->map[x][y][z].push_back( new Tile(x , y , this->gridSizeF, this->tileSheet, texture_rect, collision, type));
		std::cout << "ADDED TILE" << "\n";
		
	}

}

void TileMap::removeTile(const int x, const int y, const int z)
{
	//Remove tile at that position
	if (x < this->maxSizeWolrdGrid.x && y < this->maxSizeWolrdGrid.y && z < layers
		&& x >= 0 && y >= 0) { //they are int so cant be less than 0
		if (this->map[x][y][z].empty()==false) {
			//ok to remove tile
			delete this->map[x][y][z][this->map[x][y][z].size()-1];
			this->map[x][y][z].pop_back();
			std::cout << "REMOVED TILE" << "\n";
		}
	}

}

void TileMap::updateCollision(Entity* entity, const float& dt)
{
	//World bounds
	sf::FloatRect playerBounds = entity->getGlobalBounds();
	sf::FloatRect nextPositionBounds = entity->getNextPositionBounds(dt);

	if (nextPositionBounds.left < 0.f) {
		entity->setPosition(0.f, playerBounds.top);
		entity->stopVelocityX();
	}
		
	else if (nextPositionBounds.left + nextPositionBounds.width > this->maxSizeWorldF.x) {
		entity->setPosition(this->maxSizeWorldF.x - playerBounds.width, playerBounds.top);
		entity->stopVelocityX();
	}
		

	if (nextPositionBounds.top < 0.f) {
		entity->setPosition(playerBounds.left, 0.f);
		entity->stopVelocityY();
	}
		
	else if (nextPositionBounds.top + nextPositionBounds.height > this->maxSizeWorldF.y) {
		entity->setPosition(playerBounds.left, this->maxSizeWorldF.y - playerBounds.height);
		entity->stopVelocityY();
	}
	
	
	
	//Tiles
	this->fromX = clamp(static_cast<int>(entity->getGridPosition(this->gridSizeI).x) - 1, 0, static_cast<int>(this->maxSizeWolrdGrid.x));
	this->toX = clamp(static_cast<int>(entity->getGridPosition(this->gridSizeI).x) + 3, 0, static_cast<int>(this->maxSizeWolrdGrid.x));

	this->fromY = clamp(static_cast<int>(entity->getGridPosition(this->gridSizeI).y) - 1, 0, static_cast<int>(this->maxSizeWolrdGrid.y));
	this->toY = clamp(static_cast<int>(entity->getGridPosition(this->gridSizeI).y) + 3, 0, static_cast<int>(this->maxSizeWolrdGrid.y));


	//std::cout << this->fromX << " " << this->toX << "\n";
	//std::cout << this->fromY << " " << this->toY << "\n";


	for (int x = this->fromX; x < this->toX; ++x) {

		for (int y = this->fromY; y < this->toY; ++y) {

			for (size_t k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
				if (this->map[x][y][this->layer].empty() == false) {

					sf::FloatRect wallBounds = this->map[x][y][this->layer][k]->getGlobalBounds();

					if (this->map[x][y][this->layer][k]->getCollision()
						&& this->map[x][y][this->layer][k]->intersects(nextPositionBounds)) {


						//Bottom collisin
						if (playerBounds.top < wallBounds.top
							&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
							&& playerBounds.left < wallBounds.left + wallBounds.width
							&& playerBounds.left + playerBounds.width > wallBounds.left
							)
						{
							entity->stopVelocityY();
							entity->setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
						}

						//Top Collision
						else if (playerBounds.top > wallBounds.top
							&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
							&& playerBounds.left < wallBounds.left + wallBounds.width
							&& playerBounds.left + playerBounds.width > wallBounds.left
							)
						{
							entity->stopVelocityY();
							entity->setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
						}

						//Right collisin
						if (playerBounds.left < wallBounds.left
							&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
							&& playerBounds.top < wallBounds.top + wallBounds.height
							&& playerBounds.top + playerBounds.height > wallBounds.top
							)
						{
							entity->stopVelocityX();
							entity->setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
						}

						//Left Collision
						else if (playerBounds.left > wallBounds.left
							&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
							&& playerBounds.top < wallBounds.top + wallBounds.height
							&& playerBounds.top + playerBounds.height > wallBounds.top
							)
						{
							entity->stopVelocityX();
							entity->setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
						}

					}
				}
			}

			
		}
	}

}


void TileMap::update()
{

}

void TileMap::render(sf::RenderTarget& target, const sf::Vector2i& gridPosition) 
{
	
	this->layer = 0;
	this->fromX = clamp(gridPosition.x - 5, 0, static_cast<int>(this->maxSizeWolrdGrid.x));
	this->toX = clamp(gridPosition.x + 7, 0, static_cast<int>(this->maxSizeWolrdGrid.x));

	this->fromY = clamp(gridPosition.y - 5, 0, static_cast<int>(this->maxSizeWolrdGrid.y));
	this->toY = clamp(gridPosition.y + 7, 0, static_cast<int>(this->maxSizeWolrdGrid.y));

	for (int x = this->fromX; x < this->toX; ++x) {

		for (int y = this->fromY; y < this->toY; ++y) {
			for (size_t k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
				if (this->map[x][y][this->layer].empty() == false) {
					if (this->map[x][y][this->layer][k]->getType() == TileTypes::ABOVE) {
						this->defferedRenderStack.push(this->map[x][y][this->layer][k]);
					}
					else {
						this->map[x][y][this->layer][k]->render(target);
					}
					
				}
					
			}
		}
	}
	
	
	/*for (auto& x : this->map) {
		for (auto& y : x) {
			for (auto& z : y) {
				for (auto* k : z) {
					k->render(target);
				}
			}
		}
	}*/
}

void TileMap::renderDeffered(sf::RenderTarget& target)
{
	while (this->defferedRenderStack.empty() == false) {
		defferedRenderStack.top()->render(target);
		defferedRenderStack.pop();
	}
}






