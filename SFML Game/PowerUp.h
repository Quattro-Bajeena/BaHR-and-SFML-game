#pragma once
#include "Assets.h"

class PowerUp
{
private:
	sf::RectangleShape shape;
	const sf::Texture& texture;
	sf::IntRect rect;
	sf::Text text;

	powerUpType type;

	bool active;

	float timer;
	float timerMax;


public:
	PowerUp(sf::Vector2f position, float size, powerUpType type, const sf::Texture& texture);
	~PowerUp();

	//Functions
	void update(const float dt);
	void render(sf::RenderTarget& target);
	const bool checkCollision(sf::FloatRect object) const;

	//Accesors
	const powerUpType getPowerUpType() const;
	const bool isActive() const;
};

