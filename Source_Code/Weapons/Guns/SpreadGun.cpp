#include "stdafx.h"
#include "SpreadGun.h"
#include "RNG.h"

SpreadGun::SpreadGun(AudioManager& audio)
    :Gun(audio)
{
    this->name = "SPREAD GUN";
    this->model = gunModels::SPREAD;
    this->sprite.setTexture(Assets::Get().textures.at("SPREAD_GUN"));
    this->defaultBounds = this->sprite.getGlobalBounds();
    this->magazine = std::make_unique< Magazine>(60, 600, 0, 0, false);
    this->damage = 10;
    this->shootTimerMax = 0.2f;
    this->reloadTimeMax = 0.5f;
    this->bulletRadius = 14.f;
    this->maxSpeed = 900.f;
    this->maxDistance = 2500.f;
    this->bulletColor = sf::Color::Red;
    this->gunDistance = 30.f;
}

void SpreadGun::reloadSound()
{
    this->audio.play("spread_reload");
}

std::vector<Bullet> SpreadGun::shootBullet()
{
    
    if (this->magazine->isEmpty() == true) {
        this->emptyMagazineSound();
    }
    if ( this->shootTimer >= this->shootTimerMax && this->reloading == false && this->magazine->isEmpty() == false) {
        this->audio.playRandom("spread_shoot");
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
