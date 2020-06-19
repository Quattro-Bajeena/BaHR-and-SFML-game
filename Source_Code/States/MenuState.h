#pragma once
#include "GameState.h"
#include "MultiplayerServerState.h"
#include "EditorState.h"
#include "SettingsState.h"
#include "WorldEditorState.h"
#include "InfoState.h"

class GameState;
class MultiplayerServerState;
class SettingsState;
class WorldEditorState;
class EditorState;
class EndScreenState;

class MenuState :
	public State
{
private:
	//Variables
	sf::RectangleShape background;
	sf::Text mouseText;

	GameStatistics& statistics;
	std::map<std::string, std::unique_ptr<gui::Button>> buttons;
	std::map<std::string, sf::Text> texts;
	std::map<std::string, std::pair<sf::Vector2f, int>> orgTextParams;

	std::unique_ptr<gui::TextBox> textBox;
	float fontSize1;

	//Functions
	void initVariables();
	void initBackground();
	void initGui();
	void initButtons();
	void initText();

public:
	MenuState(StateData& state_data, GameStatistics& statistics, AudioManager& audio);
	virtual ~MenuState();

	//Function
	void refreshGui();
	void updateInput(const float& dt);
	void updateButtons();
	void updateGui();
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target) const;
	void render(sf::RenderTarget* target = nullptr) const;
};

