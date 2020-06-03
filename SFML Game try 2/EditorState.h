#ifndef EDITORSTATE_H
#define DITORSTATE_H

#include "State.h"
#include "PauseMenu.h"
#include "Gui.h"
#include"TileMap.h"

class State;
class Gui;
class PauseMenu;
class TileMap;

class EditorState :
	public State
{
private:
	//Variables
	sf::View view;
	sf::Font font;
	sf::Text cursorText;
	PauseMenu* pmenu;

	std::map<std::string, gui::Button*> buttons;

	TileMap* tileMap;

	sf::RectangleShape sidebar;

	sf::IntRect textureRect;
	sf::RectangleShape selectorRect;

	gui::TextureSelector* textureSelector;



	bool collision;
	short int type;
	float cameraSpeed;
	int layer;

	//Functions
	void initView();
	void initVariables();
	void initFonts();
	void initText();
	void initKeybinds();
	void initPauseMenu();
	void initButtons();
	void initTileMap();
	void initGui();

public:
	EditorState(StateData* state_data);
	virtual ~EditorState();

	//Function
	void updateInput(const float& dt);
	void updateEditorInput(const float& dt);
	void updateButtons();
	void updateGui(const float& dt);
	void updatePauseMenuButtons();
	void update(const float& dt);

	void renderButtons(sf::RenderTarget& target);
	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

#endif