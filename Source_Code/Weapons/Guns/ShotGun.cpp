#include "stdafx.h"
#include "ShotGun.h"

ShotGun::ShotGun(AudioManager& audio)
    :Gun(audio)

{
    this->name = "SHOT GUN";
    this->model = gunModels::SHOTGUN;
    this->sprite.setTexture(Assets::Get().textures.at("SHOTGUN_GUN"));
    this->defaultBounds = this->sprite.getGlobalBounds();
    this->magazine = std::make_unique< Magazine>(1, 32, 0, 0, false);
    this->damage = 20;
    this->shootTimerMax = 0.5f;
    this->reloadTimeMax = 1.f;
    this->bulletRadius = 12.f;
    this->maxSpeed = 1500.f;
    this->maxDistance = 800.f;
    this->bulletColor = sf::Color::Blue;
    this->gunDistance = 30.f;
}

std::vector<Bullet> ShotGun::shootBullet()
{
    if (this->readyToShoot == true && this->shootTimer >= this->shootTimerMax && this->reloading == false && this->magazine->isEmpty() == false) {
        this->audio.playRandom("shotgun_shoot");
        this->magazine->shoot(1);

        this->shootTimer = 0;
        this->readyToShoot = false;

        std::vector<Bullet> bullets;
        sf::Vector2f central = util::rotateVector(this->direction, RNG::get().randomI(-10, 10));

        int bullet_num = 11;
        float spread_angle = 90.f;
        float angle = spread_angle / (bullet_num-1);

        for (int i = 0; i < bullet_num; i++) {

            sf::Vector2f bulletDir = util::rotateVector(central, (spread_angle/2) -  (i * angle));
            bullets.emplace_back(Bullet(
                this->gunTip, bulletDir,
                this->bulletColor, this->damage * this->damageModifier,
                this->bulletRadius, this->maxSpeed, this->maxDistance
                ));
        }

        
        return bullets;
        
    }
    else return std::vector<Bullet>();
}
