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
	this->selected = false;
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
	this->pmenu->addTextBox("FILE_NAME", 450, "TEST_WORLD");
	this->pmenu->addList("ROOMS", 550, this->worldsStr);
	this->pmenu->addButton("QUIT", 900.f, "Quit");
	
	
}

void WorldEditorState::initButtons()
{

}

void WorldEditorState::loadFiles()
{
	namespace fs = std::filesystem;

	//std::cout << "-------------\n";
	std::string level_path = this->stateData.folderPaths.at("WORLD");
	//std::cout << "Levels: " << "\n";
	for (const auto& entry : fs::directory_iterator(level_path)) {
		if (entry.path().filename().has_extension()) {
			std::string filename = entry.path().filename().stem().string();
			//std::cout<< "-" << filename << "\n";
			this->worldsStr.push_back(filename);
		}

	}

	//std::cout << "-------------\n";
	std::string room_path = this->stateData.folderPaths.at("TILEMAPS");
	//std::cout << "Tile Maps: " << "\n";
	for (const auto& entry : fs::directory_iterator(room_path)) {

		std::string filename = entry.path().filename().stem().string();
		//std::cout << "-" << filename << "\n";
		this->roomFiles[filename] = entry.path().string();
	}
}

void WorldEditorState::initWorld()
{
	
	this->world = std::make_unique<World>(Assets::Get().textures.at("TILE_MAP"));
}

void WorldEditorState::initGui()
{
	//SIDABR
	this->sidebar.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData.gfxSettings->resolution.width) / 8.f,
			static_cast<float>(this->stateData.gfxSettings->resolution.height))
	);
	this->sidebar.setFillColor(sf::Color(50, 50, 50, 200));
	this->sidebar.setOutlineColor(sf::Color(230, 230, 230, 200));
	this->sidebar.setOutlineThickness(static_cast<int>(2.f * this->stateData.scale.x));

	std::vector<std::string> room_str;
	for (const auto& [room_type, filepath] : this->roomFiles) {
		room_str.emplace_back(room_type);
		this->roomFilesVec.emplace_back(room_type, filepath);
	}

	this->lists["ROOMS"] = std::make_unique< gui::DropDownList>(
		20, 100, 200.f, 80.f,
		this->stateData.scale, Assets::Get().font,
		room_str, 0);

	std::vector<std::string> enemy_limits_str;
	for (int limit : this->enemyLimits)
		enemy_limits_str.emplace_back(std::to_string(limit));


	this->lists["ENEMY_LIMIT"] = std::make_unique< gui::DropDownList>(
		20, 400, 200.f, 80.f,
		this->stateData.scale, Assets::Get().font,
		enemy_limits_str, 0);

	std::vector<std::string> enemy_spawn_times_str;
	for (float time : this->enemySpawnTimes) {
		std::string time_str = std::to_string(time);
		time_str = time_str.erase(3, time_str.length() - 3);
		time_str += " [s]";
		enemy_spawn_times_str.emplace_back(time_str);
	}
		

	this->lists["ENEMY_TIMES"] = std::make_unique< gui::DropDownList>(
		20, 500, 200.f, 80.f,
		this->stateData.scale, Assets::Get().font,
		enemy_spawn_times_str, 0);

	this->texts["SPAWN"].setString("Enemy Spawn:");
	this->texts["SPAWN"].setFont(Assets::Get().font);
	this->texts["SPAWN"].setPosition(10 * this->stateData.scale.x , 200 * this->stateData.scale.y);
	this->texts["SPAWN"].setFillColor(sf::Color(200, 160, 50));
	this->texts["SPAWN"].setCharacterSize(40 * this->stateData.scale.x);

	this->checkBox = std::make_unique<gui::CheckBox>(
		80, 270, 60, 60, this->stateData.scale,
		sf::Color(15, 10, 5, 255), sf::Color(40, 30, 20, 200), sf::Color(0, 0, 0, 100),
		sf::Color(5, 2, 0, 255), sf::Color(15, 15, 15, 200), sf::Color(20, 20, 20, 200));
	

	//MOUSE SELECTORS
	this->roomSelectedCol.first = sf::Color::Blue;
	this->roomNotSelectedCol.first = sf::Color::Red;

	this->roomSelectedCol.second = sf::Color(200, 200, 255, 40);
	this->roomNotSelectedCol.second = sf::Color(255, 200, 200, 40);

	this->selectorRect.setSize(sf::Vector2f(0,0));
	this->selectorRect.setFillColor(this->roomNotSelectedCol.second);
	this->selectorRect.setOutlineThickness(static_cast<int>(10.f * this->stateData.scale.x));
	this->selectorRect.setOutlineColor(sf::Color::Red);

	this->placeRect.setSize(sf::Vector2f(0, 0));
	this->placeRect.setFillColor(sf::Color(240, 250, 255, 240));
	this->placeRect.setOutlineThickness(static_cast<int>(20.f * this->stateData.scale.x));
	this->placeRect.setOutlineColor(sf::Color::Green);

	

}

void WorldEditorState::initEnemyOptions()
{
	this->enemyLimits = { 0, 1, 3, 5, 10, 15, 20, 25 };
	this->enemySpawnTimes = {0, 0.3f, 0.5f, 1, 2, 5 };
}



void WorldEditorState::startPlacingRoom()
{
	this->roomType = this->roomFilesVec[this->lists.at("ROOMS")->getActiveElementId()].first;
	this->roomPath = this->roomFilesVec[this->lists.at("ROOMS")->getActiveElementId()].second;
	this->placingNewRoom = true;
	this->tempRoom = new Room(this->roomType, this->roomPath, Assets::Get().textures.at("TILE_MAP"));
	this->placeRect.setSize(sf::Vector2f(this->tempRoom->getBounds().width, this->tempRoom->getBounds().height));
	this->renderTexture.create(
		this->stateData.gridSize * this->tempRoom->getSizeInTiles().x,
		this->stateData.gridSize * this->tempRoom->getSizeInTiles().y);
}



//Constuctor
WorldEditorState::WorldEditorState(StateData& state_data, AudioManager& audio)
	:State(state_data, audio)
{
	this->loadFiles();
	this->initVariables();
	this->initEnemyOptions();
	this->initView();
	this->initText();
	
	this->initButtons();
	this->initWorld();
	this->initGui(); //GUI after world
	this->initPauseMenu();

	this->audio.stopMusic();
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && this->getKeytime() && this->selected == true) {
		this->checkBox->check();
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
			else if (this->selected == false) {
				this->selected = true;
				this->selectorRect.setOutlineColor(this->roomSelectedCol.first);
				this->selectorRect.setFillColor(this->roomSelectedCol.second);

			}
			else if (this->selected == true) {
				this->selected = false;
				this->selectorRect.setOutlineColor(this->roomNotSelectedCol.first);
				this->selectorRect.setFillColor(this->roomNotSelectedCol.second);
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

	//UPDATE ROOM VARIABLES
	if (this->selected == false) {
		if (this->selectedRoom != nullptr) {
			this->enemySpawn = this->selectedRoom->getEnemyInfo().spawn;
			this->enemyLimit = this->selectedRoom->getEnemyInfo().limit;
			this->enemySpawnTime = this->selectedRoom->getEnemyInfo().time;
		}
	}
	else if (this->selected == true) {
		this->enemySpawn = this->checkBox->isChecked();
		this->enemyLimit = this->enemyLimits[this->lists.at("ENEMY_LIMIT")->getActiveElementId()];
		this->enemySpawnTime = this->enemySpawnTimes[this->lists.at("ENEMY_TIMES")->getActiveElementId()];
		
		this->selectedRoom->setEnemies(this->enemySpawn, this->enemySpawnTime, this->enemyLimit);
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


		if (this->selected == true) {
			if (this->lists.at("ENEMY_LIMIT")->isActive() == false && this->lists.at("ENEMY_TIMES")->isActive() == false) {
				this->lists.at("ENEMY_LIMIT")->update(this->mousePosWindow);
				this->lists.at("ENEMY_TIMES")->update(this->mousePosWindow);
				this->checkBox->update(this->mousePosWindow);
			}
			else if (this->lists.at("ENEMY_LIMIT")->isActive() == true) {
				this->lists.at("ENEMY_LIMIT")->update(this->mousePosWindow);
			}
			else if (this->lists.at("ENEMY_TIMES")->isActive() == true) {
				this->lists.at("ENEMY_TIMES")->update(this->mousePosWindow);
			}
			
		}
		else {
			this->lists.at("ROOMS")->update(this->mousePosWindow);
		}
		
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
	else if(this->selected == false){

		for (const auto& room : this->world->getRooms()) {
			if (room->getBounds().contains(this->mousePosView)) {
				this->selectorRect.setSize(sf::Vector2f(room->getBounds().width, room->getBounds().height));
				this->selectorRect.setPosition(room->getBounds().left, room->getBounds().top);
				this->roomOffset.x = room->getTileBounds().left;
				this->roomOffset.y = room->getTileBounds().top;
				this->selectedRoom = room;

			}
		}

		this->checkBox->setCheck(this->enemySpawn);

		int index = std::distance(this->enemyLimits.begin(), std::find(this->enemyLimits.begin(), this->enemyLimits.end(), this->enemyLimit));
		this->lists.at("ENEMY_LIMIT")->setActiveElement(index);
	
		int index_2 = std::distance(this->enemySpawnTimes.begin(), std::find(this->enemySpawnTimes.begin(), this->enemySpawnTimes.end(), this->enemySpawnTime));
		this->lists.at("ENEMY_TIMES")->setActiveElement(index_2);

		//std::cout << "enemy limit: " << index <<" | "<<"enemy timez: " << index_2 << "\n";
		
	}


	this->cursorText.setPosition(this->mousePosWindow.x + 100.f * this->stateData.scale.x, this->mousePosWindow.y - 50.f * this->stateData.scale.y);
	std::string type;
	if (this->selectedRoom != nullptr)
		type = this->selectedRoom->getTileMapType();
	else type = this->roomType;
	std::stringstream ss;
	ss << this->mousePosView.x << " | " << this->mousePosView.y << "\n"
		<<"mouse pos grid "<<this->mousePosGrid.x << " | " << this->mousePosGrid.y << "\n"
		<<"room offset "<<this->roomOffset.x << " | " << this->roomOffset.y << "\n"
		<<"selected: " <<this->selected<<"\n"
		<<"spawn: "<<this->enemySpawn << "\n"
		<<"enemy limit: "<<this->enemyLimit<<"\n"
		<<"enemy time: " << this->enemySpawnTime << "\n"
		<<"room type: "<< type << "\n";
	
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
		this->world->saveToFile(this->stateData.folderPaths.at("WORLD") + this->pmenu->getTextBoxString("FILE_NAME") + ".txt");
		this->loadFiles();
		this->pmenu->reloadList("ROOMS", this->worldsStr);
	}
	else if (this->pmenu->isButtonReleased("LOAD")) {
		this->world->loadFromFile(this->stateData.folderPaths.at("WORLD") + this->pmenu->getTextBoxString("FILE_NAME") + ".txt");
	}
	else if (this->pmenu->isListReleased("ROOMS")) {
		this->pmenu->setTextBoxString("FILE_NAME", this->pmenu->getListString("ROOMS"));
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
	else if(this->tempRoom != nullptr){
		
		this->renderTexture.clear();
		
		this->tempRoom->fullRender(this->renderTexture);

		this->renderTexture.display();
		this->placeRect.setTexture(&this->renderTexture.getTexture(), true);

		target.draw(this->placeRect);
	}

	

	target.setView(this->window.getDefaultView()); //rest rendering with default view
	target.draw(this->sidebar);
	
	target.draw(this->texts.at("SPAWN"));
	this->checkBox->render(target);

	this->lists.at("ENEMY_TIMES")->render(target);
	this->lists.at("ENEMY_LIMIT")->render(target);
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



