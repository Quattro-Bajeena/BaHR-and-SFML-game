#ifndef GAMESTATE_H
#define GAMESTATE_H


#include "State.h"
#include "PauseMenu.h"
#include "TileMap.h"

class PauseMenu;
class Player;
class TileMap;


class GameState :
	public State
{
private:
	sf::View view;
	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;
	sf::Font font;

	PauseMenu *pmenu;
	Player* player;
	TileMap* tileMap;

	//initlaizer Functions
	void initDeferredRender();
	void initView();
	void initKeybinds();
	void initTexures();
	void initPlayers();
	void initFonts();
	void initPauseMenu();
	void initTileMap();

public:
	GameState(StateData* state_data);
	virtual ~GameState();

	//Function

	void updateView(const float& dt);
	void updateInput(const float& dt);
	void updatePlayerInput(const float& dt);
	void updatePauseMenuButtons();
	void updateTileMap(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr);
};


#endif // !GAMESTATE_H