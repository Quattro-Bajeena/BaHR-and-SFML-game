#include "stdafx.h"
#include "InfoState.h"

void InfoState::initBackground()
{
	sf::Vector2f screenSize = static_cast<sf::Vector2f>(this->stateData.window->getSize());
	this->background.setSize(screenSize);
	this->background.setTexture(&Assets::Get().textures.at("MENU_BACKGROUND"));
	this->background.setFillColor(sf::Color(100, 100, 100, 160));

	this->controlsImg.setSize(screenSize * 0.5f);
	this->controlsImg.setTexture(&Assets::Get().textures.at("CONTROLS"));
	this->controlsImg.setPosition(
		100,
		screenSize.y * 0.45f);

}

void InfoState::initGui()
{
	this->buttons["BACK"] = std::make_unique< gui::Button>(
		1610.f, 460.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Back", 50,
		sf::Color(250, 70, 70, 200), sf::Color(255, 220, 220, 255), sf::Color(10, 10, 10, 200),
		sf::Color(90, 90, 90, 150), sf::Color(200, 150, 150, 200), sf::Color(20, 20, 20, 50));
}

void InfoState::initText()
{
	this->fontSize = 50 * this->stateData.scale.x;
	this->controlsText.setFont(Assets::Get().font);
	this->controlsText.setPosition(sf::Vector2f(50.f * this->stateData.scale.x, 10.f * this->stateData.scale.y));
	this->controlsText.setCharacterSize(this->fontSize * this->stateData.scale.x);
	this->controlsText.setFillColor(sf::Color(230, 200, 50));
	this->controlsText.setOutlineThickness(6 * static_cast<int>(this->stateData.scale.x));
	this->controlsText.setOutlineColor(sf::Color::Black);


	std::string text = R"(
	WASD    - movement
	R       - reload
	SPACE   - dogeroll
	LButton - shoot
	Scroll  - swap guns
	1-6     - change guns
	)";
	this->controlsText.setString(text);
	
	this->infoText.setFont(Assets::Get().font);
	this->infoText.setPosition(sf::Vector2f(600 * this->stateData.scale.x, 10.f * this->stateData.scale.y));
	this->infoText.setCharacterSize(45 * this->stateData.scale.x );
	this->infoText.setFillColor(sf::Color(230, 200, 50));
	this->infoText.setOutlineThickness(4 * static_cast<int>(this->stateData.scale.x));
	this->infoText.setOutlineColor(sf::Color::Black);

	text = R"(
	-Enter your name for your score to be recorded.
	-Kill enemies to obtain points, and get the best score you can.
	-Pick up dropped ammo to refill the magazine of your current weapon.
	-Dogeroll gives you temporary invincibilty.
	)";
	this->infoText.setString(text);

	
}

InfoState::InfoState(StateData& state_data, AudioManager& audio)
	:State(state_data, audio)
{
	
	this->initBackground();
	this->initGui();
	this->initText();
}

void InfoState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		this->endState();
	}
}

void InfoState::updateGui()
{
	for ( auto& [key, button] : this->buttons) {
		button->update(this->mousePosWindow);
	}

	if (this->buttons.at("BACK")->isReleased()) {
		this->endState();
	}
}

void InfoState::update(const float& dt)
{
	this->updateMousePosition();
	this->updateInput(dt);
	this->updateGui();
	this->resetScrollWheel();
}



void InfoState::render(sf::RenderTarget* target) const
{
	target->draw(this->background);
	target->draw(this->controlsText);
	target->draw(this->controlsImg);
	target->draw(this->infoText);
	
	for (const auto& [key, button] : this->buttons) {
		button->render(*target);
	}
}
