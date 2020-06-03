#include "stdafx.h"
#include "Game.h"

//Static functions

//Initializer functions

void Game::initVariables()
{
    this->window = nullptr;
    this->dt = 0.f;
    this->gridSize = 192;
}

void Game::initGraphicSettings()
{
    this->gfxSettings.loadFromFile("Config/graphics.ini");
}

void Game::initStateData()
{
    this->stateData.window = this->window;
    this->stateData.gfxSettings = &this->gfxSettings;
    this->stateData.supportedKeys = &this->supportedKeys;
    this->stateData.states = &this->states;
    this->stateData.gridSize = this->gridSize;
}

void Game::initWindow()
{
    /*Creates a SFML window using options from a window.ini file*/

    if(this->gfxSettings.fullscreen)
	    this->window = new sf::RenderWindow(this->gfxSettings.resolution,
            this->gfxSettings.title,
            sf::Style::Fullscreen,
            this->gfxSettings.contexSettings);
    else
        this->window = new sf::RenderWindow(this->gfxSettings.resolution,
            this->gfxSettings.title,
            sf::Style::Titlebar| sf::Style::Close,
            this->gfxSettings.contexSettings);

    this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
    this->window->setVerticalSyncEnabled(this->gfxSettings.vsync);
}

void Game::initKeys()
{
    std::ifstream ifs("Config/supported_keys.ini");
    if (ifs.is_open()) {
        std::string key = "";
        int key_value = 0;
        while (ifs >> key >> key_value) {
            this->supportedKeys[key] = key_value;
        }
    }

    ifs.close();

    //DEBUG
    for (auto i : this->supportedKeys) {
        std::cout << i.first << " " << i.second << "\n";
    }
    
}

void Game::initStates()
{
    this->states.push(new MainMenuState(&this->stateData));
}

//Constructors/Destructors
Game::Game() { 
    this->initVariables();
    this->initGraphicSettings();
    this->initWindow();
    this->initKeys();
    this->initStateData();
    this->initStates();
}

Game::~Game()
{
	delete this->window;

    while (!this->states.empty()){
        delete this->states.top();
        this->states.pop();
    }
        
}

//Function


void Game::endApplication()
{
    std::cout << "Ending Application" << "\n";
}

void Game::updateDt()
{
    /*Updates the dt variab;le with the time it takes to update adn render one frame*/
    this->dt = this->dtClock.restart().asSeconds();
    
}


void Game::updateSFMLEvents()
{
    while (this->window->pollEvent(this->sfEvent)) {
        if (this->sfEvent.type == sf::Event::Closed) {
            this->window->close();
        }
    }
}

void Game::update()
{
    this->updateSFMLEvents();
    if (this->states.empty() == false ) {

        if (this->window->hasFocus()) {

            this->states.top()->update(this->dt);
            if (this->states.top()->getQuit()) {

                delete this->states.top();
                this->states.pop();
            }
        }
        
    }
    //Application end
    else {
        this->endApplication();
        this->window->close();
    }
    
    
}

void Game::render()
{
    this->window->clear(sf::Color(40,40,40));

    //Render items
    if (!this->states.empty()) {
        this->states.top()->render(this->window);
    }

    this->window->display();
}

void Game::run()
{
    while (this->window->isOpen()) {
        this->updateDt();
        
        this->update();
        this->render();
    }
}
