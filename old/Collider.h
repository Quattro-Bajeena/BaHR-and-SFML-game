#pragma once
#include <SFML/Graphics.hpp>

class Collider
{
public:
	Collider(sf::RectangleShape& body);
	~Collider();

	void Move(float dx, float dy) { printf("| before move: %f , %f |",body.getPosition().x, body.getPosition().y); body.move(dx, dy); printf("| after move: %f , %f \n|", body.getPosition().x, body.getPosition().y);
	}

	bool CheckCollision(Collider &other, float push);
	sf::Vector2f GetPosition() { return body.getPosition(); }
	sf::Vector2f GetHalfSize() { return body.getSize() / 2.0f; }

private:
	sf::RectangleShape body;


};

