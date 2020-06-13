#pragma once
#include "Enemy.h"
class CommandoEnemy :
    public Enemy
{
private:

public:
    CommandoEnemy(sf::Vector2f position, AudioManager& audio);

    //Functions
    void shoot(sf::Vector2f targetPos);
    void updateAnimations(const float& dt);
};

