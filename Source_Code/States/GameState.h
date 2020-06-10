#pragma once
#include "State.h"
#include "EndScreenState.h"
#include "World.h"


class PauseMenu;
class Player;
class TileMap;

class GameState :
	public State
{
protected:
	sf::View view;
	mutable sf::RenderTexture renderTexture;
	mutable sf::Sprite renderSprite;

	const std::string playerName;
	GameStatistics& statistics;

	bool lost;
	bool start;
	bool restart;


	std::map<std::string, sf::Text> texts;

	//GUI
	std::unique_ptr<gui::HealthBar> healthBar;
	std::unique_ptr<gui::Bar> healthBar2;
	std::unique_ptr<gui::WeaponSlot> weaponSlot;
	std::unique_ptr<gui::ReloadBar> reloadBar;
	sf::Sprite crosshair;

	//Gameplay objects
	Player* player;
	//World* world;
	std::unique_ptr<World> world;

	//Gameplay variables
	 
	//Initlaizer funtions 
	
	void initVariables();
	void initView();
	void initText();
	void initWorld();
	virtual void initPlayer();
	void initSound();
	void initGui();
	void initDeferredRender();

	

	//Functions 
	void loose();

	void updateView(const float& dt);
	void updateInput(const float& dt);
	void updateGui(const float& dt);
	void updatePauseMenuButtons(const float& dt);

	virtual void updatePlayerInput(const float& dt);
	void updateTileMap(const float& dt);

	void renderGui(sf::RenderTarget* target = nullptr) const;

public:
	GameState(StateData& state_data, GameStatistics &stats);
	virtual ~GameState();

	//Function
	virtual void update(const float& dt);
	virtual void render(sf::RenderTarget* target = nullptr) const;
	

};

