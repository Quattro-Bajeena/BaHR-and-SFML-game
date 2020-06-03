#ifndef TILE_H
#define TILE_H

enum TileTypes{DEFAULT = 0, ABOVE, DAMAGING};

class Tile
{
private:

protected:
	sf::RectangleShape shape;
	bool collision;
	short int type;
public:
	Tile();
	Tile(int grid_x, int grid_y,float gridSizeF, const sf::Texture& texture, const sf::IntRect& rect,
		bool collision = false, short int type = TileTypes::DEFAULT);
	virtual ~Tile();

	//Accesors
	const sf::Vector2f& getPosition() const;
	const bool& getCollision() const;
	const sf::FloatRect getGlobalBounds() const;
	const short& getType() const;
	
	const std::string getAsString() const;

	//Functions
	const bool intersects(const sf::FloatRect bounds) const;

	void update();
	void render(sf::RenderTarget& target);


};

#endif // !TILE_H