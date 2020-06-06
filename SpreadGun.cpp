#include "stdafx.h"
#include "SpreadGun.h"

SpreadGun::SpreadGun()
{
    this->name = "SPREAD GUN";
    this->model = gunModels::SPREAD;
    this->sprite.setTexture(Assets::Get().textures.at("SPREAD_GUN"));
    this->defaultBounds = this->sprite.getGlobalBounds();
    this->magazine = std::make_unique< Magazine>(60, 600, 0, 0, false);
    this->damage = 8;
    this->shootTimerMax = 0.2f;
    this->reloadTimeMax = 0.5f;
    this->bulletRadius = 14.f;
    this->maxSpeed = 1000.f;
    this->maxDistance = 2000.f;
    this->bulletColor = sf::Color::Red;
    this->gunDistance = 30.f;
}

std::vector<Bullet> SpreadGun::shootBullet()
{
    if ( this->shootTimer >= this->shootTimerMax && this->reloading == false && this->magazine->isEmpty() == false) {
        this->magazine->shoot(1);
        
        this->shootTimer = 0;
        this->readyToShoot = false;
        sf::Vector2f central = util::rotateVector(this->direction, RNG::get().randomI(-10, 10));
        sf::Vector2f left = util::rotateVector(central, 30.f);
        sf::Vector2f right = util::rotateVector(central, -30.f);

        return std::vector<Bullet> {
            Bullet(
                this->gunTip, central,
                this->bulletColor, this->damage * this->damageModifier,
                this->bulletRadius, this->maxSpeed, this->maxDistance
                ),
            Bullet(
                this->gunTip, left,
                this->bulletColor, this->damage * this->damageModifier,
                this->bulletRadius, this->maxSpeed, this->maxDistance
                ),
            Bullet(
                this->gunTip, right,
                this->bulletColor, this->damage * this->damageModifier,
                this->bulletRadius, this->maxSpeed, this->maxDistance
                )
        };
    }
    else return std::vector<Bullet>();
}
