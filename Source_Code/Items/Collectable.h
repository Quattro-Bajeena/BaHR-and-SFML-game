#pragma once
#include "Assets.h"
#include "Utility.h"
class Collectable
{
protected:
	sf::Sprite sprite;
	sf::IntRect rect;
	sf::Text text;

	collectableType type;

	bool active;
	bool magnet;

	float timer;
	float timerMax;


public:
	Collectable();
	virtual ~Collectable() = default;
	//Functions
	const bool checkCollision(sf::FloatRect object) const;

	//Accesors
	const collectableType getCollectableType() const;
	const bool isActive() const;

	//Main
	virtual void update(sf::Vector2f target, const float dt) = 0;
	virtual void render(sf::RenderTarget& target) const = 0;
};

