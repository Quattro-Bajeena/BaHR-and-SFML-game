#pragma once
#include "GameState.h"
#include "MultiplayerClientState.h"


class MultiplayerServerState :
    public GameState
{
private:
    sf::IpAddress adress;
    int portNumber;
    sf::SocketSelector selector;
    sf::TcpListener listener;

    int clientsMax;
    int clientsNum;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
    std::vector<Player*> players;
    sf::TcpSocket client;
    

    //Gameplay
    PlayerInput input;
    float newConnectionTimer;

    float newConnectionTimerMax;
    //Initliazers
    void initConnection();
    //utility
    sf::Socket::Status receiveWithTimeout(sf::TcpSocket& socket, sf::Packet& packet, sf::Time timeout);
    
    //Network functions
    void listen();
    void playerJoin(sf::TcpSocket& client);
    void recievePlayerInput(sf::TcpSocket& client);
    void updatePlayerInput(Player* const c_player, const float& dt);
    void sendPlayerState();

    void updateNetwork(const float& dt);
    void updateGameLogic(const float& dt);
 
public:
    MultiplayerServerState(StateData& state_data, GameStatistics& stats, AudioManager& audio);
    ~MultiplayerServerState();

    
    void update(const float& dt);
    void render(sf::RenderTarget* target = nullptr) const;
};

