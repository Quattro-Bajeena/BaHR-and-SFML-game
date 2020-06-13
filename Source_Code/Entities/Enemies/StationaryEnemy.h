#pragma once
#include "Enemy.h"
class StationaryEnemy :
    public Enemy
{
private:

public:
    StationaryEnemy(sf::Vector2f position, AudioManager& audio);

    //Functions
    void shoot(sf::Vector2f targetPos);
    void updateAnimations(const float& dt);
};

