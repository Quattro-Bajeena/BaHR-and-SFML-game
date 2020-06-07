#include "stdafx.h"
#include "HitboxComponent.h"

HitboxComponent::HitboxComponent(sf::Sprite& sprite,
    const float offset_x, const float offset_y,
    const float width, const float height) :
    sprite(sprite), offsetX(offset_x), offsetY(offset_y), width(width), height(height),
    offsetXDefault(offset_x), offsetYDefault(offset_y)
{
    this->hitboxPresets["DEFAULT"] = sf::FloatRect(offset_x, offset_y, width, height);

    this->nextPosition.left = 0.f;
    this->nextPosition.top = 0.f;
    this->nextPosition.width = width;
    this->nextPosition.height = height;


    this->hitbox.setPosition(this->sprite.getPosition().x + offset_x, this->sprite.getPosition().y + offset_y);
    this->hitbox.setSize(sf::Vector2f(width, height));
    this->hitbox.setFillColor(sf::Color::Transparent);
    this->hitbox.setOutlineThickness(-1.f);
    this->hitbox.setOutlineColor(sf::Color::Red);

    this->centerPosition = sf::Vector2f(
        this->hitbox.getPosition().x + this->hitbox.getGlobalBounds().width / 2,
        this->hitbox.getPosition().y + this->hitbox.getGlobalBounds().height / 2
        );

    this->center.setPosition(this->centerPosition);
    this->center.setSize(sf::Vector2f(1, 1));
    this->center.setFillColor(sf::Color::Blue);
    this->center.setOutlineThickness(-5.f);
    this->center.setOutlineColor(sf::Color::Blue);

  
}

HitboxComponent::~HitboxComponent()
{
}
//Accesors

const sf::Vector2f& HitboxComponent::getPosition() const
{
    return this->hitbox.getPosition();
}
//Gets hitboxCenter
const sf::Vector2f& HitboxComponent::getCenterPosition() const
{
    return this->centerPosition;
}

const sf::FloatRect HitboxComponent::getGlobalBounds() const
{
    return this->hitbox.getGlobalBounds();
}

const sf::FloatRect& HitboxComponent::getNextPosition(const sf::Vector2f& velocity, const float& dt)
{
    this->nextPosition.left = this->hitbox.getPosition().x + velocity.x * dt;
    this->nextPosition.top = this->hitbox.getPosition().y + velocity.y * dt;
    return this->nextPosition;
}


const float HitboxComponent::getDistance(const sf::Vector2f& position) const
{
    sf::Vector2f vector = position - this->getCenterPosition();
    float distance = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
    return distance;
}



const bool HitboxComponent::checkCollision(sf::FloatRect object)const
{
    if (object.intersects(this->getGlobalBounds()))
        return true;
    else return false;

}

const bool HitboxComponent::checkContain(sf::FloatRect object)const
{
    if (object.contains(this->getCenterPosition()))
        return true;
    else return false;
}


//Modifiers
void HitboxComponent::setPosition(const float& x, const float& y)
{
    this->hitbox.setPosition(x, y);
    this->sprite.setPosition(
        x - this->offsetX,
        y - this->offsetY);
}



//Functions

void HitboxComponent::reset()
{
    this->setHitbox("DEFAULT");
}

void HitboxComponent::changeHitbox(const float& offset_x, const float& offset_y, const float& width, const float& height)
{
    this->offsetX = offset_x;
    this->offsetY = offset_y; //bruh tu był bład ze offsetY = offset_x
    this->hitbox.setSize(sf::Vector2f(width, height));
}

void HitboxComponent::addHitboxPreset(const std::string key, const float& offset_x, const float& offset_y, const float& width, const float& height)
{
    this->hitboxPresets[key] = sf::FloatRect(offset_x, offset_y, width, height);
}

void HitboxComponent::setHitbox(const std::string key)
{
    this->offsetX = this->hitboxPresets.at(key).left;
    this->offsetY = this->hitboxPresets.at(key).top;

    this->hitbox.setSize(sf::Vector2f(this->hitboxPresets.at(key).width, this->hitboxPresets.at(key).height));
}

bool HitboxComponent::intersects(const sf::FloatRect& frect)
{
    return this->hitbox.getGlobalBounds().intersects(frect);
}

void HitboxComponent::update()
{
    
    
    this->hitbox.setPosition(this->sprite.getPosition().x + this->offsetX,
        this->sprite.getPosition().y + this->offsetY);

    this->centerPosition = sf::Vector2f(
        this->hitbox.getPosition().x + this->hitbox.getGlobalBounds().width / 2,
        this->hitbox.getPosition().y + this->hitbox.getGlobalBounds().height / 2
        );
    this->center.setPosition(this->centerPosition);
    
}

void HitboxComponent::render(sf::RenderTarget& target) const
{
    target.draw(this->hitbox);
    target.draw(this->center);
}

