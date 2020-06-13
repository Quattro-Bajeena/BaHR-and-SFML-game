#include "stdafx.h"
#include "SettingsState.h"


//Initializer function
void SettingsState::initVariables()
{
	int bpp = sf::VideoMode::getDesktopMode().bitsPerPixel;

	//this->modes = sf::VideoMode::getFullscreenModes();
	this->modes.emplace_back(sf::VideoMode(3840, 2160, bpp));
	this->modes.emplace_back(sf::VideoMode(2560, 1440, bpp));
	this->modes.emplace_back(sf::VideoMode(1920, 1080, bpp));
	this->modes.emplace_back(sf::VideoMode(1600, 900, bpp));
	this->modes.emplace_back(sf::VideoMode(1366, 768, bpp));

	this->antiAliasing = { 0, 2,4,8,16 };
	this->framerates = { 120,60,30,24 };
	this->cursorVisibility = true;
	
}

void SettingsState::initBackground()
{
	
	this->background.setSize(static_cast<sf::Vector2f>(this->window.getSize()));
	this->background.setTexture(&Assets::Get().textures.at("MENU_BACKGROUND"));
	this->background.setFillColor(sf::Color(100, 100, 100, 160));
}



void SettingsState::initGui()
{
	this->buttons["BACK"] = std::make_unique< gui::Button>(
		1610.f, 300.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Back", 50,
		sf::Color(250, 70, 70, 200), sf::Color(255, 220, 220, 255), sf::Color(10, 10, 10, 200),
		sf::Color(90, 90, 90, 150), sf::Color(200, 150, 150, 200), sf::Color(20, 20, 20, 50));

	this->buttons["APPLY"] = std::make_unique< gui::Button>(
		1400.f, 300.f, 200.f, 150.f, this->stateData.scale,
		&Assets::Get().font, "Apply", 50,
		sf::Color(15, 15, 15, 200), sf::Color(250, 250, 250, 255), sf::Color(10, 10, 10, 200),
		sf::Color(100, 100, 100, 150), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 50));

	std::string vsync = this->stateData.gfxSettings->vsync ? "ON" : "OFF";
	this->buttons["V_SYNC"] = std::make_unique< gui::Button>(
		470, 600, 200.f, 80, this->stateData.scale,
		&Assets::Get().font, vsync, 36 ,
		sf::Color(230, 230, 15, 240), sf::Color(255, 255, 230, 255), sf::Color(230, 230, 15, 240),
		sf::Color(150, 150, 150, 120), sf::Color(120, 120, 90, 255), sf::Color(150, 150, 150, 120),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 50));


	std::string fullscreen = this->stateData.gfxSettings->fullscreen ? "ON" : "OFF";
	this->buttons["FULLSCREEN"] = std::make_unique< gui::Button>(
		470, 780, 200.f, 80, this->stateData.scale,
		&Assets::Get().font, fullscreen, 36 ,
		sf::Color(230, 230, 15, 240), sf::Color(255, 255, 230, 255), sf::Color(230, 230, 15, 240),
		sf::Color(150, 150, 150, 120), sf::Color(120, 120, 90, 255), sf::Color(150, 150, 150, 120),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 50));


	std::vector<std::string> fps_str;
	for (auto& fps : this->framerates) {
		fps_str.push_back(std::to_string(fps) + " fps");
	}
	int current_fps = std::distance(this->framerates.begin(), std::find(this->framerates.begin(), this->framerates.end(), this->stateData.gfxSettings->frameRateLimit));

	std::vector<std::string> modes_str;
	for (auto& i : this->modes) {
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}
	int current_mode = std::distance(this->modes.begin(), std::find(this->modes.begin(), this->modes.end(), this->stateData.gfxSettings->resolution));

	std::vector<std::string> aa_str;
	for (auto& aa : this->antiAliasing) {
		aa_str.push_back(std::to_string(aa) + 'x');
	}
	int current_aa = std::distance(this->antiAliasing.begin(), std::find(this->antiAliasing.begin(), this->antiAliasing.end(), this->stateData.gfxSettings->contexSettings.antialiasingLevel));

	this->dropDownLists["RESOLUTION"] = std::make_unique< gui::DropDownList>(
		470, 100, 200.f, 80.f,
		this->stateData.scale, Assets::Get().font,
		modes_str, current_mode);
	
	this->dropDownLists["FPS"] = std::make_unique< gui::DropDownList>(
		470, 435, 200.f, 80.f,
		this->stateData.scale, Assets::Get().font,
		fps_str, current_fps);

	this->dropDownLists["GANTI_ALIASING"] = std::make_unique< gui::DropDownList>(
		470, 280, 200.f, 80.f,
		this->stateData.scale, Assets::Get().font,
		aa_str, current_aa);
	
}

void SettingsState::initText()
{
	this->fontSize = 70;
	this->optionsText.setFont(Assets::Get().font);
	this->optionsText.setPosition(sf::Vector2f(100.f * this->stateData.scale.x, 100.f * this->stateData.scale.y));
	this->optionsText.setCharacterSize(this->fontSize * this->stateData.scale.x);
	this->optionsText.setFillColor(sf::Color::Yellow);
	this->optionsText.setOutlineThickness(7 * static_cast<int>(this->stateData.scale.x));
	this->optionsText.setOutlineColor(sf::Color::Black);


	this->optionsText.setString(
		"Resolution\n\nAntialiasing\n\nFramerate\n\nVsync\n\nFullscreen\n"
		);

}

//Constructors/destructors
SettingsState::SettingsState(StateData& state_data, AudioManager& audio)
	:State(state_data, audio)
{
	this->initVariables();
	this->initBackground();
	this->initGui();
	this->initText();

}

SettingsState::~SettingsState()
{
	
}

void SettingsState::refreshGui()
{

	for (auto const& [key,button] : this->buttons) {
		button->scale(this->stateData.scale);
	}
	for (auto const& [key,list] : this->dropDownLists) {
		list->scale(this->stateData.scale);
	}

	this->optionsText.setCharacterSize(this->fontSize * this->stateData.scale.x);
	this->optionsText.setPosition(sf::Vector2f(100.f * this->stateData.scale.x, 100.f * this->stateData.scale.y));

	this->background.setSize(static_cast<sf::Vector2f>(this->window.getSize()));
	//this->background.setTexture(&Assets::Get().textures.at("MENU_BACKGROUND"));
}


//Functions

void SettingsState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		this->endState();
	}

}

void SettingsState::updateGui()
{
	/*Updates all th GUI in the state and handles their functionality*/

	if (std::any_of(this->dropDownLists.begin(), this->dropDownLists.end(),
		[](std::pair<const std::string, std::unique_ptr<gui::DropDownList>>& list) {return list.second->isActive() == true; }) == false) {
		//Buttons 
		for (auto& [key, button] : this->buttons) {
			button->update(this->mousePosWindow);
		}

		//Dropdown lists
		for (auto& [key,list] : this->dropDownLists) {
			list->update(this->mousePosWindow);		
		}
	}
	for (auto& [key, list] : this->dropDownLists) {
		if (list->isActive() == true) {
			list->update(this->mousePosWindow);
		}

	}
	
	
	//Buttons functionality 
	//Quit the state
	if (this->buttons.at("BACK")->isReleased()) {
		this->endState();
	}
	if (this->buttons.at("FULLSCREEN")->isReleased()) {
		
		this->stateData.gfxSettings->fullscreen = this->stateData.gfxSettings->fullscreen == true ? false : true;
		std::string fullscreen_text = this->stateData.gfxSettings->fullscreen == true ? "ON" : "OFF";
		this->buttons.at("FULLSCREEN")->setText(fullscreen_text);

	}
	if (this->buttons.at("V_SYNC")->isReleased()) {

		this->stateData.gfxSettings->vsync = this->stateData.gfxSettings->vsync == true ? false : true;
		std::string vsync_text = this->stateData.gfxSettings->vsync == true ? "ON" : "OFF";
		this->buttons.at("V_SYNC")->setText(vsync_text);

	}
	//Apply selected settinghs
	if (this->buttons.at("APPLY")->isReleased()) {
		this->stateData.gfxSettings->resolution = this->modes[this->dropDownLists.at("RESOLUTION")->getActiveElementId()];
		this->stateData.gfxSettings->contexSettings.antialiasingLevel = this->antiAliasing[this->dropDownLists.at("GANTI_ALIASING")->getActiveElementId()];
		this->stateData.gfxSettings->frameRateLimit = this->framerates[this->dropDownLists.at("FPS")->getActiveElementId()];
		this->stateData.scale = sf::Vector2f(this->stateData.gfxSettings->resolution.width / 1920.f, this->stateData.gfxSettings->resolution.height / 1080.f);

		
		if (this->stateData.gfxSettings->fullscreen == false) {
			this->window.create(this->stateData.gfxSettings->resolution,
				this->stateData.gfxSettings->title,
				sf::Style::Titlebar | sf::Style::Close,
				this->stateData.gfxSettings->contexSettings);
		}
		else {
			this->window.create(this->stateData.gfxSettings->resolution,
				this->stateData.gfxSettings->title,
				sf::Style::Fullscreen,
				this->stateData.gfxSettings->contexSettings);
		}
		
		this->window.setFramerateLimit(this->stateData.gfxSettings->frameRateLimit);
		this->window.setVerticalSyncEnabled(this->stateData.gfxSettings->vsync);
		this->stateData.gfxSettings->saveToFile(this->stateData.folderPaths.at("CONFIG")+"graphics.ini");
		this->refreshGui();
	}

	

	//Dorpdown lists functioanlity


}

void SettingsState::update(const float& dt)
{

	this->updateMousePosition();
	this->updateInput(dt);
	this->updateGui();
	this->resetScrollWheel();

}

void SettingsState::renderGui(sf::RenderTarget& target) const
{

	for (auto& it : this->buttons) {
		it.second->render(target);
	}

	for (auto& it : this->dropDownLists) {
		it.second->render(target);
	}
	
}

void SettingsState::render(sf::RenderTarget* target) const
{
	if (!target) {
		target = &this->window;
	}

	target->draw(this->background);

	this->renderGui(*target);
	target->draw(this->optionsText);

	//remove later
	sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(Assets::Get().font);
	mouseText.setCharacterSize(24);
	std::stringstream ss;
	ss << this->mousePosView.x << "\n" << this->mousePosView.y;
	mouseText.setString(ss.str());


	target->draw(mouseText);
}



