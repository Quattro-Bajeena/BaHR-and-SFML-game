#ifndef STATE_H
#define STATE_H

#include"Entity.h"
#include "Player.h"
#include "GraphicsSettings.h"

class Player;
class GraphicsSettings;
class State;

class StateData {
public:
	StateData():
		gridSize(0),window(nullptr), gfxSettings(nullptr), supportedKeys(nullptr), states(nullptr)
	{};

	//Variables
	int gridSize;
	sf::RenderWindow* window;
	GraphicSettings* gfxSettings;
	std::map<std::string, int>* supportedKeys;
	std::stack<State*>* states;
};

class State
{
private:

protected:
	StateData* stateData;
	std::stack<State*>* states;
	sf::RenderWindow* window;

	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keybinds;
	bool quit;
	bool paused;
	float keytime;
	float keytimeMax;
	int gridSize;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2i mousePosGrid;


	//Resources
	std::map<std::string,sf::Texture> textures;

	//Functions
	virtual void initKeybinds() = 0;
public:
	State(StateData* state_data);
	virtual ~State();

	//Accesors
	const bool& getQuit() const;
	const bool getKeytime();

	//Functions
	void endState();
	void pauseState();
	void unpauseState();
	virtual void updateMousePosition(sf::View* view = nullptr);
	virtual void updateKeytime(const float &dt);
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

};

#endif // !STATE_H