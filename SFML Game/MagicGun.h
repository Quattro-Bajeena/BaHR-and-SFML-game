#pragma once
#include "Gun.h"
class MagicGun :
	public Gun
{
private:

public:
	MagicGun();
	~MagicGun() = default;

	std::vector<Bullet> shootBullet();
};

