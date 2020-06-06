#include "stdafx.h"
#include "Game.h"

void Game::initVariables()
{
    srand(time(NULL));
    this->window = nullptr;
    this->dt = 0.f;
    this->gridSize = 192;
}


void Game::initWindow()
{
    /*Creates a SFML window using options from a window.ini file*/

    if (this->gfxSettings.fullscreen)
        this->window = new sf::RenderWindow(this->gfxSettings.resolution,
            this->gfxSettings.title,
            sf::Style::Fullscreen,
            this->gfxSettings.contexSettings);
    else
        this->window = new sf::RenderWindow(this->gfxSettings.resolution,
            this->gfxSettings.title,
            sf::Style::Titlebar | sf::Style::Close,
            this->gfxSettings.contexSettings);

    this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
    this->window->setVerticalSyncEnabled(this->gfxSettings.vsync);
}

void Game::initGraphicSettings()
{
    this->gfxSettings.loadFromFile("Config/graphics.ini");
}

void Game::initStateData()
{
    this->stateData.window = this->window;
    this->stateData.gfxSettings = &this->gfxSettings;
    //this->stateData.supportedKeys = &this->supportedKeys;
    this->stateData.states = &this->states;
    this->stateData.gridSize = this->gridSize;
    this->stateData.scale = sf::Vector2f(this->gfxSettings.resolution.width / 1920.f, this->gfxSettings.resolution.height / 1080.f);
}


void Game::initStates()
{
    this->states.push(new MenuState(this->stateData, *this->statistics ));
}

void Game::initStatistics()
{
    std::map<std::string, std::string> file_paths = { { "SCORE_BOARD" , "Statistics/score_board.txt" } , {"ENEMY_KILLS", "Statistics/enemy_kills.txt"} };
    this->statistics = std::make_unique<GameStatistics>(file_paths);
    this->statistics->load();
}

void Game::initAssets()
{
    Assets::Get().font.loadFromFile("Assets/Fonts/impact.ttf");

    //Entites
    Assets::Get().textures["PLAYER_SHEET"].loadFromFile("Assets/Images/Sprites/Player/player_sprite_sheet.png");
    Assets::Get().textures["REGULAR_ENEMY"].loadFromFile("Assets/Images/Sprites/Enemies/regular enemy spritesheet.png");
    Assets::Get().textures["COMMANDO_ENEMY"].loadFromFile("Assets/Images/Sprites/Enemies/commando enemy spritesheet.png");
    Assets::Get().textures["STATIONARY_ENEMY"].loadFromFile("Assets/Images/Sprites/Enemies/stationary enemy spritesheet.png");
    Assets::Get().textures["BOSS_ENEMY"].loadFromFile("Assets/Images/Sprites/Enemies/boss enemy spritesheet.png");

    //Objects, UI
    Assets::Get().textures["POWER_UP"].loadFromFile("Assets/Images/Sprites/PowerUpIcons.png");
    Assets::Get().textures["HEARTS"].loadFromFile("Assets/Images/Sprites/hearts_sheet.png");
    Assets::Get().textures["CROSSHAIR"].loadFromFile("Assets/Images/Sprites/crosshair.png");

    //Backgrouds
    Assets::Get().textures["MENU_BACKGROUND"].loadFromFile("Assets/Images/Backgrounds/background.png");
    Assets::Get().textures["END_SCREEN_BACKGROUND"].loadFromFile("Assets/Images/Backgrounds/end_screen_background.png");

    //Tiles
    Assets::Get().textures["TILE_MAP"].loadFromFile("Assets/Images/Tiles/Tilemap.png");

    //Guns
    Assets::Get().textures["NORMAL_GUN"].loadFromFile("Assets/Images/Sprites/Weapons/normal gun.png");
    Assets::Get().textures["REVOLVER_GUN"].loadFromFile("Assets/Images/Sprites/Weapons/revolver.png");
    Assets::Get().textures["SPREAD_GUN"].loadFromFile("Assets/Images/Sprites/Weapons/spreadgun.png");
    Assets::Get().textures["SHOTGUN_GUN"].loadFromFile("Assets/Images/Sprites/Weapons/shotgun.png");
    Assets::Get().textures["CANON_GUN"].loadFromFile("Assets/Images/Sprites/Weapons/canon gun.png");
    Assets::Get().textures["MAGIC_GUN"].loadFromFile("Assets/Images/Sprites/Weapons/magic gun.png");

    Assets::Get().weaponTextures[gunModels::NORMAL] = &Assets::Get().textures.at("NORMAL_GUN");
    Assets::Get().weaponTextures[gunModels::REVOLVER] = &Assets::Get().textures.at("REVOLVER_GUN");
    Assets::Get().weaponTextures[gunModels::SPREAD] = &Assets::Get().textures.at("SPREAD_GUN");
    Assets::Get().weaponTextures[gunModels::SHOTGUN] = &Assets::Get().textures.at("SHOTGUN_GUN");
    Assets::Get().weaponTextures[gunModels::CANNON] = &Assets::Get().textures.at("CANON_GUN");
    Assets::Get().weaponTextures[gunModels::MAGIC] = &Assets::Get().textures.at("MAGIC_GUN");

   //Sounds
   Assets::Get().soundBuffers["HIT"].loadFromFile("Assets/Sounds/uguu.wav");
   //Assets::Get().sounds["HIT"].setBuffer(Assets::Get().soundBuffers.at("HIT"));
}



//Constructors/Destructors
Game::Game() {
    this->initVariables();
    this->initGraphicSettings();
    this->initWindow();
    this->initStateData();
    this->initAssets();
    this->initStatistics();
    this->initStates(); //last
    
}

Game::~Game()
{
    delete this->window;

    while (!this->states.empty()) {
        delete this->states.top();
        this->states.pop();
    }


}

//Function


void Game::endApplication()
{
    //TO DO something?
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
            this->endApplication();
            this->window->close();
        }
        if (this->sfEvent.type == sf::Event::MouseWheelMoved) {
            if (this->states.empty() == false)
                this->states.top()->updateScrollWheel(sfEvent.mouseWheel.delta);
        }
        if (this->sfEvent.type == sf::Event::TextEntered) {
            this->states.top()->typeText(sfEvent.text.unicode);
            
        }
    }
}

void Game::update()
{
    this->updateSFMLEvents();
    if (this->states.empty() == false) {
        //Update State at the top
        this->states.top()->update(this->dt);

        while(this->states.empty() == false && this->states.top()->getQuit()) {//must be in that order so it wont check getQuit on empty stakc

            //deleting state at the top
            delete this->states.top();
            this->states.pop();

            //refereshes the state that we're going back to
            if (this->states.empty() == false)
                this->states.top()->refreshGui(); 
        }
 
    }
    //Application end
    else {
        this->endApplication();
        this->window->close();
    }

}

void Game::render() const
{
    this->window->clear(sf::Color(40, 40, 40));

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