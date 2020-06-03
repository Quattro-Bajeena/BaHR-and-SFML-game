#include "Collider.h"
#include <iostream>

Collider::Collider(sf::RectangleShape& body) :
	body(body)
{
	std::cout << body.getPosition().x << " | " << body.getPosition().y << "\n";
}

Collider::~Collider()
{
}

bool Collider::CheckCollision(Collider &other, float push)
{
	sf::Vector2f otherPosition = other.GetPosition();
	sf::Vector2f otherHalfSize = other.GetHalfSize();
	sf::Vector2f thisPosition = GetPosition();
	sf::Vector2f thisHalfSize = GetHalfSize();

	float deltaX = otherPosition.x - thisPosition.x;
	float deltaY = otherPosition.y - thisPosition.y;

	

	float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
	float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

	//printf("%f | %f | %f | %f | ", deltaX, deltaY, intersectX, intersectY );

	if (intersectX < 0.0f && intersectY < 0.0f) {
		push = std::min(std::max(push, 0.0f), 1.0f);

		if (intersectX > intersectY) {
			if (deltaX > 0.0f) {
				Move(intersectX * (1.0f - push), 0.0f);
				other.Move(-intersectX * push, 0.0f);
				//printf("interaction ");
			}
			else {
				Move(-intersectX * (1.0f - push), 0.0f);
				other.Move( intersectX * push, 0.0f);
				//printf("interaction ");
			}
		}
		else {
			if (deltaY > 0.0f) {
				Move(0.0f, intersectY * (1.0f - push));
				other.Move(0.0f, -intersectY * push);
				//printf("interaction ");
			}
			else {
				Move(0.0f, -intersectY * ( 1.0f - push));
				other.Move(0.0f, intersectY * push);
				//printf("interaction ");
			}

		}
		//printf("Your(platforM) body: %f | %f \n", body.getPosition().x, other.body.getPosition().y);
		//printf("Other(player) body: %f | %f \n", other.body.getPosition().x, other.body.getPosition().y);
		return true;
	}


	return false;
}
