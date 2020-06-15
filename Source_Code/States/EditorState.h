#pragma once
#include "State.h"
#include "PauseMenu.h"
#include "Gui.h"
#include"TileMap.h"

class State;
class PauseMenu;
class TileMap;

class EditorState :
	public State
{
private:
	//Variables
	sf::View view;
	sf::Text cursorText;

	//GUI
	std::unique_ptr<PauseMenu> pmenu;
	std::vector<std::string> mapNames;

	std::map<std::string, std::unique_ptr<gui::Button>> buttons;
	std::map<std::string, std::unique_ptr<gui::DropDownList>> lists;
	sf::RectangleShape sidebar;

	sf::IntRect textureRect;
	sf::RectangleShape selectorRect;

	std::unique_ptr<gui::TextureSelector> textureSelector;
	std::unique_ptr<TileMap> tileMap;

	bool collision;
	short int type;
	float cameraSpeed;
	int layer;
	sf::Vector2i mapSize;

	bool sidebarActive;
	std::pair<std::vector<int>, std::vector<int>> mapSizes;

	//Functions
	void initView();
	void initVariables();
	void initText();
	//void initKeybinds();
	void initPauseMenu();
	void initButtons();
	void initTileMap();
	void initGui();
	void initMapFiles();

public:
	EditorState(StateData& state_data, AudioManager& audio);
	virtual ~EditorState();

	//Function
	void updateInput(const float& dt);
	void updateEditorInput(const float& dt);
	void updateButtons();
	void updateGui(const float& dt);
	void updatePauseMenuButtons();
	void update(const float& dt);

	void renderButtons(sf::RenderTarget& target) const;
	void renderGui(sf::RenderTarget& target) const;
	void render(sf::RenderTarget* target = nullptr) const;
};

