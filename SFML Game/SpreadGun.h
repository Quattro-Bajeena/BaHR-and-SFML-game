#pragma once
#include "Gun.h"
class SpreadGun :
	public Gun
{
private:

public:
	SpreadGun();
	~SpreadGun() = default;

	std::vector<Bullet> shootBullet();
};

