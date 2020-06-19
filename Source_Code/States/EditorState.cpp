#include "stdafx.h"
#include "EditorState.h"
//Initializer function

void EditorState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData.gfxSettings->resolution.width),
			static_cast<float>(this->stateData.gfxSettings->resolution.height)
			));

	this->view.setCenter(
		sf::Vector2f(
			static_cast<float>(this->stateData.gfxSettings->resolution.width / 2.f),
			static_cast<float>(this->stateData.gfxSettings->resolution.height / 2.f)
			));


}


void EditorState::initVariables()
{
	this->textureRect = sf::IntRect(0, 0, static_cast<int>(this->stateData.gridSize*this->stateData.scale.x), static_cast<int>(this->stateData.gridSize * this->stateData.scale.y));
	this->collision = false;
	this->type = TileTypes::DEFAULT;
	this->cameraSpeed = 500.f;
	this->layer = 0;
	this->cursorVisibility = true;
	this->mapSizes = { {5,10,15,20,25,30}, {5,10,15,20,25,30} };
	this->mapSize = { 15,15 };
}



void EditorState::initText()
{
	this->cursorText.setPosition(this->mousePosView.x + 50.f, this->mousePosView.y - 50.f);
	this->cursorText.setFont(Assets::Get().font);
	this->cursorText.setCharacterSize(36);
	this->cursorText.setOutlineColor(sf::Color::Black);
	this->cursorText.setOutlineThickness(static_cast<int>(4.f*this->stateData.scale.x));


}


void EditorState::initPauseMenu()
{
	this->pmenu = std::make_unique< PauseMenu>(this->window, Assets::Get().font, this->stateData.scale);
	this->pmenu->addButton("RESUME", 180, "Resume");
	this->pmenu->addTextBox("FILE_NAME", 300, "DEFAULT");
	this->pmenu->addList("MAP_NAMES", 390, this->mapNames);
	this->pmenu->addButton("CLEAR", 600, "Clear");
	this->pmenu->addButton("SAVE", 700, "Save");
	this->pmenu->addButton("LOAD", 800, "Load");
	this->pmenu->addButton("QUIT", 900, "Quit");


}

void EditorState::initButtons()
{

}

void EditorState::initTileMap()
{
	this->tileMap = std::make_unique< TileMap>(static_cast<float>(this->stateData.gridSize), this->mapSize.x, this->mapSize.y, 0, 0, 1, Assets::Get().textures.at("TILE_MAP"));
	this->tileMap->showCollisionTiles();
}

void EditorState::initGui()
{
	this->sidebar.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData.gfxSettings->resolution.width) / 8.f,
			static_cast<float>(this->stateData.gfxSettings->resolution.height))
		);
	this->sidebar.setFillColor(sf::Color(50, 50, 50, 200));
	this->sidebar.setOutlineColor(sf::Color(230, 230, 230, 200));
	this->sidebar.setOutlineThickness(static_cast<int>(2.f*this->stateData.scale.x));


	this->selectorRect.setSize(sf::Vector2f(
		static_cast<float>(this->stateData.gridSize),
		static_cast<float>(this->stateData.gridSize)));
	this->selectorRect.setFillColor(sf::Color(230, 230, 230, 150));
	this->selectorRect.setOutlineThickness(static_cast<int>(2.f * this->stateData.scale.x));
	this->selectorRect.setOutlineColor(sf::Color::Black);

	this->selectorRect.setTexture(this->tileMap->getTileSheet());
	this->selectorRect.setTextureRect(this->textureRect);

	this->textureSelector = std::make_unique< gui::TextureSelector>(260, 50.f,
		this->stateData.gridSize * 4.f - 1.f, this->stateData.gridSize * 4.f - 1.f, this->stateData.scale,
		this->stateData.gridSize, this->tileMap->getTileSheet(),
		Assets::Get().font, "TS");

	std::pair<std::vector<std::string>,std::vector<std::string>> map_sizes_str;
	for (int size_x : this->mapSizes.first) {
		map_sizes_str.first.emplace_back(std::to_string(size_x));
	}
	for (int size_y : this->mapSizes.second) {
		map_sizes_str.second.emplace_back(std::to_string(size_y));
	}

	int def_index_x = std::distance(this->mapSizes.first.begin(), std::find(this->mapSizes.first.begin(), this->mapSizes.first.end(), this->tileMap->getWorldGridSize().x));
	int def_index_y = std::distance(this->mapSizes.second.begin(), std::find(this->mapSizes.second.begin(), this->mapSizes.second.end(), this->tileMap->getWorldGridSize().y));

	this->lists["SIZE_X"] = std::make_unique<gui::DropDownList>(30, 250, 200, 80, this->stateData.scale, Assets::Get().font, map_sizes_str.first, def_index_x);
	this->lists["SIZE_Y"] = std::make_unique<gui::DropDownList>(30, 350, 200, 80, this->stateData.scale, Assets::Get().font, map_sizes_str.second, def_index_y);

	this->buttons["APPLY_SIZE"] = std::make_unique< gui::Button>(
		30,150,200,80,
		this->stateData.scale, &Assets::Get().font,"Apply Size",40,
		sf::Color(15, 15, 15, 255), sf::Color(250, 250, 250, 255), sf::Color(0, 0, 0, 255),
		sf::Color(200, 200, 200, 230), sf::Color(130, 130, 130, 200), sf::Color(40, 40, 40, 30));
	
}

void EditorState::initMapFiles()
{
	namespace fs = std::filesystem;

	std::cout << "-------------\n";
	std::string room_path = this->stateData.folderPaths.at("TILEMAPS");
	std::cout << "Tile Maps: " << "\n";
	for (const auto& entry : fs::directory_iterator(room_path)) {

		std::string filename = entry.path().filename().stem().string();
		std::cout << "-" << filename << "\n";
		this->mapNames.push_back(filename);
	}
}

//Constuctor
EditorState::EditorState(StateData& state_data, AudioManager& audio)
	:State(state_data, audio)
{
	this->initVariables();
	this->initMapFiles();
	this->initView();
	this->initText();
	this->initPauseMenu();
	this->initButtons();
	this->initTileMap();
	this->initGui();
	

	this->audio.stopMusic();
}



EditorState::~EditorState()
{

}



void EditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->getKeytime()) {
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && this->getKeytime()) {
		this->textureSelector->switchHidden();
	}
}

void EditorState::updateEditorInput(const float& dt)
{
	//Move View
	//std::floor for rounding down float movemnt to nearest intiger to avoid scvreen tearing
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->view.move(0, -std::floor(cameraSpeed * dt));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->view.move(0, std::floor(cameraSpeed * dt));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->view.move(-std::floor(cameraSpeed * dt), 0);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->view.move(std::floor(cameraSpeed * dt), 0);
	}

	if (this->scroll != 0) {
		this->view.zoom(1 + (-1 * this->scroll * 0.05f));
	}


	//Add a tile to a tilemap
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getClickTime()) {
		if (this->sidebarActive == false) {

			if (this->textureSelector->getActive() == false) {
				this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, this->layer, textureRect, this->collision, this->type);
			}
			else {
				this->textureRect = this->textureSelector->getTextureRect();
				
			}
		}
	}

	//Remove tile from the tilemap
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getClickTime()) {
		if (this->sidebarActive == false) {
			if (this->textureSelector->getActive() == false) {
				this->tileMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, this->layer);
			}
			else {
				//this->textureRect = this->textureSelector->getTextureRect();
			}
		}


	}

	//Toggle collsiosn
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && this->getKeytime()) {
		if (this->collision) {
			this->collision = false;
			this->selectorRect.setOutlineColor(sf::Color::Black);
		}
			
		else {
			this->collision = true;
			this->selectorRect.setOutlineColor(sf::Color::Red);
		}
			
	}

	//Change Map Size
	if (this->buttons.at("APPLY_SIZE")->isReleased()) {
		this->mapSize = sf::Vector2i(std::stoi(this->lists.at("SIZE_X")->getActiveText()), std::stoi(this->lists.at("SIZE_Y")->getActiveText()));
		this->tileMap->changeMaxSize(this->mapSize.x, this->mapSize.y);
	}

	//Increse and decrease type
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && this->getKeytime()) {
		this->type++;//chnage limit to max type
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && this->getKeytime()) {
		if (this->type > 0)
			this->type--;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && this->getKeytime()) {
		this->layer++;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && this->getKeytime()) {
		if (this->layer > 0)
			this->layer--;
	}



}

void EditorState::updateButtons()
{
	//if no list are active updates all buttons are lists, if any list is active it doesnt do it
	if (std::any_of(this->lists.begin(), this->lists.end(),
		[](std::pair<const std::string, std::unique_ptr<gui::DropDownList>>& list) {return list.second->isActive() == true; }) == false) {
		//Buttons 
		for (auto& [key, button] : this->buttons) {
			button->update(this->mousePosWindow);
		}

		//Dropdown lists
		for (auto& [key, list] : this->lists) {
			list->update(this->mousePosWindow);
		}
	}
	//only updates the active list
	for (auto& [key, list] : this->lists) {
		if (list->isActive() == true) {
			list->update(this->mousePosWindow);
		}

	}

}

void EditorState::updateGui(const float& dt)
{
	if (this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)) == true) {
		this->sidebarActive = true;
	}
	else this->sidebarActive = false;

	this->textureSelector->update(this->mousePosWindow, dt);
	if (this->textureSelector->getActive()) {
		this->selectorRect.setTextureRect(this->textureRect);
	}

	this->selectorRect.setPosition(
		static_cast<float>(this->mousePosGrid.x * this->stateData.gridSize),
		static_cast<float>(this->mousePosGrid.y * this->stateData.gridSize));

	this->cursorText.setPosition(this->mousePosWindow.x + 100.f, this->mousePosWindow.y - 50.f);
	std::stringstream ss;
	ss << this->mousePosView.x << "\n"
		<< this->mousePosView.y << "\n"
		<< this->mousePosGrid.x << " | " << this->mousePosGrid.y << "\n"
		<< this->textureRect.left << " | " << this->textureRect.top << "\n"
		/*<< "Collision: " << this->collision << "\n"*/
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
	else if (this->pmenu->isButtonReleased("QUIT")) {
		this->endState();
	}
	else if (this->pmenu->isButtonReleased("SAVE")) {
		this->tileMap->saveToFile(this->stateData.folderPaths.at("TILEMAPS") + this->pmenu->getTextBoxString("FILE_NAME")+".txt");
	}
	else if (this->pmenu->isButtonReleased("LOAD")) {
		this->tileMap->loadFromFile(this->stateData.folderPaths.at("TILEMAPS") + this->pmenu->getTextBoxString("FILE_NAME") + ".txt", Assets::Get().textures.at("TILE_MAP"));
		this->mapSize = this->tileMap->getWorldGridSize();
	}
	else if (this->pmenu->isListReleased("MAP_NAMES")) {
		this->pmenu->setTextBoxString("FILE_NAME", this->pmenu->getListString("MAP_NAMES"));
	}
	else if (this->pmenu->isButtonReleased("CLEAR")) {
		this->tileMap->clearTiles();
	}
}

void EditorState::update(const float& dt)
{

	this->updateMousePosition(&this->view);
	this->updateKeytime(dt);
	this->updateInput(dt);
	if (this->paused == false) {//unbpased
		this->updateButtons();
		this->updateGui(dt);
		this->updateEditorInput(dt);
	}
	else {//paused
		this->pmenu->update(this->mousePosWindow);
		this->pmenu->updateTextBox(this->textReceived, this->unicodeText, this->mousePosWindow);
		this->updatePauseMenuButtons();
	}
	this->resetScrollWheel();



}

void EditorState::renderButtons(sf::RenderTarget& target) const
{

	for (auto& it : this->buttons) {
		it.second->render(target);
	}
}

void EditorState::renderGui(sf::RenderTarget& target) const
{
	if (this->textureSelector->getActive() == false && this->sidebarActive == false) {
		//if mouse is over texture selector the recatnlw will not dispaly
		target.setView(this->view); //recatngle renedring accoring to view
		target.draw(this->selectorRect);
	}

	
	target.setView(this->window.getDefaultView()); //rest rendering with default view
	target.draw(this->sidebar);
	this->lists.at("SIZE_Y")->render(target);
	this->lists.at("SIZE_X")->render(target);
	this->renderButtons(target);
	this->textureSelector->render(target);
	target.draw(this->cursorText);


}

void EditorState::render(sf::RenderTarget* target) const
{
	if (!target) {
		target = &this->window;
	}
	//Setting new view for tiles
	target->setView(this->view);
	this->tileMap->fullRender(*target);


	//reseting the view soUi wont be affected by it
	target->setView(this->window.getDefaultView());
	

	//UI handles the view inside
	this->renderGui(*target);

	if (this->paused) {
		target->setView(this->window.getDefaultView());
		this->pmenu->render(*target);
	}



}


