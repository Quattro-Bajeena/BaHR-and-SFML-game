#pragma once
#include "Gun.h"

class NormalGun :
	public Gun
{
private:

public:

	NormalGun(AudioManager& audio);
	~NormalGun() = default;

	std::vector<Bullet> shootBullet();
};

