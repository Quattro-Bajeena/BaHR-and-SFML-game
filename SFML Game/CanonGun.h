#pragma once
#include "Gun.h"
class CanonGun :
	public Gun
{
private:

public:

	CanonGun();
	~CanonGun() = default;

	std::vector<Bullet> shootBullet();
};

