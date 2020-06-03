#include "stdafx.h"
#include "GameState.h"


void GameState::initDeferredRender()
{
	this->renderTexture.create(
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height
		);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(sf::IntRect(
		0,
		0,
		this->stateData->gfxSettings->resolution.width,
		this->stateData->gfxSettings->resolution.height));
}

void GameState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width),
			static_cast<float>(this->stateData->gfxSettings->resolution.height)
			));

	this->view.setCenter(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width / 2.f),
				static_cast<float>(this->stateData->gfxSettings->resolution.height / 2.f)
			));
}

//Initializers funtions
void GameState::initKeybinds()
{

	std::ifstream ifs("Config/gamestate_keybinds.ini");
	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();

}

void GameState::initTexures()
{
	if (!this->textures["PLAYER_SHEET"].loadFromFile("Assets/Images/Sprites/Player/player_sprite_sheet.png")) {
		std::cout << "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_tEXTURE" << "\n";
		//throw"ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_tEXTURE";
	}

	//temporary font solution to deleteing later
	if (!this->font.loadFromFile("Assets/Fonts/Dosis-Light.otf")) {
		std::cout << "ERROR::GAMEState::COULD NOT LOAD FONT" << "\n";
		//throw("ERROR::MainMenuState::COULD NOT LOAD FONT");
	}
}

void GameState::initPlayers()
{
	this->player = new Player(0, 0, this->textures["PLAYER_SHEET"]);
}

void GameState::initFonts()
{
	if (!this->font.loadFromFile("Assets/Fonts/Dosis-Light.otf")) {
		throw("ERROR::GAMESTATE::COULD NOT LOAD FONT");
	}
}

void GameState::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->font);
	this->pmenu->addButton("QUIT", 800.f, "Quit");
}

void GameState::initTileMap()
{
	//TO DO: add a checkh thats its the same size as mape made in Editor
	this->tileMap = new TileMap(static_cast<float>(this->stateData->gridSize), 20, 20, "Assets/Images/Tiles/Tilemap.png");
	this->tileMap->loadFromFile("tile_map.txt");
}

//Constuctors / destructors
GameState::GameState(StateData* state_data)
	:State(state_data), pmenu(nullptr)
{
	this->initDeferredRender();
	this->initView();
	this->initKeybinds();
	this->initTexures();
	this->initPlayers();
	this->initFonts();
	this->initPauseMenu();
	this->initTileMap();
}

GameState::~GameState()
{
	delete this->player;
	delete this->pmenu;
	delete this->tileMap;
}


//Functions
void GameState::updateView(const float& dt)
{
	//Rounding camera postion to nearest intiger to avoid screen tearing caued by floating point
	this->view.setCenter(
		std::floor(this->player->getCenterPosition().x),
		std::floor(this->player->getCenterPosition().y));
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime()) {
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
}

void GameState::updatePlayerInput(const float& dt)
{
	//think about those if else bc they preffer one direction over the other when 2 buttons are pressed at the same time

	//Player Input 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT")))) {
		this->player->move(1.f, 0.f, dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT")))) {
		this->player->move( -1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP")))) {
		this->player->move( 0.f, -1.f, dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN")))) {
		this->player->move( 0.f, 1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("ROLL")))) {
		this->player->roll();
	}


}

void GameState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonReleased("QUIT") ) {
		this->endState();
	}
}

void GameState::updateTileMap(const float& dt)
{
	this->tileMap->update();
	this->tileMap->updateCollision(this->player, dt);
}

void GameState::update(const float& dt)
{
	this->updateMousePosition(&this->view);
	this->updateInput(dt);
	this->updateKeytime(dt);

	if (!this->paused) { //unpaused update
		this->updatePlayerInput(dt);
		this->updateTileMap(dt); //collision handling
		this->player->update(dt);
		

		this->updateView(dt);
	}
	else {//pasued update

		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;	
	}
	this->renderTexture.clear();

	this->renderTexture.setView(this->view);
	this->tileMap->render(this->renderTexture, this->player->getGridPosition(this->stateData->gridSize));
	this->player->render(this->renderTexture);
	this->tileMap->renderDeffered(this->renderTexture);

	if (this->paused) {
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pmenu->render(this->renderTexture);
	}
	

	this->renderTexture.setView(this->view);


	//remove later for debbuing postion of player and relative postion of mause to the player
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(24);
	std::stringstream ss;
	ss << this->player->getPosition().x - this->mousePosView.x << "\n" << this->player->getPosition().y - this->mousePosView.y;
	mouseText.setString(ss.str());

	sf::Text playerText;
	playerText.setPosition(this->player->getPosition().x, this->player->getPosition().y - 100);
	playerText.setFont(this->font);
	playerText.setCharacterSize(24);
	std::stringstream sss;
	sss << this->player->getPosition().x << "\n" << this->player->getPosition().y;
	playerText.setString(sss.str());

	this->renderTexture.draw(mouseText);
	this->renderTexture.draw(playerText);
	
	//Final Render
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
	
}
