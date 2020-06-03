#include "stdafx.h"
#include "MainMenuState.h"

//Initializer function
void MainMenuState::initVariables()
{
}

void MainMenuState::initBackground()
{
	this->background.setSize(static_cast<sf::Vector2f>(this->window->getSize()));

	if (!this->backgroundTexture.loadFromFile("Assets/Images/Backgrounds/bg1.png")) {
		throw"ERROR::MAIN_MENU_STATE::FAILED_tolOAD_BACGORUND_TEXTURE";
	}
	this->background.setTexture(&this->backgroundTexture);
}

void MainMenuState::initFonts()
{
	if (!this->font.loadFromFile("Assets/Fonts/Dosis-Light.otf")) {
		throw("ERROR::MAINMENUSTATE::COULD NOT LOAD FONT"); 
	}
}

void MainMenuState::initKeybinds()
{

	std::ifstream ifs("Config/mainmenustate_keybinds.ini");
	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();

}

void MainMenuState::initButtons()
{
	this->buttons["GAME_STATE"] = new gui::Button(
		1400.f, 140.f, 200.f, 150.f,
		&this->font, "New Game", 45, 
		sf::Color(10, 240, 10, 200), sf::Color(230, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(120, 120, 120, 150), sf::Color(150, 200, 150, 200), sf::Color(20, 20, 20, 50)
		);

	this->buttons["SETTINGS_STATE"] = new gui::Button(
		1400.f, 300.f, 200.f, 150.f,
		&this->font, "Settings", 50,
		sf::Color(240, 240, 15, 200), sf::Color(255, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(180, 180, 130, 200), sf::Color(20, 20, 20, 50));

	this->buttons["EDITOR_STATE"] = new gui::Button(
		1610.f, 140.f, 200.f, 150.f,
		&this->font, "Editor", 50,
		sf::Color(15, 15, 15, 200), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 50));

	this->buttons["EXIT_STATE"] = new gui::Button(
		1610.f, 300.f, 200.f, 150.f,
		&this->font, "Quit",50,
		sf::Color(255, 65, 65, 200), sf::Color(255, 220, 220, 255), sf::Color(10, 10, 10, 200),
		sf::Color(90, 90, 90, 150), sf::Color(200, 150, 150, 200), sf::Color(20, 20, 20, 50));
}

MainMenuState::MainMenuState(StateData* state_data)
	:State(state_data)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();

}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}



void MainMenuState::updateInput(const float& dt)
{
	
	
}

void MainMenuState::updateButtons()
{
	/*Updates all th buttons in the state and handles their functionality*/
	for (auto& it : this->buttons) {
		it.second->update(this->mousePosWindow);
	}

	//New game
	if (this->buttons["GAME_STATE"]->isReleased()) {
		this->states->push(new GameState(this->stateData));
	}

	//Settings
	if (this->buttons["SETTINGS_STATE"]->isReleased()) {
		this->states->push(new SettingsState(this->stateData));
	}

	//Editor
	if (this->buttons["EDITOR_STATE"]->isReleased()) {
		this->states->push(new EditorState(this->stateData));
	}

	//Quit the state
	if (this->buttons["EXIT_STATE"]->isReleased()) {
		this->endState();
	}
}

void MainMenuState::update(const float& dt)
{
	
	this->updateMousePosition();
	this->updateInput(dt);
	this->updateButtons();
	
	
	
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	
	for (auto& it : this->buttons) {
		it.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	target->draw(this->background);

	this->renderButtons(*target);

	//remove later
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(24);
	std::stringstream ss;
	ss << this->mousePosView.x << "\n" << this->mousePosView.y;
	mouseText.setString(ss.str());


	target->draw(mouseText);
}

