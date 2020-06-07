#include "stdafx.h"
#include "WorldEditorState.h"

//Initializer function

void WorldEditorState::initView()
{
	this->view.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData.gfxSettings->resolution.width * 4),
			static_cast<float>(this->stateData.gfxSettings->resolution.height * 4)
		));

	this->view.setCenter(
		sf::Vector2f(
			static_cast<float>(this->stateData.gfxSettings->resolution.width *2),
			static_cast<float>(this->stateData.gfxSettings->resolution.height *2)
		));

	this->renderTexture.create(
		this->stateData.gridSize * 15,
		this->stateData.gridSize * 15
	);

}


void WorldEditorState::initVariables()
{
	this->cameraSpeed = 2000.f;
	this->cursorVisibility = true;

	this->enemySpawn = false;
	this->enemySpawnTime = 0.f;
	this->enemyLimit = 0;
	this->placingNewRoom = false;


	this->roomType = "";
	this->roomOffset = { 0,0 };
	this->offset = { 0,0 };

	this->tempRoom = nullptr;
}



void WorldEditorState::initText()
{
	this->cursorText.setPosition(this->mousePosView.x + 50.f, this->mousePosView.y - 50.f);
	this->cursorText.setFont(Assets::Get().font);
	this->cursorText.setCharacterSize(36);
	this->cursorText.setOutlineColor(sf::Color::Black);
	this->cursorText.setOutlineThickness(static_cast<int>(4.f * this->stateData.scale.x));


}


void WorldEditorState::initPauseMenu()
{
	this->pmenu = std::make_unique< PauseMenu>(this->window, Assets::Get().font, this->stateData.scale);
	this->pmenu->addButton("RESUME", 160.f, "Resume");
	this->pmenu->addButton("SAVE", 250.f, "Save");
	this->pmenu->addButton("LOAD", 350.f, "Load");
	this->pmenu->addTextBox("FILE_PATH", 450, "TEST_WORLD");
	this->pmenu->addList("ROOMS", 550, this->worldsStr);
	this->pmenu->addButton("QUIT", 900.f, "Quit");
	
	
}

void WorldEditorState::initButtons()
{

}

void WorldEditorState::loadFiles()
{
	namespace fs = std::filesystem;

	std::cout << "-------------\n";
	std::string level_path = "World";
	std::cout << "Levels: " << "\n";
	for (const auto& entry : fs::directory_iterator(level_path)) {
		if (entry.path().filename().has_extension()) {
			std::string filename = entry.path().filename().stem().string();
			std::cout<< "-" << filename << "\n";
			this->worldsStr.push_back(filename);
		}

	}

	std::cout << "-------------\n";
	std::string room_path = "World/TileMaps/";
	std::cout << "Tile Maps: " << "\n";
	for (const auto& entry : fs::directory_iterator(room_path)) {

		std::string filename = entry.path().filename().stem().string();
		std::cout << "-" << filename << "\n";
		this->roomFiles[filename] = entry.path().string();
	}
}

void WorldEditorState::initWorld()
{
	
	this->world = std::make_unique<World>(Assets::Get().textures.at("TILE_MAP"));
}

void WorldEditorState::initGui()
{
	this->sidebar.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData.gfxSettings->resolution.width) / 8.f,
			static_cast<float>(this->stateData.gfxSettings->resolution.height))
	);
	this->sidebar.setFillColor(sf::Color(50, 50, 50, 200));
	this->sidebar.setOutlineColor(sf::Color(230, 230, 230, 200));
	this->sidebar.setOutlineThickness(static_cast<int>(2.f * this->stateData.scale.x));


	this->selectorRect.setSize(sf::Vector2f(0,0));
	this->selectorRect.setFillColor(sf::Color(255, 200, 200, 40));
	this->selectorRect.setOutlineThickness(static_cast<int>(10.f * this->stateData.scale.x));
	this->selectorRect.setOutlineColor(sf::Color::Red);

	this->placeRect.setSize(sf::Vector2f(0, 0));
	this->placeRect.setFillColor(sf::Color(240, 250, 255, 240));
	this->placeRect.setOutlineThickness(static_cast<int>(20.f * this->stateData.scale.x));
	this->placeRect.setOutlineColor(sf::Color::Green);

	std::vector<std::string> room_str;
	for (const auto& [room_type, filepath] : this->roomFiles) {
		room_str.emplace_back(room_type);
		this->roomFilesVec.emplace_back(room_type, filepath);
	}

	this->lists["ROOMS"] = std::make_unique< gui::DropDownList>(
		10, 100, 200.f, 80.f,
		this->stateData.scale, Assets::Get().font,
		room_str, 0);

}



void WorldEditorState::startPlacingRoom()
{
	this->roomType = this->roomFilesVec[this->lists.at("ROOMS")->getActiveElementId()].first;
	this->roomPath = this->roomFilesVec[this->lists.at("ROOMS")->getActiveElementId()].second;
	this->placingNewRoom = true;
	this->tempRoom = new Room(this->roomType, this->roomPath, Assets::Get().textures.at("TILE_MAP"));
	this->placeRect.setSize(sf::Vector2f(this->tempRoom->getBounds().width, this->tempRoom->getBounds().height));
}



//Constuctor
WorldEditorState::WorldEditorState(StateData& state_data)
	:State(state_data)
{
	this->loadFiles();
	this->initVariables();
	this->initView();
	this->initText();
	
	this->initButtons();
	this->initWorld();
	this->initGui(); //GUI after world
	this->initPauseMenu();


}



WorldEditorState::~WorldEditorState()
{

}



void WorldEditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->getKeytime()) {
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
	
}

void WorldEditorState::updateEditorInput(const float& dt)
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && this->getKeytime()) {
		this->startPlacingRoom();
	}

	//Add a new Room
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime()) {
		if (this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)) == false) {

			if (this->placingNewRoom == true) {
				this->world->addRoom(this->roomType, this->offset.x, this->offset.y, this->enemySpawn, this->enemySpawnTime, this->enemyLimit);

				this->placingNewRoom = false;
				delete this->tempRoom;
				this->tempRoom = nullptr;
			}
			
		}
	}

	//Remove a room
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeytime()) {
		if (this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)) == false) {
			if (this->placingNewRoom == false) {
				this->world->removeRoom(this->roomOffset);
				this->selectedRoom = nullptr;
			}
			else {
				this->placingNewRoom = false;
				delete this->tempRoom;
				this->tempRoom = nullptr;
				
			}
		}

	}

	

}

void WorldEditorState::updateButtons()
{
	
	if (this->placingNewRoom == false) {

		if (this->lists.at("ROOMS")->isActive() == false) {
			for (auto& [key, button] : this->buttons) {
				button->update(this->mousePosWindow);
			}
		}

		
		this->lists.at("ROOMS")->update(this->mousePosWindow);
		if (this->lists.at("ROOMS")->isReleased() == true) {
			this->startPlacingRoom();

		}
	}
	
}

void WorldEditorState::updateGui(const float& dt)
{
	
	if (this->placingNewRoom == true) {
		

		this->offset = sf::Vector2i(
			this->mousePosGrid.x - static_cast<int>(this->tempRoom->getTileBounds().width / 2),
			this->mousePosGrid.y - static_cast<int>(this->tempRoom->getTileBounds().height / 2)
		);
		this->placeRect.setPosition(sf::Vector2f(this->offset * this->stateData.gridSize));
		
	}
	else {

		for (const auto& room : this->world->getRooms()) {
			if (room->getBounds().contains(this->mousePosView)) {
				this->selectorRect.setSize(sf::Vector2f(room->getBounds().width, room->getBounds().height));
				this->selectorRect.setPosition(room->getBounds().left, room->getBounds().top);
				this->roomOffset.x = room->getTileBounds().left;
				this->roomOffset.y = room->getTileBounds().top;
				this->selectedRoom = room;

			}
		}
		
	}

	if (this->selectedRoom) {
		this->selectedRoom->setEnemies(this->enemySpawn, this->enemySpawnTime, this->enemyLimit);
	}



	this->cursorText.setPosition(this->mousePosWindow.x + 100.f * this->stateData.scale.x, this->mousePosWindow.y - 50.f * this->stateData.scale.y);
	std::stringstream ss;
	ss << this->mousePosView.x << " | "<< this->mousePosView.y << "\n"
		<< this->mousePosGrid.x << " | " << this->mousePosGrid.y << "\n"
		<<this->roomOffset.x <<" | "<<this->roomOffset.y <<"\n"
		<< this->roomType << "\n";

	this->cursorText.setString(ss.str());

}

void WorldEditorState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonReleased("RESUME")) {
		this->unpauseState();
	}
	else if (this->pmenu->isButtonReleased("QUIT")) {
		this->endState();
	}
	else if (this->pmenu->isButtonReleased("SAVE")) {
		this->world->saveToFile("World/" + this->pmenu->getTextBoxString("FILE_PATH") + ".txt");
		this->loadFiles();
		this->pmenu->reloadList("ROOMS", this->worldsStr);
	}
	else if (this->pmenu->isButtonReleased("LOAD")) {
		this->world->loadFromFile("World/" + this->pmenu->getListString("ROOMS") + ".txt");
	}
	else if (this->pmenu->isListReleased("ROOMS")) {
		this->pmenu->setTextBoxString("FILE_PATH", this->pmenu->getListString("ROOMS"));
	}


}

void WorldEditorState::update(const float& dt)
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

void WorldEditorState::renderButtons(sf::RenderTarget& target) const
{

	for (auto& [key, button] : this->buttons) {
		button->render(target);
	}
}

void WorldEditorState::renderGui(sf::RenderTarget& target) const
{
	target.setView(this->view);
	

	if (this->placingNewRoom == false && this->selectedRoom != nullptr &&this->world->getRooms().size() != 0) {

		target.draw(this->selectorRect);
	}
	else if(this->tempRoom){
		this->renderTexture.clear();

		this->tempRoom->fullRender(this->renderTexture);

		this->renderTexture.display();
		this->placeRect.setTexture(&this->renderTexture.getTexture(), true);

		target.draw(this->placeRect);
	}

	

	target.setView(this->window.getDefaultView()); //rest rendering with default view
	target.draw(this->sidebar);
	this->lists.at("ROOMS")->render(target);

	target.draw(this->cursorText);


}

void WorldEditorState::render(sf::RenderTarget* target) const
{
	if (!target) {
		target = &this->window;
	}
	//Setting new view for tiles
	target->setView(this->view);
	this->world->fullRender(*target);

	//reseting the view soUi wont be affected by it
	target->setView(this->window.getDefaultView());
	this->renderButtons(*target);

	//UI handles the view inside
	this->renderGui(*target);

	if (this->paused) {
		target->setView(this->window.getDefaultView());
		this->pmenu->render(*target);
	}



}



