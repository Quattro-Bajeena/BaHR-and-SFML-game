#include "stdafx.h"
#include "MagicGun.h"

MagicGun::MagicGun()
{
    this->name = "MAGIC_GUN";
    this->model = gunModels::MAGIC;
    this->sprite.setTexture(Assets::Get().textures.at("MAGIC_GUN"));
    this->defaultBounds = this->sprite.getGlobalBounds();
    this->magazine = std::make_unique< Magazine>(1, 1, 1,1, true);
    this->shootTimerMax = 0.1f;
    this->reloadTimeMax = 0.0f;

}

std::vector<Bullet> MagicGun::shootBullet()
{
    if (this->shootTimer >= this->shootTimerMax && this->reloading == false) {
        this->magazine->shoot(0);
        this->shootTimer = 0;
        this->readyToShoot = false;

        std::vector<Bullet> bullets;
        sf::Vector2f central = util::rotateVector(this->direction, static_cast<float>(rand() % 20) - 10.f);

        int bullet_num = rand()%3 + 1;
        float spread_angle = rand()%45 + 45.f;
        float angle = spread_angle / (bullet_num - 1);

        for (int i = 0; i < bullet_num; i++) {

            sf::Vector2f bulletDir = util::rotateVector(central, (spread_angle / 2) - (i * angle));
            sf::Color rand_color = sf::Color(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55, 255);
            int rand_damage = 10 + rand() % 5;
            float rand_radius = 10 + rand() % 10;
            float rand_speed = 800 + rand() % 400;
            float rand_distance = 1000 + rand() % 1000;


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
