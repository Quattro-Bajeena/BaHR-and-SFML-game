#ifndef HITBOX_COMPONENT
#define HITBOX_COMPONENT
#pragma once

class HitboxComponent
{

private:
	sf::Sprite &sprite;
	sf::RectangleShape hitbox;
	sf::RectangleShape center;
	sf::FloatRect nextPosition;
	sf::Vector2f centerPosition;
	const float width;
	const float height;
	float offsetX;
	float offsetY;
	const float offsetXDefault;
	const float offsetYDefault;

public:
	HitboxComponent(sf::Sprite& sprite,
		const float offset_x, const float offset_y,
		const float width, const float height);
	virtual ~HitboxComponent();

	//Accesors
	const sf::Vector2f& getPosition() const;
	const sf::Vector2f& getCenterPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const sf::FloatRect& getNextPosition(const sf::Vector2f& velocity, const float& dt);

	const bool checkCollision(sf::FloatRect object) const;
	const bool checkContain(sf::FloatRect object) const;
	const float getDistance(const sf::Vector2f& position) const;

	//Modifiers
	void setPosition(const float& x, const float& y);

	//functions
	void reset();
	void changeHitbox(const float& offset_x, const float& offset_y, const float& width, const float& height);
	bool intersects(const sf::FloatRect& frect);
	void update();
	void render(sf::RenderTarget& target) const;
};

#endif

