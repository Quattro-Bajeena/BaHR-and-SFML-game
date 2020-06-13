#include "stdafx.h"
#include "FastGun.h"

FastGun::FastGun(AudioManager& audio)
    :Gun(audio)
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

