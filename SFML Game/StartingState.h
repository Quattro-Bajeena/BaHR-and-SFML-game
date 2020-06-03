#pragma once
#include "State.h"
class StartingState :
	public State
{
private:
	//Varaibles
	sf::Font font;
	sf::RectangleShape blind;
	sf::Music music;

	//Init funtions
	void initFont();
	void initMusic();

public:
	StartingState(sf::RenderWindow* window, std::stack<State*>* states);
	virtual ~StartingState();

	//Functions
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr);

};

