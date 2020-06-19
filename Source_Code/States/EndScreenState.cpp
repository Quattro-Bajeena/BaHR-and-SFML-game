#include "stdafx.h"
#include "EndScreenState.h"

//Initliaziers
void EndScreenState::initVariables()
{
	this->cursorVisibility = true;
}

void EndScreenState::initBackground()
{
	this->background.setSize(static_cast<sf::Vector2f>(this->window.getSize()));
	
	this->background.setTexture(&Assets::Get().textures.at("END_SCREEN_BACKGROUND"));
}


void EndScreenState::initButtons()
{
	this->buttons["RETRY"] = new gui::Button(
		1610.f, 140.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "New Game", 45,
		sf::Color(10, 240, 10, 200), sf::Color(230, 255, 230, 255), sf::Color(10, 10, 10, 200),
		sf::Color(120, 120, 120, 150), sf::Color(150, 200, 150, 200), sf::Color(20, 20, 20, 50)
		);

	this->buttons["MENU_STATE"] = new gui::Button(
		1610.f, 300.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Quit", 50,
		sf::Color(255, 65, 65, 200), sf::Color(255, 220, 220, 255), sf::Color(10, 10, 10, 200),
		sf::Color(90, 90, 90, 150), sf::Color(200, 150, 150, 200), sf::Color(20, 20, 20, 50));

	
	
}

void EndScreenState::initText()
{
	this->texts["SCORE"].setFont(Assets::Get().font);
	this->texts["SCORE"].setCharacterSize(80 * this->stateData.scale.x);
	this->texts["SCORE"].setPosition(100.f * this->stateData.scale.x, 50.f * this->stateData.scale.y);
	this->texts["SCORE"].setOutlineThickness(static_cast<int>(7.f*this->stateData.scale.x));
	this->texts["SCORE"].setFillColor(sf::Color(200, 230, 30, 255));
	this->texts["SCORE"].setOutlineColor(sf::Color::Black);

	this->texts["ENEMY_KILLS"].setFont(Assets::Get().font);
	this->texts["ENEMY_KILLS"].setCharacterSize(80 * this->stateData.scale.x);
	this->texts["ENEMY_KILLS"].setPosition(600.f * this->stateData.scale.x, 50.f * this->stateData.scale.y);
	this->texts["ENEMY_KILLS"].setOutlineThickness(static_cast<int>(7.f * this->stateData.scale.x));
	this->texts["ENEMY_KILLS"].setFillColor(sf::Color(200, 230, 30, 255));
	this->texts["ENEMY_KILLS"].setOutlineColor(sf::Color::Black);

}

void EndScreenState::initGui()
{
	int score = this->statistics.getCurrentScore();
	std::stringstream score_stream;
	score_stream << "THE SCORE: " << score<<"\n"
		<< "-------------" << "\n";
	this->texts.at("SCORE").setString(score_stream.str());

	std::stringstream kills_stream;
	std::map<enemyType, int> kills = this->statistics.getCurrentEnemyKills();
	kills_stream << "ENEMY KILLS" << "\n" << "-----------------" << "\n"
		<< "REGULAR: " << kills.at(enemyType::REGULAR) << "\n"
		<< "COMMANDO: " << kills.at(enemyType::COMMANDO) << "\n"
		<< "STATIONARY: " << kills.at(enemyType::STATIONARY) << "\n"
		<< "BOSS: " << kills.at(enemyType::BOSS) << "\n";
	this->texts.at("ENEMY_KILLS").setString(kills_stream.str());
}

//Constructor
EndScreenState::EndScreenState(StateData& state_data, GameStatistics& stats, AudioManager& audio)
	:State(state_data, audio), statistics(stats)
{
	this->initVariables();
	this->initBackground();
	this->initButtons();
	this->initText();
	this->initGui();
}

EndScreenState::~EndScreenState()
{
}


//Functions
void EndScreenState::updateInput(const float& dt)
{
}

void EndScreenState::updateButtons()
{
	/*Updates all th buttons in the state and handles their functionality*/
	for (const auto& [key, button] : this->buttons) {
		button->update(this->mousePosWindow);
	}

	//Retry
	if (this->buttons["RETRY"]->isReleased()) {
		this->statistics.reset();
		this->endState();
		this->states.push(new GameState(this->stateData,this->statistics, this->audio));
	}

	//Return to menu
	if (this->buttons["MENU_STATE"]->isReleased()) {
		this->statistics.reset();
		this->endState();
	}

}



void EndScreenState::update(const float& dt)
{
	this->updateMousePosition();
	this->updateInput(dt);
	this->updateButtons();
	this->resetScrollWheel();
}

void EndScreenState::renderButtons(sf::RenderTarget& target) const
{
	for (const auto& [key, button] : this->buttons) {
		button->render(target);
	}
}

void EndScreenState::render(sf::RenderTarget* target) const
{
	if (!target) {
		target = &this->window;
	}
	target->draw(this->background);
	this->renderButtons(*target);

	target->draw(this->texts.at("SCORE"));
	target->draw(this->texts.at("ENEMY_KILLS"));
		
}
