#pragma once
#include "Collectable.h"

class Ammo :
    public Collectable
{

private:
    sf::Vector2f position;
    float speed;
    float magnetThreshold;
    bool magnet;

    float percent;

    bool ready;

    void startingAnimtion(const float& dt);
    
public:
    //Constructors
    Ammo(enemyType type, sf::Vector2f pos);
    Ammo(float percent, sf::Vector2f pos);
    Ammo(sf::Vector2f start_pos, sf::Vector2f end_pos);

    //Accesors
    const float getRefillPercent() const;

    //Main function
    void update(sf::Vector2f target, const float dt);
    void render(sf::RenderTarget& target) const;
};

