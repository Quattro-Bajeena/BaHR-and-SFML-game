#pragma once
#include "Enemy.h"
class RegularEnemy :
    public Enemy
{
private:

public:
    RegularEnemy(sf::Vector2f position, AudioManager& audio);

    //Functions
    void shoot(sf::Vector2f targetPos);
    void updateAnimations(const float& dt);
};

