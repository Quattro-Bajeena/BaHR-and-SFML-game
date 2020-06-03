#include "stdafx.h"
#include "FastGun.h"

FastGun::FastGun(std::string name, sf::Texture& texture,
    int clip_size, int capacity, int damage,
    float shoot_time_max, float reload_time_max, float bullet_radius,
    float max_speed, float max_distance,
    sf::Color bullet_color)
    :Gun(name, texture,
        clip_size, capacity, damage,
        shoot_time_max, reload_time_max, bullet_radius, max_speed, max_distance,
        bullet_color)
{
}

std::vector<Bullet> FastGun::shootBullet()
{
    if (this->shootTimer >= this->shootTimerMax && this->magazine->shoot(1) == true && this->reloading == false) {
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

