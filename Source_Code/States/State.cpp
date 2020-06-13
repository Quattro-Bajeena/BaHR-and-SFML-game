
#include "stdafx.h"
#include "State.h"


State::State(StateData& state_data, AudioManager& audio)
	: stateData(state_data),audio(audio),
	window(*state_data.window),
	quit(false), paused(false), states(*state_data.states), gridSize(state_data.gridSize),
	keytime(0.f), keytimeMax(0.3f), clickTime(0.f), clickTimeMax(0.1f)
{
	this->refreshGui();
	this->window.setMouseCursorVisible(true);
	this->scroll = 0;
	this->scrollActive = false;
	this->textReceived = false;
}


State::~State()
{
}

const bool& State::getQuit() const
{
	return this->quit;
}

const bool State::getClickTime()
{
	if (this->clickTime >= this->clickTimeMax) {
		this->clickTime = 0.f;
		return true;
	}
	else return false;
}

const bool State::getKeytime()
{
	if (this->keytime >= this->keytimeMax) {
		this->keytime = 0.f;
		return true;
	}
	else return false;
}


void State::endState()
{
	this->audio.stopMusic();
	//std::cout << "Ending this State" << "\n";
	this->quit = true;

}

void State::pauseState()
{
	this->paused = true;
	this->window.setMouseCursorVisible(true);
}

void State::unpauseState()
{
	this->paused = false;
	if(this->cursorVisibility == false)
		this->window.setMouseCursorVisible(false);
}

void State::refreshGui()
{
	
}

void State::updateMousePosition(sf::View* view) //default: = nullptr
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(this->window);

	if (view)
		this->window.setView(*view);

	this->mousePosView = this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window));

	this->mousePosGrid =
		sf::Vector2i(
			static_cast<int>(this->mousePosView.x) / (this->gridSize),
			static_cast<int>(this->mousePosView.y) / (this->gridSize)
			);

	this->window.setView(this->window.getDefaultView());
}

void State::updateScrollWheel(int ticks)
{
	this->scroll = ticks;
	this->scrollActive = true;
}


void State::resetScrollWheel()
{
	this->scroll = 0;
}

void State::typeText(sf::Uint32 unicode_text)
{
	this->unicodeText = unicode_text;
	this->textReceived = true;
}

void State::updateKeytime(const float& dt)
{
	if (this->keytime < this->keytimeMax) {
		this->keytime += dt;
	}
	if (this->clickTime < this->clickTimeMax) {
		this->clickTime += dt;
	}
}


