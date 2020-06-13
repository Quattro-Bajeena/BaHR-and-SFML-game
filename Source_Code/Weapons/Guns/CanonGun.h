#pragma once
#include "Gun.h"
class CanonGun :
	public Gun
{
private:

public:

	CanonGun(AudioManager& audio);
	~CanonGun() = default;

	void reloadSound();
	std::vector<Bullet> shootBullet();
};

