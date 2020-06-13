#pragma once
#include "Gun.h"

class FastGun :
	public Gun
{
private:
public:
	FastGun(AudioManager& audio);
	~FastGun() = default;

	std::vector<Bullet> shootBullet();
};

