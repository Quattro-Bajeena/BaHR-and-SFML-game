
#include "stdafx.h"
#include "GameState.h"

//Initlaizers
void GameState::initDeferredRender()
{
	this->renderTexture.create(
		this->stateData.gfxSettings->resolution.width,
		this->stateData.gfxSettings->resolution.height
		);

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(sf::IntRect(
		0,
		0,
		this->stateData.gfxSettings->resolution.width,
		this->stateData.gfxSettings->resolution.height));
}


void GameState::initView() {

	this->view = sf::View(sf::Vector2f(0, 0), sf::Vector2f(this->window.getSize().x / this->stateData.scale.x, this->window.getSize().y / this->stateData.scale.y) );
}

void GameState::initVariables() {
	this->player = nullptr;

	this->lost = false;
	this->restart = false;
	this->start = false;

	this->cursorVisibility = false;
}

void GameState::initText()
{

}


void GameState::initPlayer()
{
	player = new Player(this->statistics.getName(),this->world->randomFreeTile());

}

void GameState::initSound()
{
	//this->music.openFromFile("Assets/Sounds/gwyn.wav");
	//this->endScreenMusic.openFromFile("Assets/Sounds/guts.wav");
}

void GameState::initGui()
{
	this->crosshair.setTexture(Assets::Get().textures.at("CROSSHAIR"));
	this->crosshair.setPosition(static_cast<sf::Vector2f>(this->mousePosWindow));
	this->crosshair.setScale(1.5 * this->stateData.scale.x, 1.5 * this->stateData.scale.x);

	this->pauseMenu = std::make_unique< PauseMenu>(this->window, Assets::Get().font, this->stateData.scale);
	this->pauseMenu->addButton("QUIT", 800.f, "Quit");

	
	this->healthBar = std::make_unique< gui::HealthBar>(50, 50, 300, 64, this->stateData.scale, Assets::Get().textures.at("HEARTS"), this->player->getHealth());
	this->healthBar2 = std::make_unique<gui::Bar>(50, 300, 300, 50, this->player->getMaxHealth(),
		true, this->stateData.scale,
		sf::Color::Red, sf::Color::Black, 8,
		Assets::Get().font);

	this->weaponSlot = std::make_unique<gui::WeaponSlot>(1450, 800, 400, 200, 100, 100,
		this->player->getMagazineInfo(), Assets::Get().weaponTextures, this->player->getCurrentGunModel(),
		this->stateData.scale, Assets::Get().font);

	this->reloadBar = std::make_unique<gui::ReloadBar>(-100, 100, 300, 50, this->stateData.scale);

	this->texts["SCORE"].setFont(Assets::Get().font);
	this->texts["SCORE"].setCharacterSize(50 * this->stateData.scale.x);
	this->texts["SCORE"].setPosition(50.f * this->stateData.scale.x, 120.f * this->stateData.scale.y);
	this->texts["SCORE"].setOutlineThickness(static_cast<int>(3.f * this->stateData.scale.x));
	this->texts["SCORE"].setOutlineColor(sf::Color::Black);

	this->texts["HEALTH"].setFont(Assets::Get().font);
	this->texts["HEALTH"].setCharacterSize(50 * this->stateData.scale.x);
	this->texts["HEALTH"].setPosition(50.f * this->stateData.scale.x, 170.f * this->stateData.scale.y);
	this->texts["HEALTH"].setOutlineThickness(static_cast<int>(3.f*this->stateData.scale.x));
	this->texts["HEALTH"].setOutlineColor(sf::Color::Black);

	this->texts["PLAYER_INFO"].setFont(Assets::Get().font);
	this->texts["PLAYER_INFO"].setCharacterSize(50 * this->stateData.scale.x);
	this->texts["PLAYER_INFO"].setOutlineThickness(static_cast<int>(3.f * this->stateData.scale.x));
	this->texts["PLAYER_INFO"].setOutlineColor(sf::Color::Black);
}


void GameState::initWorld()
{
	this->world = std::make_unique< World>(this->stateData.folderPaths.at("WORLD")+"test_world.txt",Assets::Get().textures.at("TILE_MAP"),this->statistics);
	
}

//Constuctors / destructors
GameState::GameState(StateData& state_data, GameStatistics& stats)
	:State(state_data), statistics(stats), playerName(stats.getName())
{
	this->initVariables();
	this->initWorld();
	this->initDeferredRender();
	this->initPlayer();
	this->initView();
	this->initText();
	this->initSound();
	this->initGui();

	this->window.setMouseCursorVisible(false);
	
}

GameState::~GameState()
{
	delete this->player;
	std::cout << "Ending GameState" << "\n";

}

//Functions

void GameState::loose()
{
	this->statistics.update();
	this->statistics.save();
	
	this->states.push(new EndScreenState(this->stateData, this->statistics));
	this->endState();
}


void GameState::updateView(const float& dt)
{
	this->view.setCenter(
		util::clamp(
			std::floor(this->player->getCenterPosition().x),
			this->world->getRoomBounds().left + this->view.getSize().x/2,
			this->world->getRoomBounds().left+ this->world->getRoomBounds().width - this->view.getSize().x/2),
		util::clamp(
			std::floor(this->player->getCenterPosition().y),
			this->world->getRoomBounds().top  + this->view.getSize().y/2,
			this->world->getRoomBounds().top + this->world->getRoomBounds().height- this->view.getSize().y/2)
		);
	
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->getKeytime()) {
		if (this->paused==false) {
			this->pauseState();	
		}
		else {
			this->unpauseState();
		}		
	}

	/*if (this->scroll != 0) {
		this->view.zoom(1  + (-1 * this->scroll * 0.05f));
	}*/

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		this->view.zoom(1 + (0.5f * dt));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->view.zoom(1 - (0.5f * dt));
	}
}

void GameState::updateGui(const float& dt)
{
	std::stringstream ss;
	ss << this->player->getGridPosition(this->stateData.gridSize).x << " | "
		<< this->player->getGridPosition(this->stateData.gridSize).y << "\n";

	this->texts.at("PLAYER_INFO").setString(ss.str());
	this->texts.at("PLAYER_INFO").setPosition(this->player->getCenterPosition().x, this->player->getCenterPosition().y);
	
	this->texts.at("SCORE").setString("ECTS: " + std::to_string(this->statistics.getCurrentScore()));
	this->texts.at("HEALTH").setString("Health: " + std::to_string(this->player->getHealth()));

	this->healthBar->update(this->player->getHealth());
	this->healthBar2->update(this->player->getHealth());
	this->weaponSlot->update(this->player->getMagazineInfo(), dt);

	this->reloadBar->update(this->player->getCenterPosition(), dt);
	this->crosshair.setPosition(static_cast<sf::Vector2f>(this->mousePosWindow));
	
}

void GameState::updatePauseMenuButtons(const float& dt)
{
	if (this->pauseMenu->isButtonReleased("QUIT")) {
		this->statistics.reset();
		this->endState();
	}
}

void GameState::updatePlayerInput(const float& dt)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->player->move(-1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->player->move(1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->player->move(0.f, -1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->player->move(0.f, 1.f, dt);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		this->player->roll(dt);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		player->shoot(mousePosView);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		if (this->player->canReload()) {
			this->player->reload();
			this->reloadBar->reloadAnimationStart(this->player->getReloadTime());
		}
	}
	//Swapping weapons with mouse scroll
	if (this->scroll != 0) {
		this->player->changeCurrGunAdjacent(this->scroll);
		this->weaponSlot->setCurrentGun(this->player->getCurrentGunModel(), this->player->getMagazineInfo());
	}

	
	//One option for swapping weapons by 1-9 numbers
	if (this->textReceived == true && this->unicodeText >= 48 && this->unicodeText <= 57) {
		this->player->changeCurrGunPos(this->unicodeText - 49);
		this->weaponSlot->setCurrentGun(this->player->getCurrentGunModel(), this->player->getMagazineInfo());
		this->textReceived = false;
	}

	//Alternative
	/*for (int key = sf::Keyboard::Key::Num1; key <= sf::Keyboard::Key::Num9; key++) {
		if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key))) {
			this->player->changeCurrGunPos(key - 27);
		}
	}*/
	

}

void GameState::updateTileMap(const float& dt)
{

	
}



//Public main gampelay
void GameState::update(const float& dt) {

	//GameStateplay
	this->updateMousePosition(&this->view); //Mouse Positions
	this->updateInput(dt); //menu buttons
	this->updateKeytime(dt);
	

	if (this->paused == false) { //unpaused
		
		this->updateGui(dt);

		this->updatePlayerInput(dt); //Player move and shoot

		this->world->update(this->player, dt); //Updating Enemies and PowerUps
		this->world->updateCollision(this->player, dt); //Player's collision with map
		this->world->updateBulletCollisions(this->player->getBullets());//Player's bullets collison with map and enemies

		this->player->update(dt);
		this->player->updateWeapon(this->mousePosView, dt); //updating gun position, handling reloads
		this->player->updateBullets(this->world->getEnemies(), dt); //Collision of player bullets withe enemies
		

		this->updateView(dt); //camera following player
		if (player->isDead() == true) { //chechks is player is dead from any causes
			this->loose();
		}
	}
	else {
		this->updatePauseMenuButtons(dt);
		this->pauseMenu->update(this->mousePosWindow);
	}

	this->resetScrollWheel();

}



void GameState::render(sf::RenderTarget* target)  const {

	this->renderTexture.clear();
	this->renderTexture.setView(this->view);

	this->world->render(this->renderTexture,
		this->player->getGridPosition(this->stateData.gridSize));


	//render player and players bullets
	this->player->render(this->renderTexture);

	//Render Tiles above the player
	this->world->renderDeffered(this->renderTexture);
	target->draw(this->texts.at("PLAYER_INFO"));
	this->renderGui(&this->renderTexture);

	//Final Render
	
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}

void GameState::renderGui(sf::RenderTarget* target) const
{
	//UI that follows tha player
	//target->draw(this->texts.at("PLAYER_INFO"));
	this->reloadBar->render(*target);

	//Static on screen UI
	target->setView(target->getDefaultView()); // render UI
	target->draw(this->texts.at("SCORE"));
	target->draw(this->texts.at("HEALTH"));
	
	this->healthBar->render(target);
	this->healthBar2->render(true, *target);
	this->weaponSlot->render(*target);
	

	
	if (this->paused == true) {
		this->pauseMenu->render(*target);
	}
	else {
		target->draw(this->crosshair);
	}
	target->setView(this->view);
}

