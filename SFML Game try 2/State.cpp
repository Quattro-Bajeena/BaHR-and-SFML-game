#include "stdafx.h"
#include "State.h"

State::State(StateData* state_data)
	: stateData(state_data),
	window(state_data->window), supportedKeys(state_data->supportedKeys),
	quit(false), paused(false), states(state_data->states), gridSize(state_data->gridSize),
	keytime(0.f), keytimeMax(0.2f)
{
	
}

State::~State()
{
}

const bool& State::getQuit() const 
{
	return this->quit;
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
	std::cout << "Ending this State" << "\n";
	this->quit = true;
	
}

void State::pauseState()
{
	this->paused = true;
}

void State::unpauseState()
{
	this->paused = false;
}

void State::updateMousePosition(sf::View* view) //default: = nullptr
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

	if(view)
		this->window->setView(*view);
	

	this->mousePosView = this->window->mapPixelToCoords( sf::Mouse::getPosition(*this->window));
	this->mousePosGrid =
		sf::Vector2i(
			static_cast<int>(this->mousePosView.x) / (this->gridSize),
			static_cast<int>(this->mousePosView.y) / (this->gridSize)
			);
		
	this->window->setView(this->window->getDefaultView());
}

void State::updateKeytime(const float& dt)
{
	if (this->keytime < this->keytimeMax) {
		this->keytime += dt;
	}
}


