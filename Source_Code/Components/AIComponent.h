#pragma once
#include "RNG.h"
#include "Utility.h"
class AIComponent
{

private:
	sf::Sprite& sprite;
	enemyType type;
	enemyState& state;

	sf::CircleShape triggerArea;

	sf::Vector2f targetDir;
	sf::Vector2f roamDir;
	float distance;

	float aggroDistance;
	bool aggro;

	float roamTimer;
	float roamTimerMax;

	sf::Vector2f follow();
	sf::Vector2f keepDistance(const float threshold);
	sf::Vector2f stayStill();
	sf::Vector2f runAway(const float threshold);
	sf::Vector2f roam();
public:
	AIComponent(sf::Sprite& sprite, enemyType type, enemyState& state);
	~AIComponent();
	//Accesors
	const bool isAggro() const;
	//Functions
	sf::Vector2f nextMove(const sf::Vector2f& target_dir, const float& distance);
	void update(const float& dt);
	void render(sf::RenderTarget& target) const;
};

