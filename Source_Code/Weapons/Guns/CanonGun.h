#pragma once
#include "Gun.h"
class CanonGun :
	public Gun
{
private:

public:

	CanonGun(AudioManager& audio);
	~CanonGun() = default;

	std::vector<Bullet> shootBullet();
};

