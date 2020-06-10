#pragma once
#include "GameState.h"
#include "World.h"

struct PlayerInput {
    PlayerInput();
    PlayerInput(int id, sf::Vector2f move, sf::Vector2f mousePosView, sf::Vector2i mousePosWindow, bool shoot, bool roll, bool reload, int scroll, int gunPos);
    int id;
    sf::Vector2f move;
    sf::Vector2f mousePosView;
    sf::Vector2i mousePosWindow;
    bool shoot;
    bool roll;
    bool reload;
    int scroll;
    int gunPos;
};

struct PlayerState {
    PlayerState()
        :position(-1,-1), health(-1)
    {};
    PlayerState(sf::Vector2f playerPos, int playerHealth)
        :position(playerPos), health(playerHealth)
    {}
    sf::Vector2f position;
    int health;
};

struct StateOfGame {
    World world;
    Player player;
};

sf::Packet& operator <<(sf::Packet& packet, const PlayerInput& input);
sf::Packet& operator >>(sf::Packet& packet, PlayerInput& input);
sf::Packet& operator <<(sf::Packet& packet, const PlayerState& state);
sf::Packet& operator >>(sf::Packet& packet, PlayerState& state);



class MultiplayerClientState :
    public State
{

private:
    //Network

    sf::TcpSocket socket;
    sf::IpAddress adress;
    int portNumber;
    PlayerInput input;
    PlayerState serverState;


    //GAMEPLAY
    std::string playerId;
    Player* player;
    World* world;

    //Initlaizer
    void initConnection();
    void initPlayer();

    void sendInput();
    void receive();
    void receivePlayerState();
public:
    MultiplayerClientState(StateData& state_data);

    void updateInput(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget* target = nullptr) const;
};

