#include "stdafx.h"
#include "MultiplayerServerState.h"

void MultiplayerServerState::initConnection()
{
	//std::cout << "Listening for connections\n";
	this->adress = sf::IpAddress::getLocalAddress();
	this->portNumber = 53000;

	this->clientsNum = 0;
	this->clientsMax = 4;
	this->clients.emplace_back(std::make_unique<sf::TcpSocket>());

	this->listener.setBlocking(false);

	// bind the listener to a port
	if (this->listener.listen(this->portNumber) != sf::Socket::Done)
	{
		std::cout << "error on bining the listner\n";
	}

	this->newConnectionTimer = 0.f;
	this->newConnectionTimerMax = 1.f;
}



sf::Socket::Status MultiplayerServerState::receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout)
{
	
	sf::SocketSelector selector;
	selector.add(socket);
	if (selector.wait(timeout))
		return socket.receive(packet);
	else
		return sf::Socket::NotReady;
	
}

void MultiplayerServerState::listen()
{
	if (true) {
		if (this->listener.accept(*this->clients.back()) != sf::Socket::Done) {

			
		}
		else {
			
			this->playerJoin(*this->clients.back());
			this->clients.emplace_back(std::make_unique<sf::TcpSocket>());
			this->clientsNum++;
			std::cout << "Accepted new connection\n";
		}
		this->newConnectionTimer = 0.f;
	}
	
}

void MultiplayerServerState::playerJoin(sf::TcpSocket& client)
{
	std::string player_id = "player_id__" + std::to_string(this->clientsNum);
	this->players.emplace_back(new Player(player_id, this->world->randomFreeTile(), this->audio));

	sf::Packet start_packet;
	start_packet << "connected" << this->players.back()->getName() << this->players.back()->getPosition().x << this->players.back()->getPosition().y;
	client.send(start_packet);
}

void MultiplayerServerState::recievePlayerInput(sf::TcpSocket& client)
{
	this->input = PlayerInput();
	sf::Packet packet;
	if (client.receive(packet) == sf::Socket::Status::Done) {
		//this->receiveWithTimeout(this->client, packet, sf::milliseconds(300));
		if (packet >> this->input)
		{
			
		}
		else
		{
			
		}
	}
	
	
}

void MultiplayerServerState::updatePlayerInput(Player* const c_player, const float& dt)
{
	this->mousePosWindow = this->input.mousePosWindow;

	
	this->window.setView(this->view);
	this->mousePosView = this->window.mapPixelToCoords(this->mousePosWindow);
	this->window.setView(this->window.getDefaultView());


	c_player->move(this->input.move.x, this->input.move.y, dt);

	if (this->input.roll == true) {
		c_player->roll(dt);
	}
	else if (this->input.shoot == true) {
		c_player->shoot(this->input.mousePosView);
	}

	if (this->input.reload == true) {
		if (c_player->canReload()) {
			c_player->reload();
			this->reloadBar->reloadAnimationStart(c_player->getReloadTime());
		}
	}

	//Swapping weapons with mouse scroll
	if (this->input.scroll != 0) {
		c_player->changeCurrGunAdjacent(this->input.scroll);
		this->weaponSlot->setCurrentGun(c_player->getCurrentGunModel(), c_player->getMagazineInfo());
	}

	if (this->input.gunPos != -1) {
		c_player->changeCurrGunPos(this->input.gunPos);
		this->weaponSlot->setCurrentGun(c_player->getCurrentGunModel(), c_player->getMagazineInfo());
	}
		
		
	
}

void MultiplayerServerState::sendPlayerState()
{
	for (int i = 0; i < this->clients.size() - 1; i++) {
		PlayerState state = PlayerState(this->players[i]->getCenterPosition(), this->players[i]->getHealth());
		sf::Packet packet;
		packet << state;
		this->clients[i]->send(packet);
	}
}

void MultiplayerServerState::updateNetwork(const float& dt)
{
	this->listen();
	for (int i = 0; i < this->clients.size() - 1; i++) {
		this->recievePlayerInput(*this->clients[i]);
		this->updatePlayerInput(this->players[i], dt);
	}
	
}

void MultiplayerServerState::updateGameLogic(const float& dt)
{
	this->updateKeytime(dt);
	this->updateGui(dt);

	for (Player* player : this->players) {
		this->world->update(player, dt); //Updating Enemies and PowerUps
		this->world->updateCollision(player, dt); //Player's collision with map
		this->world->updateBulletCollisions(player->getBullets());//Player's bullets collison with map and enemies

		player->update(dt);
		player->updateWeapon(this->mousePosView, dt); //updating gun position, handling reloads
		player->updateBullets(this->world->getEnemies(), dt); //Collision of player bullets withe enemies
	}
	
	this->updateView(dt); //camera following player
	if (player->isDead() == true) { //chechks is player is dead from any causes
		this->loose();
	}

	
	this->resetScrollWheel();
}

MultiplayerServerState::MultiplayerServerState(StateData& state_data, GameStatistics& stats, AudioManager& audio)
	:GameState(state_data, stats, audio)
{
	this->initConnection();
}

MultiplayerServerState::~MultiplayerServerState()
{
	util::purge(this->players);
}

//MAIN UPDATE
void MultiplayerServerState::update(const float& dt)
{
	this->newConnectionTimer += dt;

	this->updateNetwork(dt);
	this->updateGameLogic(dt);
}

void MultiplayerServerState::render(sf::RenderTarget* target) const
{
	this->renderTexture.clear();
	this->renderTexture.setView(this->view);

	this->world->render(this->renderTexture,
		this->player->getGridPosition(this->stateData.gridSize));


	//render player and players bullets
	for (Player* player : this->players) {
		player->render(this->renderTexture);
	}
	

	//Render Tiles above the player
	this->world->renderDeffered(this->renderTexture);
	target->draw(this->texts.at("PLAYER_INFO"));
	this->renderGui(&this->renderTexture);

	//Final Render

	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}
