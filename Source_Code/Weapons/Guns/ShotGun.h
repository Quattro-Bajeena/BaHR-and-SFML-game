#pragma once
#include "Gun.h"
#include "RNG.h"
class ShotGun :
	public Gun
{
private:

public:
	ShotGun();
	~ShotGun() = default;

	std::vector<Bullet> shootBullet();
};

