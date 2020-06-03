#include "stdafx.h"
#include "EditorState.h"

//Initializer function

void EditorState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width),
			static_cast<float>(this->stateData->gfxSettings->resolution.height)
			));

	this->view.setCenter(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width / 2.f),
			static_cast<float>(this->stateData->gfxSettings->resolution.height / 2.f)
			));


}


void EditorState::initVariables()
{
	this->textureRect = sf::IntRect(0, 0, static_cast<int>(this->stateData->gridSize), static_cast<int>(this->stateData->gridSize));
	this->collision = false;
	this->type = TileTypes::DEFAULT;
	this->cameraSpeed = 500.f;
	this->layer = 0;
}



void EditorState::initFonts()
{
	if (!this->font.loadFromFile("Assets/Fonts/Dosis-Light.otf")) {
		throw("ERROR::EDITORSTATE::COULD NOT LOAD FONT");
	}
}

void EditorState::initText()
{
	this->cursorText.setPosition(this->mousePosView.x + 50.f, this->mousePosView.y - 50.f);
	this->cursorText.setFont(this->font);
	this->cursorText.setCharacterSize(36);
	this->cursorText.setOutlineColor(sf::Color::Black);
	this->cursorText.setOutlineThickness(4.f);


}

void EditorState::initKeybinds()
{

	std::ifstream ifs("Config/editorstate_keybinds.ini");
	if (ifs.is_open()) {
std::string key = "";
std::string key2 = "";

while (ifs >> key >> key2) {
	this->keybinds[key] = this->supportedKeys->at(key2);
}
	}

	ifs.close();

}

void EditorState::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->font);
	this->pmenu->addButton("RESUME", 150.f, "Resume");
	this->pmenu->addButton("SAVE", 300.f, "Save");
	this->pmenu->addButton("LOAD", 500.f, "Load");
	this->pmenu->addButton("QUIT", 800.f, "Quit");


}

void EditorState::initButtons()
{

}

void EditorState::initTileMap()
{
	this->tileMap = new TileMap(static_cast<float>(this->stateData->gridSize), 10, 10, "Assets/Images/Tiles/Tilemap.png");
}

void EditorState::initGui()
{
	this->sidebar.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData->gfxSettings->resolution.width) / 12.f,
			static_cast<float>(this->stateData->gfxSettings->resolution.height))
		);
	this->sidebar.setFillColor(sf::Color(50, 50, 50, 200));
	this->sidebar.setOutlineColor(sf::Color(230, 230, 230, 200));
	this->sidebar.setOutlineThickness(2.f);


	this->selectorRect.setSize(sf::Vector2f(
		static_cast<float>(this->stateData->gridSize),
		static_cast<float>(this->stateData->gridSize)));
	this->selectorRect.setFillColor(sf::Color(230, 230, 230, 150));
	this->selectorRect.setOutlineThickness(2.f);
	this->selectorRect.setOutlineColor(sf::Color::Black);

	this->selectorRect.setTexture(this->tileMap->getTileSheet());
	this->selectorRect.setTextureRect(this->textureRect);

	this->textureSelector = new gui::TextureSelector(this->stateData->gfxSettings->resolution.width / 12.f + 10.f, 50.f,
		this->stateData->gridSize*4.f - 1.f, this->stateData->gridSize*4.f - 1.f,
		this->stateData->gridSize, this->tileMap->getTileSheet(),
		this->font, "TS");


}

//Constuctor
EditorState::EditorState(StateData* state_data)
	:State(state_data)
{
	this->initVariables();
	this->initView();
	this->initFonts();
	this->initText();
	this->initKeybinds();
	this->initPauseMenu();
	this->initButtons();
	this->initTileMap();
	this->initGui();


}



EditorState::~EditorState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
	delete this->pmenu;
	delete this->tileMap;
	delete this->textureSelector;
}



void EditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime()) {
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
}

void EditorState::updateEditorInput(const float& dt)
{
	//Move View
	//std::floor for rounding down float movemnt to nearest intiger to avoid scvreen tearing
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_UP")))) {
		this->view.move(0, -std::floor(cameraSpeed * dt));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_DOWN")))) {
		this->view.move(0, std::floor(cameraSpeed * dt));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_LEFT")))) {
		this->view.move(-std::floor(cameraSpeed * dt), 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_RIGHT")))) {
		this->view.move(std::floor(cameraSpeed * dt), 0);
	}


	//Add a tile to a tilemap
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime()) {
		if (this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow))==false){

			if (this->textureSelector->getActive() == false) {
				this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, this->layer, textureRect, this->collision, this->type);
			}
			else {
				this->textureRect = this->textureSelector->getTextureRect();
			}
		}		
	}

	//Remove tile from the tilemap
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeytime() ) {
		if (this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)) == false) {
			if (this->textureSelector->getActive() == false) {
				this->tileMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, this->layer);
			}
			else {
				this->textureRect = this->textureSelector->getTextureRect();
			}
		}
		
		
	}

	//Toggle collsiosn
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TOGGLE_COLLISION"))) && this->getKeytime()) {
		if (this->collision)
			this->collision = false;
		else
			this->collision = true;
	}

	//Increse and decrease type
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("INC_TYPE"))) && this->getKeytime()) {
		this->type++;//chnage limit to max type
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("DEC_TYPE"))) && this->getKeytime()) {
		if(this->type > 0)
			this->type--;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("INC_LAYER"))) && this->getKeytime()) {
		this->layer++;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("DEC_LAYER"))) && this->getKeytime()) {
		if (this->layer > 0)
			this->layer--;
	}



}

void EditorState::updateButtons()
{
	/*Updates all th buttons in the state and handles their functionality*/
	for (auto& it : this->buttons) {
		it.second->update(this->mousePosWindow);
	}


}

void EditorState::updateGui(const float& dt)
{
	
	this->textureSelector->update(this->mousePosWindow, dt);
	if (this->textureSelector->getActive()) {
		this->selectorRect.setTextureRect(this->textureRect);
	}
	
	this->selectorRect.setPosition(
		static_cast<float>(this->mousePosGrid.x * this->stateData->gridSize),
		static_cast<float>(this->mousePosGrid.y * this->stateData->gridSize));

	this->cursorText.setPosition(this->mousePosWindow.x + 100.f, this->mousePosWindow.y - 50.f);
	std::stringstream ss;
	ss << this->mousePosView.x << "\n"
		<< this->mousePosView.y << "\n"
		<< this->mousePosGrid.x << " | " << this->mousePosGrid.y << "\n"
		<< this->textureRect.left << " | " << this->textureRect.top << "\n"
		<< "Collision: " << this->collision << "\n"
		<< "Type: " << this->type << "\n"
		<< "Layer: " << this->layer << "\n"
		<< "Tiles: " << this->tileMap->getLayerSize(this->mousePosGrid.x, this->mousePosGrid.y, this->layer);
	this->cursorText.setString(ss.str());

	
}

void EditorState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonReleased("RESUME")) {
		this->unpauseState();
	}
	if (this->pmenu->isButtonReleased("QUIT")) {
		this->endState();
	}
	if (this->pmenu->isButtonReleased("SAVE")) {
		this->tileMap->saveToFile("tile_map.txt");
	}
	if (this->pmenu->isButtonReleased("LOAD")) {
		this->tileMap->loadFromFile("tile_map.txt");
	}
}

void EditorState::update(const float& dt)
{

	this->updateMousePosition(&this->view);
	this->updateKeytime(dt);
	this->updateInput(dt);
	if (this->paused==false) {//unbpased
		this->updateButtons();
		this->updateGui(dt);
		this->updateEditorInput(dt);
	}
	else {//paused
		this->pmenu->update(this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	this->updateButtons();



}

void EditorState::renderButtons(sf::RenderTarget& target)
{

	for (auto& it : this->buttons) {
		it.second->render(target);
	}
}

void EditorState::renderGui(sf::RenderTarget& target)
{
	if (this->textureSelector->getActive() == false) {
		//if mouse is over texture selector the recatnlw will not dispaly
		target.setView(this->view); //recatngle renedring accoring to view
		target.draw(this->selectorRect);
	}
		

	target.setView(this->window->getDefaultView()); //rest rendering with default view
	target.draw(this->sidebar);
	this->textureSelector->render(target);
	target.draw(this->cursorText);


}

void EditorState::render(sf::RenderTarget* target)
{
	if (!target) {
		target = this->window;
	}
	//Setting new view for tiles
	target->setView(this->view);
	this->tileMap->render(*target, this->mousePosGrid);
	this->tileMap->renderDeffered(*target);
	
	//reseting the view soUi wont be affected by it
	target->setView(this->window->getDefaultView());
	this->renderButtons(*target);

	//UI handles the view inside
	this->renderGui(*target);

	if (this->paused) {
		target->setView(this->window->getDefaultView());
		this->pmenu->render(*target);
	}
	
	

}


