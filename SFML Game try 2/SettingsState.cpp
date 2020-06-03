#include "stdafx.h"
#include "SettingsState.h"


//Initializer function
void SettingsState::initVariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initBackground()
{
	this->background.setSize(static_cast<sf::Vector2f>(this->window->getSize()));

	if (!this->backgroundTexture.loadFromFile("Assets/Images/Backgrounds/bg1.png")) {
		throw"ERROR::MAIN_MENU_STATE::FAILED_tolOAD_BACGORUND_TEXTURE";
	}
	this->background.setTexture(&this->backgroundTexture);
}

void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("Assets/Fonts/Dosis-Light.otf")) {
		throw("ERROR::SettingsState::COULD NOT LOAD FONT");
	}
}

void SettingsState::initKeybinds()
{

	std::ifstream ifs("Config/settingsstate_keybinds.ini");
	if (ifs.is_open()) {
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2) {
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();

}

void SettingsState::initGui()
{
	this->buttons["BACK"] = new gui::Button(
		1610.f, 300.f, 200.f, 150.f,
		&this->font, "Back", 50,
		sf::Color(250, 70, 70, 200), sf::Color(255, 220, 220, 255), sf::Color(10, 10, 10, 200),
		sf::Color(90, 90, 90, 150), sf::Color(200, 150, 150, 200), sf::Color(20, 20, 20, 50));

	this->buttons["APPLY"] = new gui::Button(
		1400.f, 300.f, 200.f, 150.f,
		&this->font, "Apply", 50,
		sf::Color(15, 15, 15, 200), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 50));

	std::vector<std::string> modes_str;
	for (auto& i : this->modes) {
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}
	this->dropDownLists["RESOLUTION"] = new gui::DropDownList(1100.f, 300.f, 150.f, 80.f, font, modes_str.data(), modes_str.size());
}

void SettingsState::initText()
{
	this->optionsText.setFont(font);
	this->optionsText.setPosition(sf::Vector2f(100.f, 100.f));
	this->optionsText.setCharacterSize(50);
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 200));


	this->optionsText.setString(
		"Resolution \n\nFullscreen \n\nVsync \n\nAntialiasing \n"
		);

}

//Constructors/destructors
SettingsState::SettingsState(StateData* state_data)
	:State(state_data)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initGui();
	this->initText();

}

SettingsState::~SettingsState()
{
	for (auto& i : buttons) {
		delete i.second;
	}

	for (auto& i : dropDownLists) {
		delete i.second;
	}
}


//Functions

void SettingsState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE")))) {
		this->endState();
	}

}

void SettingsState::updateGui()
{
	/*Updates all th GUI in the state and handles their functionality*/

	//Buttons 
	for (auto& it : this->buttons) {
		it.second->update(this->mousePosWindow);
	}

	//Buttons functionality 
	//Quit the state
	if (this->buttons["BACK"]->isReleased()) {
		this->endState();
	}
	//Apply selected settinghs
	if (this->buttons["APPLY"]->isReleased()) {
		this->stateData->gfxSettings->resolution = this->modes[this->dropDownLists["RESOLUTION"]->getActiveElementId()];
		this->window->create(this->stateData->gfxSettings->resolution,
			this->stateData->gfxSettings->title,
			sf::Style::Titlebar | sf::Style::Close,
			this->stateData->gfxSettings->contexSettings);
		this->window->setFramerateLimit(this->stateData->gfxSettings->frameRateLimit);
		this->window->setVerticalSyncEnabled(this->stateData->gfxSettings->vsync);
	}

	//Dropdown lists
	for (auto& it : this->dropDownLists) {
		it.second->update(this->mousePosWindow);
	}

	//Dorpdown lists functioanlity

	
}

void SettingsState::update(const float& dt)
{

	this->updateMousePosition();
	this->updateInput(dt);
	this->updateGui();
;

}

void SettingsState::renderGui(sf::RenderTarget& target)
{

	for (auto& it : this->buttons) {
		it.second->render(target);
	}

	for (auto& it : this->dropDownLists) {
		it.second->render(target);
	}
}

void SettingsState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}

	target->draw(this->background);

	this->renderGui(*target);
	target->draw(this->optionsText);

	//remove later
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(24);
	std::stringstream ss;
	ss << this->mousePosView.x << "\n" << this->mousePosView.y;
	mouseText.setString(ss.str());


	target->draw(mouseText);
}


