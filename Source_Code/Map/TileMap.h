#pragma once
#include "Tile.h"
#include "Entity.h"


class Tile;
class Entity;

class TileMap
{
private:
	
	void clear();
	float gridSizeF;
	int gridSizeI;

	sf::Vector2i maxSizeWorldGrid;
	sf::Vector2f maxSizeWorldF;

	int layers;
	std::vector<std::vector<std::vector< std::vector<Tile*> > > > map;
	mutable std::stack<Tile*> defferedRenderStack;

	sf::Texture& tileSheet;
	//std::string textureFile;

	sf::RectangleShape collisionBox;
	sf::RectangleShape boundary;

	int offsetX;
	int offsetY;

	//Culling
	/*int fromX;
	int toX;
	int fromY;
	int toY;
	*/
	int layer;

public:
	TileMap(float grid_size, int width, int height, int offsetX, int offsetY, int layers, sf::Texture& tile_sheet);
	TileMap(const std::string file_name, sf::Texture& tile_sheet, int offsetX, int offsetY);
	virtual ~TileMap();

	//Accesors
	const sf::Texture* const getTileSheet() const;
	const sf::Vector2f getSize() const;
	const sf::Vector2i getWorldGridSize() const;
	const sf::FloatRect getBounds() const;
	const sf::IntRect getTileBounds() const;
	const int getLayerSize(const int x, const int y, const int z) const;
	const sf::Vector2f randomFreeTile();
	void clearTiles();

	//Function
	//Editingh
	void addTile(const int x, const int y, const int z,
		const sf::IntRect& texture_rect, bool collision = false, short int type = TileTypes::DEFAULT);
	void removeTile(const int x, const int y, const int z);
	void changeMaxSize(const int width, const int height);
	

	//Saving/Loading
	void saveToFile(const std::string file_name);
	void loadFromFile(const std::string file_name, sf::Texture tile_sheet);


	//Gameplay
	void updateCollision(Entity* entity, const float& dt);
	const bool checkCollision(sf::FloatRect bounds) const;

	void update();
	void render(sf::RenderTarget& target, const sf::Vector2i& gridPosition) const;
	void renderDeffered(sf::RenderTarget& target) const;

	void fullRender(sf::RenderTarget& target) const;


};

