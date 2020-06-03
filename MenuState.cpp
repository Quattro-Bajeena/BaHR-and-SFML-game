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

}


void MenuState::initButtons()
{
	this->buttons["GAME_STATE"] = std::make_unique<gui::Button>( 
		1400.f, 140.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "New Game", 45, 
		sf::Color(10, 240, 10, 200), sf::Color(230, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(120, 120, 120, 150), sf::Color(150, 200, 150, 200), sf::Color(20, 20, 20, 50)
		);

	this->buttons["SETTINGS_STATE"] = std::make_unique< gui::Button>(
		1400.f, 300.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Settings", 50,
		sf::Color(240, 240, 15, 200), sf::Color(255, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(180, 180, 130, 200), sf::Color(20, 20, 20, 50));

	this->buttons["EDITOR_STATE"] = std::make_unique< gui::Button>(
		1610.f, 140.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Editor", 50,
		sf::Color(15, 15, 15, 200), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 50));

	this->buttons["EXIT_STATE"] = std::make_unique< gui::Button>(
		1610.f, 300.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Quit", 50,
		sf::Color(255, 65, 65, 200), sf::Color(255, 220, 220, 255), sf::Color(10, 10, 10, 200),
		sf::Color(90, 90, 90, 150), sf::Color(200, 150, 150, 200), sf::Color(20, 20, 20, 50));

	this->textBox = std::make_unique< gui::TextBox>(1400.f, 480.f, 410.f, 80.f, this->stateData.scale,
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

	this->textsOrgPos["SCORE_BOARD"] = this->texts.at("SCORE_BOARD").getPosition();
	this->textsOrgPos["ENEMY_KILLS"] = this->texts.at("ENEMY_KILLS").getPosition();
}

MenuState::MenuState(StateData& state_data,GameStatistics& statistics)
	:State(state_data), statistics(statistics)
{
	this->initVariables();
	this->initBackground();
	this->initGui();
	this->initButtons();
	this->initText();
	this->refreshGui();

}

MenuState::~MenuState()
{
	
}



void MenuState::refreshGui() //updating after going back from finished game or changing graphics settings
{
	for (auto& [key, text] : this->texts) {
		text.setCharacterSize(this->fontSize1 * this->stateData.scale.x);
		text.setPosition(this->textsOrgPos.at(key).x * this->stateData.scale.x, this->textsOrgPos.at(key).y * this->stateData.scale.y);
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
		this->statistics.setName(this->textBox->isStringDefault() ? "-unknown-" : this->textBox->getText());
		this->states.push(new GameState(this->stateData, this->statistics));
	}

	//Level Editor
	if (this->buttons["EDITOR_STATE"]->isReleased()) {
		this->states.push(new EditorState(this->stateData));
	}

	//Settings
	if (this->buttons["SETTINGS_STATE"]->isReleased()) {
		this->states.push(new SettingsState(this->stateData));
	}

	//Quit the state
	if (this->buttons["EXIT_STATE"]->isReleased()) {
		this->endState();
	}
}

void MenuState::updateGui()
{

	if (this->textReceived == true) {
		this->textBox->typeText(this->unicodeText);
		this->textReceived = false;
	}

	this->textBox->update(this->mousePosWindow);

	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(Assets::Get().font);
	mouseText.setCharacterSize(24);
	std::stringstream ss;
	ss << this->mousePosView.x << "\n" << this->mousePosView.y;
	mouseText.setString(ss.str());
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

	for (auto& it : this->buttons) {
		it.second->render(target);
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
	target->draw(this->texts.at("SCORE_BOARD"));
	target->draw(this->texts.at("ENEMY_KILLS"));
	target->draw(mouseText);
}

