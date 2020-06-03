#ifndef HITBOX_COMPONENT
#define HITBOX_COMPONENT

class HitboxComponent
{

private:
	sf::Sprite& sprite;
	sf::RectangleShape hitbox;
	sf::RectangleShape center;
	sf::FloatRect nextPosition;
	sf::Vector2f centerPosition;
	const float width;
	const float height;
	float offsetX;
	float offsetY;
	float offsetXDefault;
	float offsetYDefault;

public:
	HitboxComponent(sf::Sprite& sprite,
		float offset_x, float offset_y,
		float width, float height );
	virtual ~HitboxComponent();

	//Accesors
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getCenterPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const sf::FloatRect& getNextPosition(const sf::Vector2f& velocity, const float& dt);

	//Modifiers
	void setPosition(const float &x, const float &y);

	//functions
	void reset();
	void changeHitbox(const float& offset_x,const float& offset_y, const float& width, const float& height);
	bool intersects(const sf::FloatRect& frect);
	void update();
	void render(sf::RenderTarget& target);
};

#endif