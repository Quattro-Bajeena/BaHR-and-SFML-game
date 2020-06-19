#include "stdafx.h"
#include "MenuState.h"


//Initializer function
void MenuState::initVariables()
{
	this->cursorVisibility = true;
}

void MenuState::initBackground()
{
	this->background.setSize(static_cast<sf::Vector2f>(this->window.getSize()));
	this->background.setTexture(&Assets::Get().textures.at("MENU_BACKGROUND"));
}

void MenuState::initGui()
{
	mouseText.setFont(Assets::Get().font);
	mouseText.setCharacterSize(24);
}


void MenuState::initButtons()
{
	this->buttons["GAME_STATE"] = std::make_unique<gui::Button>( 
		1400.f, 140.f, 410.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "New Game", 55, 
		sf::Color(10, 240, 10, 200), sf::Color(230, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(120, 120, 120, 150), sf::Color(150, 200, 150, 200), sf::Color(20, 20, 20, 50)
		);

	this->buttons["SETTINGS_STATE"] = std::make_unique< gui::Button>(
		1400.f, 300.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Settings", 50,
		sf::Color(240, 240, 15, 200), sf::Color(255, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(180, 180, 130, 200), sf::Color(20, 20, 20, 50));

	/*this->buttons["SERVER_STATE"] = std::make_unique< gui::Button>(
		1400.f, 460.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Create\nServer", 45,
		sf::Color(240, 240, 15, 200), sf::Color(255, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(180, 180, 130, 200), sf::Color(20, 20, 20, 50));

	this->buttons["MULTIPLAYER_STATE"] = std::make_unique< gui::Button>(
		1610.f, 460.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Multiplayer", 40,
		sf::Color(240, 240, 15, 200), sf::Color(255, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(180, 180, 130, 200), sf::Color(20, 20, 20, 50));*/

	this->buttons["EDITOR_STATE"] = std::make_unique< gui::Button>(
		1610.f, 300, 200.f, 70.f, this->stateData.scale,
		&Assets::Get().font, "Map Editor", 35,
		sf::Color(35, 20, 50, 210), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(140, 110, 160, 200), sf::Color(20, 20, 20, 50));

	this->buttons["WORLD_EDITOR_STATE"] = std::make_unique< gui::Button>(
		1610.f, 375, 200.f, 70.f, this->stateData.scale,
		&Assets::Get().font, "Level Editor", 35,
		sf::Color(35, 20, 50, 210), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(140, 110, 160, 200), sf::Color(20, 20, 20, 50));

	this->buttons["INFO_STATE"] = std::make_unique< gui::Button>(
		1400.f, 460.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Tutorial", 50,
		sf::Color(255, 180, 50, 230), sf::Color(255, 220, 210, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(200, 180, 130, 200), sf::Color(20, 20, 20, 50));

	this->buttons["EXIT_STATE"] = std::make_unique< gui::Button>(
		1610.f, 460.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Quit", 50,
		sf::Color(255, 65, 65, 230), sf::Color(255, 220, 220, 255), sf::Color(10, 10, 10, 200),
		sf::Color(90, 90, 90, 150), sf::Color(200, 150, 150, 200), sf::Color(20, 20, 20, 50));

	this->buttons["MUTE_MUSIC"] = std::make_unique< gui::Button>(
		50, 950, 100, 80, this->stateData.scale,
		&Assets::Get().font, "Mute", 40,
		sf::Color(220, 220, 220, 230), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 200),
		sf::Color(90, 90, 90, 150), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 50));

	

	this->textBox = std::make_unique< gui::TextBox>(1400.f, 620.f, 410.f, 80.f, this->stateData.scale,
		Assets::Get().font, "insert_name", 50, 15, 
		sf::Color(40, 40, 40, 255), sf::Color(20, 20, 20, 255), sf::Color(5, 5, 5, 255),
		sf::Color(200, 200, 200, 200), sf::Color(230, 230, 230, 230), sf::Color(240, 240, 240, 255));

	
}

void MenuState::initText()
{
	this->fontSize1 = 60;
	this->texts["SCORE_BOARD"].setFont(Assets::Get().font);
	this->texts["SCORE_BOARD"].setCharacterSize(fontSize1);
	this->texts["SCORE_BOARD"].setPosition(100.f , 50.f );
	this->texts["SCORE_BOARD"].setOutlineThickness(7.f );
	this->texts["SCORE_BOARD"].setFillColor(sf::Color(200, 230, 30, 255));
	this->texts["SCORE_BOARD"].setOutlineColor(sf::Color::Black);
	
		
	this->texts["ENEMY_KILLS"].setFont(Assets::Get().font);
	this->texts["ENEMY_KILLS"].setCharacterSize(fontSize1);
	this->texts["ENEMY_KILLS"].setPosition(700.f , 50.f );
	this->texts["ENEMY_KILLS"].setOutlineThickness(7.f );
	this->texts["ENEMY_KILLS"].setFillColor(sf::Color(200, 230, 30, 255));
	this->texts["ENEMY_KILLS"].setOutlineColor(sf::Color::Black);

	this->texts["CREATOR_INFO"].setFont(Assets::Get().font);
	this->texts["CREATOR_INFO"].setPosition(sf::Vector2f(1200, 850 ));
	this->texts["CREATOR_INFO"].setCharacterSize(fontSize1 * 0.6);
	this->texts["CREATOR_INFO"].setFillColor(sf::Color(250, 200, 50));
	this->texts["CREATOR_INFO"].setOutlineThickness(3);
	this->texts["CREATOR_INFO"].setOutlineColor(sf::Color::Black);

	


	for (auto& [key, text] : this->texts) {
		this->orgTextParams[key].first = text.getPosition();
		this->orgTextParams[key].second = text.getCharacterSize();
	}

	std::string text = R"(
	'Boy and His Robot' (name subject to change)
	 ver. alpha 0.8
		-Mateusz Oleszek, 2020
	)";
	this->texts["CREATOR_INFO"].setString(text);
}

MenuState::MenuState(StateData& state_data,GameStatistics& statistics, AudioManager& audio)
	:State(state_data, audio), statistics(statistics)
{
	this->initVariables();
	this->initBackground();
	this->initGui();
	this->initButtons();
	this->initText();
	this->refreshGui();
	this->audio.playMusic("menu_track");
}

MenuState::~MenuState()
{
	
}



void MenuState::refreshGui() //updating after going back from finished game or changing graphics settings
{
	this->audio.resumeMusic("menu_track");

	for (auto& [key, text] : this->texts) {
		text.setCharacterSize(this->orgTextParams.at(key).second * this->stateData.scale.x);
		text.setPosition(this->orgTextParams.at(key).first.x * this->stateData.scale.x, this->orgTextParams.at(key).first.y * this->stateData.scale.y);
		text.setOutlineThickness(static_cast<int>(7.f * this->stateData.scale.x));
	}

	for (auto const& [key, button] : this->buttons) {
		button->scale(this->stateData.scale);
	}
	this->textBox->scale(this->stateData.scale);

	this->background.setSize(static_cast<sf::Vector2f>(this->window.getSize()));
	this->background.setTexture(&Assets::Get().textures.at("MENU_BACKGROUND"));

	std::vector<std::pair<std::string, int>> score_board = this->statistics.getBestScores(5);
	std::stringstream score_stream;
	score_stream << "BEST SCORES" << "\n" << "-----------" << "\n";
	for (int i = 0; i < score_board.size(); ++i) {
		score_stream << i+1 << ".  "<< score_board[i].first<<":  " << score_board[i].second <<" pts" <<"\n";
	}
	this->texts.at("SCORE_BOARD").setString(score_stream.str());

	std::stringstream kills_stream;
	std::map<enemyType, int> kills = this->statistics.getTotalEnemyKills();
	kills_stream 
		<< "TOTAL ENEMY KILLS" << "\n" <<"-----------------"<<"\n"
		<< "REGULAR: " << kills.at(enemyType::REGULAR) << "\n"
		<< "COMMANDO: " << kills.at(enemyType::COMMANDO) << "\n"
		<< "STATIONARY: " << kills.at(enemyType::STATIONARY) << "\n"
		<< "BOSS: " << kills.at(enemyType::BOSS) << "\n";
	this->texts.at("ENEMY_KILLS").setString(kills_stream.str());
}

void MenuState::updateInput(const float& dt)
{


}

void MenuState::updateButtons()
{
	/*Updates all th buttons in the state and handles their functionality*/
	for (auto& it : this->buttons) {
		it.second->update(this->mousePosWindow);
	}

	//New game
	if (this->buttons["GAME_STATE"]->isReleased()) {

		if (this->textBox->isStringDefault())
			this->statistics.setNameDefault();
		else this->statistics.setName(this->textBox->getText());

		this->states.push(new GameState(this->stateData, this->statistics, this->audio));
	}

	//Level Editor
	if (this->buttons["EDITOR_STATE"]->isReleased()) {
		this->states.push(new EditorState(this->stateData, this->audio));
	}

	if (this->buttons["WORLD_EDITOR_STATE"]->isReleased()) {
		this->states.push(new WorldEditorState(this->stateData, this->audio));
	}

	//Settings
	if (this->buttons["SETTINGS_STATE"]->isReleased()) {
		this->states.push(new SettingsState(this->stateData, this->audio));
	}

	if (this->buttons["INFO_STATE"]->isReleased()) {
		this->states.push(new InfoState(this->stateData, this->audio));
	}

	/*if (this->buttons["SERVER_STATE"]->isReleased()) {
		this->states.push(new MultiplayerServerState(this->stateData, this->statistics, this->audio));
	}

	if (this->buttons["MULTIPLAYER_STATE"]->isReleased()) {
		this->states.push(new MultiplayerClientState(this->stateData, this->audio));
	}*/

	//Quit the state
	if (this->buttons["EXIT_STATE"]->isReleased()) {
		this->endState();
	}
	if (this->buttons["MUTE_MUSIC"]->isReleased()) {
		this->audio.toggleMuteMusic();
	}
}

void MenuState::updateGui()
{

	if (this->textReceived == true) {
		this->textBox->typeText(this->unicodeText);
		this->textReceived = false;
	}

	this->textBox->update(this->mousePosWindow);

	
}

void MenuState::update(const float& dt)
{

	this->updateMousePosition();
	this->updateInput(dt);
	this->updateButtons();
	this->updateGui();
	this->resetScrollWheel();

}

void MenuState::renderButtons(sf::RenderTarget& target) const
{

	for (auto& [key, button] : this->buttons) {
		button->render(target);
	}

	this->textBox->render(target);
}

void MenuState::render(sf::RenderTarget* target) const 
{
	if (!target) {
		target = &this->window;
	}

	target->draw(this->background);

	this->renderButtons(*target);
	for (auto& [key, text] : this->texts) {
		target->draw(text);
	}

	target->draw(mouseText);
}

