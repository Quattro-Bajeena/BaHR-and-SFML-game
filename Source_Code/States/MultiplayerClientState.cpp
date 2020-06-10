#include "stdafx.h"
#include "MultiplayerClientState.h"

sf::Packet& operator <<(sf::Packet& packet, const PlayerInput& input)
{
    return packet <<
        input.move.x << input.move.y <<
        input.mousePosView.x << input.mousePosView.y <<
        input.mousePosWindow.x << input.mousePosWindow.y <<
        input.shoot << input.roll << input.reload << input.scroll << input.gunPos;
}

sf::Packet& operator >>(sf::Packet& packet, PlayerInput& input)
{
    return packet >> input.move.x >> input.move.y >>
        input.mousePosView.x >> input.mousePosView.y >>
        input.mousePosWindow.x >> input.mousePosWindow.y >>
        input.shoot >> input.roll >> input.reload >> input.scroll >> input.gunPos;
}

sf::Packet& operator<<(sf::Packet& packet, const PlayerState& state)
{
	return packet << state.position.x << state.position.y << state.health;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerState& state)
{
	return packet >> state.position.x >> state.position.y >> state.health;
}



PlayerInput::PlayerInput()
    :move(0, 0), mousePosView(0, 0), mousePosWindow(0, 0), shoot(false), roll(false), reload(false), scroll(0), gunPos(-1)
{
}

PlayerInput::PlayerInput(int id, sf::Vector2f move, sf::Vector2f mousePosView, sf::Vector2i mousePosWindow, bool shoot, bool roll, bool reload, int scroll, int gunPos)
    :id(id), move(move), mousePosView(mousePosView), mousePosWindow(mousePosWindow), shoot(shoot), roll(roll), reload(reload), scroll(scroll), gunPos(gunPos)
{
}



void MultiplayerClientState::initConnection()
{
    std::cout << "Starting Initlaizng connection\n";
    this->adress = sf::IpAddress::getLocalAddress();
    this->portNumber = 53000;
    std::cout << "Connecting to adress: " << this->adress << ". On port: " << this->portNumber << ".\n";
	this->socket.setBlocking(false);
    sf::Socket::Status status = this->socket.connect(this->adress, this->portNumber);

    if (status != sf::Socket::Done)
    {
        std::cout << "ERROR: Coulnd't connect\n";
    }
    else std::cout << "Connecting succsful\n";
}

void MultiplayerClientState::initPlayer()
{


}

void MultiplayerClientState::updateInput(const float& dt)
{
	this->input = PlayerInput();
	//std::cout << "starting to update input\n";
	

	this->input.mousePosWindow = this->mousePosWindow;
	this->input.mousePosView = this->mousePosView;

	//std::cout << "mouse pos Window: " << this->input.mousePosWindow.x << "|" << this->input.mousePosWindow.y << "\n";
	//std::cout << "mouse pos View: " << this->input.mousePosView.x << "|" << this->input.mousePosView.y << "\n";
		
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->input.move.x += (-1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->input.move.x += (1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->input.move.y += (-1.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->input.move.y += (1.f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		this->input.roll = true;
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		this->input.shoot = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		this->input.reload = true;
	}
	//Swapping weapons with mouse scroll
	if (this->scroll != 0) {
		this->input.scroll = this->scroll;
	}


	//One option for swapping weapons by 1-9 numbers
	if (this->textReceived == true && this->unicodeText >= 48 && this->unicodeText <= 57) {
		this->input.gunPos = (this->unicodeText - 49);
		this->textReceived = false;

	}
}


void MultiplayerClientState::sendInput()
{
	sf::Packet packet;
	packet << this->input;
	if (this->socket.send(packet) == sf::Socket::Done) {
		//std::cout << "packet sent\n";
	}
	else std::cout << "sending error\n";
	this->input = PlayerInput();
}

void MultiplayerClientState::receive()
{
	sf::Packet packet;
	if (this->socket.receive(packet) == sf::Socket::Status::Done) {
		std::string command;
		
		if (packet >> command )
		{
			if (command == "connected") {
				std::string id;
				sf::Vector2f pos;
				packet >> id >> pos.x >> pos.y;
				this->player = new Player(id, pos);
				std::cout << "succesfully connected\n";
			}
		}
		else
		{
			std::cout << "failed to recieve\n";
		}
	}
	else std::cout << "Didnt recieve a packet\n";
}

void MultiplayerClientState::receivePlayerState()
{
	
	sf::Packet packet;
	this->socket.receive(packet);
	if ( packet >> this->serverState)
	{
		//std::cout << "succesfully recieved player input\n";

	}
	else
	{
		std::cout << "failed to receive player state\n";
	}
}

//CONSTRUCTOR PUBLIC
MultiplayerClientState::MultiplayerClientState(StateData& state_data)
    :State(state_data)
{
    this->initConnection();
	this->initPlayer();

}

void MultiplayerClientState::update(const float& dt)
{
	
	//std::cout << this->mousePosView.x<<" | " << this->mousePosView.y << "\n";
	if (this->window.hasFocus()) {
		this->updateMousePosition();
		this->updateInput(dt);
	}
	this->sendInput();

	this->receive();
	this->resetScrollWheel();

}



void MultiplayerClientState::render(sf::RenderTarget* target) const
{

	
}
