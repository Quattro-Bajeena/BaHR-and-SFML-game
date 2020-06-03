#pragma once
#include "Gun.h"
class ShotGun :
	public Gun
{
private:

public:
	ShotGun();
	~ShotGun() = default;

	std::vector<Bullet> shootBullet();
};

