#pragma once
#include "State.h"
#include "PauseMenu.h"
#include "Button.h"
#include "World.h"
#include "DropDownList.h"

class WorldEditorState :
    public State
{
private:
	//Variables
	sf::View view;
	sf::Text cursorText;

	//GUI
	std::unique_ptr<PauseMenu> pmenu;
	std::map<std::string, std::unique_ptr<gui::DropDownList>> lists;
	std::map<std::string, std::unique_ptr<gui::Button>> buttons;
	sf::RectangleShape sidebar;

	//sf::IntRect textureRect;
	sf::RectangleShape selectorRect;

	mutable sf::RenderTexture renderTexture;
	mutable sf::RectangleShape placeRect;

	Room* tempRoom;
	Room* selectedRoom;
	
	//Objects
	std::map<std::string, std::string> roomFiles;
	std::vector<std::pair<std::string, std::string>> roomFilesVec;
	std::vector<std::string> worldsStr;

	std::unique_ptr<World> world;

	//Variables settings
	float cameraSpeed;
	bool placingNewRoom;

	std::string roomPath;
	std::string roomType;
	sf::Vector2i roomOffset;
	sf::Vector2i offset;

	bool enemySpawn;
	float enemySpawnTime;
	int enemyLimit;
	
	

	//Functions
	
	void initView();
	void initVariables();
	void initText();
	//void initKeybinds();
	void initPauseMenu();
	void initButtons();
	void loadFiles();
	void initWorld();
	void initGui();


	void startPlacingRoom();

public:
	WorldEditorState(StateData& state_data);
	virtual ~WorldEditorState();

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

