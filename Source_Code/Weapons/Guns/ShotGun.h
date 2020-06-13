#pragma once
#include "Gun.h"
#include "RNG.h"
class ShotGun :
	public Gun
{
private:

public:
	ShotGun(AudioManager& audio);
	~ShotGun() = default;

	void reloadSound();
	std::vector<Bullet> shootBullet();
};

