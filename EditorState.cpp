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
	this->textureRect = sf::IntRect(0, 0, static_cast<int>(this->stateData.gridSize), static_cast<int>(this->stateData.gridSize));
	this->collision = false;
	this->type = TileTypes::DEFAULT;
	this->cameraSpeed = 500.f;
	this->layer = 0;
	this->cursorVisibility = true;
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
	this->pmenu->addButton("RESUME", 150.f, "Resume");
	this->pmenu->addButton("SAVE", 300.f, "Save");
	this->pmenu->addButton("LOAD", 500.f, "Load");
	this->pmenu->addButton("QUIT", 800.f, "Quit");
	this->pmenu->addTextBox("FILE_PATH", 650, "default_map");


}

void EditorState::initButtons()
{

}

void EditorState::initTileMap()
{
	this->tileMap = std::make_unique< TileMap>(static_cast<float>(this->stateData.gridSize), 15, 15, 0, 0, 1, Assets::Get().textures.at("TILE_MAP"));
}

void EditorState::initGui()
{
	this->sidebar.setSize(
		sf::Vector2f(
			static_cast<float>(this->stateData.gfxSettings->resolution.width) / 12.f,
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

	this->textureSelector = std::make_unique< gui::TextureSelector>(this->stateData.gfxSettings->resolution.width / 12.f + 10.f, 50.f,
		this->stateData.gridSize * 4.f - 1.f, this->stateData.gridSize * 4.f - 1.f, this->stateData.scale,
		this->stateData.gridSize, this->tileMap->getTileSheet(),
		Assets::Get().font, "TS");


}

//Constuctor
EditorState::EditorState(StateData& state_data)
	:State(state_data)
{
	this->initVariables();
	this->initView();
	this->initText();
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime()) {
		if (this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)) == false) {

			if (this->textureSelector->getActive() == false) {
				this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, this->layer, textureRect, this->collision, this->type);
			}
			else {
				this->textureRect = this->textureSelector->getTextureRect();
			}
		}
	}

	//Remove tile from the tilemap
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeytime()) {
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && this->getKeytime()) {
		if (this->collision)
			this->collision = false;
		else
			this->collision = true;
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
		static_cast<float>(this->mousePosGrid.x * this->stateData.gridSize),
		static_cast<float>(this->mousePosGrid.y * this->stateData.gridSize));

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
		this->tileMap->saveToFile("maps/" + this->pmenu->getTextBoxString("FILE_PATH")+".txt");
	}
	if (this->pmenu->isButtonReleased("LOAD")) {
		this->tileMap->loadFromFile("maps/" + this->pmenu->getTextBoxString("FILE_PATH") + ".txt", Assets::Get().textures.at("TILE_MAP"));
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
	this->updateButtons();
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
	if (this->textureSelector->getActive() == false) {
		//if mouse is over texture selector the recatnlw will not dispaly
		target.setView(this->view); //recatngle renedring accoring to view
		target.draw(this->selectorRect);
	}


	target.setView(this->window.getDefaultView()); //rest rendering with default view
	target.draw(this->sidebar);
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
	this->tileMap->render(*target, this->mousePosGrid);
	this->tileMap->renderDeffered(*target);

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


