#include "stdafx.h"
#include "CanonGun.h"

CanonGun::CanonGun(AudioManager& audio)
    :Gun(audio)
{
    this->name = "CANON";
    this->model = gunModels::CANNON;
    this->sprite.setTexture(Assets::Get().textures.at("CANON_GUN"));
    this->defaultBounds = this->sprite.getGlobalBounds();
    this->magazine = std::make_unique< Magazine>(8, 50, 0, 0, false);
    this->damage = 60;
    this->shootTimerMax = 0.6f;
    this->reloadTimeMax = 1.5f;
    this->bulletRadius = 30.f;
    this->maxSpeed = 800.f;
    this->maxDistance = 1500.f;
    this->bulletColor = sf::Color::Black;
    this->gunDistance = 30.f;
}

std::vector<Bullet> CanonGun::shootBullet()
{
    this->requestingShoot = true;
    if (this->shootTimer >= this->shootTimerMax && this->reloading == false && this->magazine->isEmpty() == false) {
        this->audio.playRandom("canon_shoot", 30, 0.7);
        this->magazine->shoot(1);
        this->shootTimer = 0.f;
        this->readyToShoot = false;
        return std::vector<Bullet>{
            Bullet(
                this->gunTip, this->direction,
                this->bulletColor, this->damage* this->damageModifier,
                this->bulletRadius, this->maxSpeed, this->maxDistance
                )};

    }
    else return std::vector<Bullet>();
}
