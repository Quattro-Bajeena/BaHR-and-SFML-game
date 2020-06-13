#include "stdafx.h"
#include "Revolver.h"



Revolver::Revolver(AudioManager& audio)
    :Gun(audio)
{
    this->name = "REVOLVER";
    this->model = gunModels::REVOLVER;
    this->sprite.setTexture(Assets::Get().textures.at("REVOLVER_GUN"));
    this->defaultBounds = this->sprite.getGlobalBounds();
    this->magazine = std::make_unique< Magazine>(12, 350, 0, 0, false);
    this->damage = 12;
    this->shootTimerMax = 0.2f;
    this->reloadTimeMax = 0.3f;
    this->bulletRadius = 12.f;
    this->maxSpeed = 1400.f;
    this->maxDistance = 2000.f;
    this->bulletColor = sf::Color(220,220,200);
    this->gunDistance = 30.f;
}

std::vector<Bullet> Revolver::shootBullet()
{
    this->requestingShoot = true;
    if (this->readyToShoot == true && this->shootTimer >= this->shootTimerMax && this->reloading == false && this->magazine->isEmpty() == false) {
        this->audio.playRandom("revolver_shoot");
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
