#include "stdafx.h"
#include "NormalGun.h"



NormalGun::NormalGun(AudioManager& audio)
    :Gun(audio)
{
    this->name = "NORMAL_GUN";
    this->model = gunModels::NORMAL;
    this->sprite.setTexture(Assets::Get().textures.at("NORMAL_GUN"));
    this->defaultBounds = this->sprite.getGlobalBounds();
    this->magazine = std::make_unique< Magazine>(16, 1000, 0, 0, false);
    this->damage = 6;
    this->shootTimerMax = 0.2f;
    this->reloadTimeMax = 0.55f;
    this->bulletRadius = 15.f;
    this->maxSpeed = 1000.f;
    this->maxDistance = 2000.f;
    this->bulletColor = sf::Color::White;
    this->gunDistance = 30.f;

    
}

void NormalGun::reloadSound()
{
    this->audio.play("normal_reload",10,1.1);
}

std::vector<Bullet> NormalGun::shootBullet()
{
    if (this->magazine->isEmpty() == true) {
        this->emptyMagazineSound();
    }
    if (this->shootTimer >= this->shootTimerMax  && this->reloading == false && this->magazine->isEmpty() == false) {
        this->audio.playRandom("normal_shoot");
        this->magazine->shoot(1);
        this->shootTimer = 0.f;
        return std::vector<Bullet>{
            Bullet(
                this->gunTip, this->direction,
                this->bulletColor, this->damage* this->damageModifier,
                this->bulletRadius, this->maxSpeed, this->maxDistance
                )};
        
    }
    else return std::vector<Bullet>();
}
