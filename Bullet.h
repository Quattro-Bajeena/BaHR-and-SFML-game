#pragma once

class Bullet {
private:
	float maxDistance;
	sf::CircleShape shape;
	float maxSpeed;
	int damage;
	sf::Vector2f direction;
	sf::Vector2f currentVelocity;


public:
	Bullet(sf::Vector2f startPos, sf::Vector2f dir,
		sf::Color color, float damage, float radius = 15.f,
		float max_speed = 1000.f, float max_distance = 2000.f);

	void move(const float dt);
	const bool checkCollision(sf::FloatRect object) const;
	const bool DistanceLimit(const sf::Vector2f& position) const;

	//Accesors
	const sf::CircleShape getShape() const;
	const int getDamage() const;

};

