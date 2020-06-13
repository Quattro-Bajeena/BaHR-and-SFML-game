#include "stdafx.h"
#include "MagicGun.h"
#include "RNG.h"

MagicGun::MagicGun(AudioManager& audio)
    :Gun(audio)
{
    this->name = "MAGIC_GUN";
    this->model = gunModels::MAGIC;
    this->sprite.setTexture(Assets::Get().textures.at("MAGIC_GUN"));
    this->defaultBounds = this->sprite.getGlobalBounds();
    this->magazine = std::make_unique< Magazine>(1, 1, 1,1, true);
    this->shootTimerMax = 0.1f;
    this->reloadTimeMax = 0.0f;

}

void MagicGun::reloadSound()
{
}

std::vector<Bullet> MagicGun::shootBullet()
{
    
    if (this->shootTimer >= this->shootTimerMax && this->reloading == false) {
        this->magazine->shoot(0);
       
        this->readyToShoot = false;
        this->shootTimer = 0;
        std::vector<Bullet> bullets;
        sf::Vector2f central = util::rotateVector(this->direction, RNG::get().randomI(-10, 10));

        int bullet_num = RNG::get().randomI(1,4);
        float spread_angle = RNG::get().randomI(45, 90);
        float angle = spread_angle / (bullet_num - 1);

        for (int i = 0; i < bullet_num; i++) {

            sf::Vector2f bulletDir = util::rotateVector(central, (spread_angle / 2) - (i * angle));
            sf::Color rand_color = sf::Color(RNG::get().randomI(55,255), RNG::get().randomI(55, 255), RNG::get().randomI(55, 255), 255);
            int rand_damage = RNG::get().randomI(10, 15);
            float rand_radius = RNG::get().randomI(10, 20);
            float rand_speed = RNG::get().randomI(800, 1200);
            float rand_distance = RNG::get().randomI(1000, 2000);


            bullets.emplace_back(Bullet(
                this->gunTip, bulletDir,
                rand_color, rand_damage * this->damageModifier,
                rand_radius, rand_speed, rand_distance
                ));
        }  
        return bullets;

    }
    else return std::vector<Bullet>();
}
