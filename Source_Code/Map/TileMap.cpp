#include "stdafx.h"
#include "TileMap.h"

void TileMap::clear()
{

	for (int x = 0; x < this->maxSizeWorldGrid.x; x++) {

		for (int y = 0; y < this->maxSizeWorldGrid.y; y++) {

			for (int z = 0; z < this->layers; z++) {

				while (this->map[x][y][z].size() > 0) {
					delete this->map[x][y][z][this->map[x][y][z].size() - 1];
					this->map[x][y][z].pop_back();
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

TileMap::TileMap(float grid_size, int width, int height, int offsetX, int offsetY, int layers, sf::Texture& tile_sheet)
	:gridSizeF(grid_size), maxSizeWorldGrid(width, height), offsetX(offsetX), offsetY(offsetY), layers(layers), tileSheet(tile_sheet)
{
	this->gridSizeI = static_cast<int>(this->gridSizeF);
	this->maxSizeWorldF.x = static_cast<float>(width) * grid_size;
	this->maxSizeWorldF.y = static_cast<float>(height) * grid_size;

	/*this->fromX = 0;
	this->fromY = 0;
	this->toX = 0;
	this->toY = 0;*/
	this->layer = 0;

	this->map.resize(this->maxSizeWorldGrid.x, std::vector<std::vector<std::vector<Tile*>>>());
	for (int x = 0; x < this->maxSizeWorldGrid.x; x++) {

		for (int y = 0; y < this->maxSizeWorldGrid.y; y++) {
			this->map[x].resize(this->maxSizeWorldGrid.y, std::vector<std::vector<Tile*>>());

			for (int z = 0; z < this->layers; z++) {
				this->map[x][y].resize(this->layers, std::vector<Tile*>());
			}
		}
	}
	this->showCollision = false;
	this->showBoundary = false;
	this->boundary.setSize(this->maxSizeWorldF);
	this->boundary.setPosition(offsetX * gridSizeF, offsetY * gridSizeF);
	this->boundary.setFillColor(sf::Color::Transparent);
	this->boundary.setOutlineColor(sf::Color::Magenta);
	this->boundary.setOutlineThickness(8.f);
	
}

TileMap::TileMap(const std::string file_name, sf::Texture& tile_sheet, int offsetX, int offsetY)
	:offsetX(offsetX), offsetY(offsetY), tileSheet(tile_sheet)
{
	/*this->fromX = 0;
	this->fromY = 0;
	this->toX = 0;
	this->toY = 0;*/

	std::ifstream in_file(file_name);
	if (in_file.is_open()) {

		sf::Vector2i size;
		int gridSize = 0;
		int layers = 0;
		//std::string texture_file = "";
		int x = 0;
		int y = 0;
		int z = 0;
		int trX = 0;
		int trY = 0;
		bool collision = false;
		this->showCollision = false;
		int short type = TileTypes::DEFAULT;

		in_file >> size.x >> size.y >> gridSize >> layers;// >> texture_file;

		this->gridSizeF = static_cast<float>(gridSize);
		this->gridSizeI = gridSize;
		this->maxSizeWorldGrid = size;
		this->layers = layers;

		this->gridSizeI = static_cast<int>(this->gridSizeF);
		this->maxSizeWorldF.x = static_cast<float>(size.x) * gridSize;
		this->maxSizeWorldF.y = static_cast<float>(size.y) * gridSize;

		this->boundary.setSize(this->maxSizeWorldF);
		this->boundary.setPosition(offsetX * gridSizeF, offsetY * gridSizeF);
		this->boundary.setFillColor(sf::Color::Transparent);
		this->boundary.setOutlineColor(sf::Color::Magenta);
		this->boundary.setOutlineThickness(-2.f);

		//Clears and rebuilds the tilemap to default state
		this->map.resize(this->maxSizeWorldGrid.x, std::vector<std::vector<std::vector<Tile*>>>());
		for (int x = 0; x < this->maxSizeWorldGrid.x; x++) {


			for (int y = 0; y < this->maxSizeWorldGrid.y; y++) {
				this->map[x].resize(this->maxSizeWorldGrid.y, std::vector<std::vector<Tile*>>());

				for (int z = 0; z < this->layers; z++) {
					this->map[x][y].resize(this->layers, std::vector<Tile*>());
				}
			}
		}

		//Getting inforamtions about individual new tiles and creating them
		while (in_file >> x >> y >> z >> trX >> trY >> collision >> type) {
			this->map[x][y][z].push_back(
				new Tile(
					x + this->offsetX, y + this->offsetY,
					this->gridSizeF, this->tileSheet,
					sf::IntRect(trX, trY,
						this->gridSizeI,
						this->gridSizeI),
					collision,this->showCollision, type)
				);
		}

	}
	else std::cout << "CONSTRUCTOR: coulnd not load tilemap from file: " << file_name << "\n";

	in_file.close();
}

TileMap::~TileMap()
{
	this->clear();
}

//Accesors
const sf::Texture* const TileMap::getTileSheet() const
{
	return &this->tileSheet;
}

const sf::Vector2f TileMap::getSize() const
{
	return this->maxSizeWorldF;
}

const sf::Vector2i TileMap::getWorldGridSize() const
{
	return this->maxSizeWorldGrid;
}

const sf::FloatRect TileMap::getBounds() const
{
	return sf::FloatRect(this->offsetX * this->gridSizeF, this->offsetY*this->gridSizeF, this->maxSizeWorldF.x, this->maxSizeWorldF.y);
}

const sf::IntRect TileMap::getTileBounds() const
{
	return sf::IntRect(this->offsetX, this->offsetY, this->maxSizeWorldGrid.x, this->maxSizeWorldGrid.y);
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
	return -1;
}

const sf::Vector2f TileMap::randomFreeTile()
{
	//sf::Vector2f rand_tile = sf::Vector2f(rand() % this->maxSizeWorldGrid.x, rand() % this->maxSizeWorldGrid.y);
	int i = 0;
	bool ok = false;
	int x, y;

	while (ok == false) {
		ok = true;
		x = RNG::get().randomI(1,this->maxSizeWorldGrid.x-2);
		y = RNG::get().randomI(1, this->maxSizeWorldGrid.y-2);

		if (this->map[x][y][this->layer].size() == 0) {
			ok = false;
			continue;
		}

		for (int k = 0; k < this->map[x][y][this->layer].size(); ++k) {
			if (this->map[x][y][this->layer][k]->getCollision() == true) {
				ok = false;
				continue;
			}
		}
		
		if (i > 300) {
			std::cout << "Couldn't find a random free tile\n";
			break;
		}
		i++;
	}
	

	return sf::Vector2f((x + this->offsetX) * this->gridSizeF, (y+this->offsetY) * this->gridSizeF);
}

void TileMap::clearTiles()
{
	for (int x = 0; x < this->maxSizeWorldGrid.x; x++) {

		for (int y = 0; y < this->maxSizeWorldGrid.y; y++) {

			for (int z = 0; z < this->layers; z++) {

				while (this->map[x][y][z].size() > 0) {
					delete this->map[x][y][z][this->map[x][y][z].size() - 1];
					this->map[x][y][z].pop_back();
				}

				
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
		out_file << this->maxSizeWorldGrid.x << " " << this->maxSizeWorldGrid.y << "\n";
		out_file << this->gridSizeI << "\n";
		out_file << this->layers << "\n";
		//out_file << this->textureFile << "\n";

		for (int x = 0; x < this->maxSizeWorldGrid.x; x++) {

			for (int y = 0; y < this->maxSizeWorldGrid.y; y++) {

				for (int z = 0; z < this->layers; z++) {

					for (int k = 0; k < this->map[x][y][z].size(); k++)
					{
						out_file << x << " " << y << " " << z << " "
							<< this->map[x][y][z][k]->getAsString() << " ";//make sure this last space is not saved
					}
					

				}
			}
		}
	}
	else std::cout << "coulnd not save tilemap to file: " << file_name << "\n";

	out_file.close();

}

void TileMap::loadFromFile(const std::string file_name, sf::Texture tile_sheet)
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
		//std::string texture_file = "";
		int x = 0;
		int y = 0;
		int z = 0;
		int trX = 0;
		int trY = 0;
		bool collision = false;
		int short type = TileTypes::DEFAULT;

		in_file >> size.x >> size.y >> gridSize >> layers; //>> texture_file;

		this->gridSizeF = static_cast<float>(gridSize);
		this->gridSizeI = gridSize;
		this->maxSizeWorldGrid = size;
		this->maxSizeWorldF = sf::Vector2f(size * gridSize);
		this->layers = layers;

		this->boundary.setSize(this->maxSizeWorldF);
		this->boundary.setPosition(offsetX * gridSizeF, offsetY * gridSizeF);
		//this->textureFile = texture_file;

		//Clears and rebuilds the tilemap to default state
		this->clear();
		this->map.resize(this->maxSizeWorldGrid.x, std::vector<std::vector<std::vector<Tile*>>>());
		for (int x = 0; x < this->maxSizeWorldGrid.x; x++) {


			for (int y = 0; y < this->maxSizeWorldGrid.y; y++) {
				this->map[x].resize(this->maxSizeWorldGrid.y, std::vector<std::vector<Tile*>>());

				for (int z = 0; z < this->layers; z++) {
					this->map[x][y].resize(this->layers, std::vector<Tile*>());
				}
			}
		}

		//Loading texture

		//Getting inforamtions about individual new tiles and creating them
		while (in_file >> x >> y >> z >> trX >> trY >> collision >> type) {
			this->map[x][y][z].push_back(
				new Tile(
					x+this->offsetX, y+this->offsetY,
					this->gridSizeF, this->tileSheet,
					sf::IntRect(trX, trY,
						this->gridSizeI,
						this->gridSizeI),
					collision,this->showCollision, type)
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
	if (x < this->maxSizeWorldGrid.x && y < this->maxSizeWorldGrid.y && z < layers && x>=0 && y>=0 && this->map[x][y][z].size() < 3) {
		//ok to add tile
		this->map[x][y][z].push_back(new Tile(x + offsetX, y + offsetY, this->gridSizeF, this->tileSheet, texture_rect, collision,this->showCollision, type));
		//std::cout << "ADDED TILE" << "\n";

	}

}

void TileMap::removeTile(const int x, const int y, const int z)
{
	//Remove tile at that position
	if (x < this->maxSizeWorldGrid.x && y < this->maxSizeWorldGrid.y && z < layers && x >= 0 && y >= 0) { //they are int so cant be less than 0
		if (this->map[x][y][z].empty() == false) {
			//ok to remove tile
			delete this->map[x][y][z][this->map[x][y][z].size() - 1];
			this->map[x][y][z].pop_back();
			//std::cout << "REMOVED TILE" << "\n";
		}
	}

}

void TileMap::changeMaxSize(const int width, const int height)
{
	if (width < this->maxSizeWorldGrid.x) {
		for (int x = width; x < this->maxSizeWorldGrid.x; x++) {
			for (int y = 0; y < this->maxSizeWorldGrid.y; y++) {
				for (int z = 0; z < this->layer; z++) {
					while (this->map[x][y][z].size() > 0) {
						delete this->map[x][y][z][this->map[x][y][z].size() - 1];
						this->map[x][y][z].pop_back();
					}
				}
			}
		}
	}

	if (height < this->maxSizeWorldGrid.y) {
		for (int x = 0; x < this->maxSizeWorldGrid.x; x++) {
			for (int y = height; y < this->maxSizeWorldGrid.y; y++) {
				for (int z = 0; z < this->layer; z++) {
					while (this->map[x][y][z].size() > 0) {
						delete this->map[x][y][z][this->map[x][y][z].size() - 1];
						this->map[x][y][z].pop_back();
					}
				}
			}
		}
	}
	

	this->maxSizeWorldGrid = sf::Vector2i(width, height);
	this->maxSizeWorldF.x = static_cast<float>(width) * this->gridSizeI;
	this->maxSizeWorldF.y = static_cast<float>(height) * this->gridSizeI;

	this->boundary.setSize(this->maxSizeWorldF);

	this->map.resize(this->maxSizeWorldGrid.x, std::vector<std::vector<std::vector<Tile*>>>());
	for (int x = 0; x < this->maxSizeWorldGrid.x; x++) {

		for (int y = 0; y < this->maxSizeWorldGrid.y; y++) {
			this->map[x].resize(this->maxSizeWorldGrid.y, std::vector<std::vector<Tile*>>());

			for (int z = 0; z < this->layers; z++) {
				this->map[x][y].resize(this->layers, std::vector<Tile*>());
			}
		}
	}
}

void TileMap::showCollisionTiles()
{
	this->showCollision = true;
}

void TileMap::showMapBoundary()
{
	this->showBoundary = !this->showBoundary;
}

void TileMap::updateCollision(Entity* entity, const float& dt) //change to bool?
{
	//World bounds
	sf::FloatRect playerBounds = entity->getGlobalBounds();
	sf::FloatRect nextPositionBounds = entity->getNextPositionBounds(dt);

	//Tiles
	//Only tiles next to the entity are checkhed for collision
	int fromX = util::clamp(entity->getGridPosition(this->gridSizeI).x - this->offsetX - 2, 0, this->maxSizeWorldGrid.x);
	int toX = util::clamp(static_cast<int>(entity->getGridPosition(this->gridSizeI).x) - this->offsetX + 3, 0, static_cast<int>(this->maxSizeWorldGrid.x));

	int fromY = util::clamp(static_cast<int>(entity->getGridPosition(this->gridSizeI).y) - this->offsetY - 2, 0, static_cast<int>(this->maxSizeWorldGrid.y));
	int toY = util::clamp(static_cast<int>(entity->getGridPosition(this->gridSizeI).y) -this->offsetY + 3, 0, static_cast<int>(this->maxSizeWorldGrid.y));

	

	for (int x = fromX; x < toX; ++x) {

		for (int y = fromY; y < toY; ++y) {

			for (size_t k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
				

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
					else if (playerBounds.left < wallBounds.left
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

const bool TileMap::checkCollision(sf::FloatRect bounds) const
{
	sf::Vector2i grid_position =  sf::Vector2i(
		static_cast<int>(bounds.left) / this->gridSizeI,
		static_cast<int>(bounds.top) / this->gridSizeI
		);

	//Tiles
	//Only tiles next to the entity are checkhed for collision
	int fromX = util::clamp(grid_position.x - this->offsetX - 1, 0, this->maxSizeWorldGrid.x);
	int toX = util::clamp(grid_position.x - this->offsetX + 2, 0, this->maxSizeWorldGrid.x);

	int fromY = util::clamp(grid_position.y - this->offsetY - 1, 0, this->maxSizeWorldGrid.y);
	int toY = util::clamp(grid_position.y - this->offsetY + 2, 0, this->maxSizeWorldGrid.y);



	for (int x = fromX; x < toX; ++x) {

		for (int y = fromY; y < toY; ++y) {

			for (size_t k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
				
				if (this->map[x][y][this->layer][k]->getCollision()
					&& this->map[x][y][this->layer][k]->intersects(bounds)) {
					return true;

				}

			}


		}
	}
	return false;

}


void TileMap::update()
{

}

void TileMap::render(sf::RenderTarget& target, const sf::Vector2i& gridPosition) const
{

	
	int fromX = util::clamp(gridPosition.x -this->offsetX - 30, 0, static_cast<int>(this->maxSizeWorldGrid.x));
	int toX = util::clamp(gridPosition.x - this->offsetX + 30, 0, static_cast<int>(this->maxSizeWorldGrid.x));

	int fromY = util::clamp(gridPosition.y - this->offsetY - 30, 0, static_cast<int>(this->maxSizeWorldGrid.y));
	int toY = util::clamp(gridPosition.y - this->offsetY + 30, 0, static_cast<int>(this->maxSizeWorldGrid.y));

	for (int x = fromX; x < toX; ++x) {

		for (int y = fromY; y < toY; ++y) {
			for (int k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
				if (this->map[x][y][this->layer][k]->getType() == TileTypes::ABOVE) {
					this->defferedRenderStack.push(this->map[x][y][this->layer][k]);
				}
				else {
					this->map[x][y][this->layer][k]->render(target);
				}	

			}
		}
	}

	if(this->showBoundary == true)
		target.draw(this->boundary);

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

void TileMap::renderDeffered(sf::RenderTarget& target) const
{
	while (this->defferedRenderStack.empty() == false) {
		defferedRenderStack.top()->render(target);
		defferedRenderStack.pop();
	}
}

void TileMap::fullRender(sf::RenderTarget& target) const
{
	for (auto& x : this->map) {
		for (auto& y : x) {
			for (auto& z : y) {
				for (auto* k : z) {
					k->render(target);
				}
			}
		}

	}
	
	target.draw(this->boundary);
}









