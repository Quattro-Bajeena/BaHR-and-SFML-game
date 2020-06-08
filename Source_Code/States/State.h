#pragma once

#include "PowerUp.h"
#include "GameStatistics.h"
#include "GraphicSettings.h"
#include "Assets.h"
#include "Gui.h"

class State;

class StateData {
public:
	StateData() :
		gridSize(0), window(nullptr), gfxSettings(nullptr), supportedKeys(nullptr), states(nullptr)
	{};

	//Variables
	int gridSize;
	sf::Vector2f scale;
	sf::RenderWindow* window;
	GraphicSettings* gfxSettings;
	std::map<std::string, std::string> folderPaths;
	std::map<std::string, int>* supportedKeys;
	std::stack<State*>* states;
	
};

class State
{
private:

protected:

	//Resources 
	StateData& stateData;
	sf::RenderWindow& window;
	std::stack<State*>& states;

	//GUI
	std::unique_ptr<PauseMenu> pauseMenu;

	bool quit;
	bool paused;
	bool cursorVisibility;

	float keytime;
	float keytimeMax;

	float clickTime;
	float clickTimeMax;

	int gridSize;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2i mousePosGrid;
	int scroll;
	bool scrollActive;

	bool textReceived;
	sf::Uint32 unicodeText;

	//Initializers

public:
	State(StateData& state_data);
	virtual ~State();

	//Accesors
	const bool& getQuit() const;
	const bool getKeytime();
	const bool getClickTime();

	//Functions
	void endState();
	void pauseState();
	void unpauseState();
	virtual void refreshGui();
	virtual void updateMousePosition(sf::View* view = nullptr);
	virtual void updateScrollWheel(int ticks);
	virtual void resetScrollWheel();
	virtual void typeText(sf::Uint32 unicode_text);
	virtual void updateKeytime(const float& dt);
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) const = 0;

};

