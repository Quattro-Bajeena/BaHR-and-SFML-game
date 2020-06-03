#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
class Entity;

class Player :
	public Entity
{
private:
	//Variables
	bool rolling;
	bool rollingUp;
	float rollTimer;

	//Initliazer funtions
	void initVariables();
	void initComponents();
public:
	Player(float x, float y, sf::Texture& texture_sheet);
	virtual ~Player();

	//Functions
	void roll();
	void updateAnimations(const float &dt);
	void update(const float& dt);
	void render(sf::RenderTarget& target);

};

#endif // !PLAYER_H