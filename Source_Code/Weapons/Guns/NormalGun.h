#pragma once
#include "Gun.h"

class NormalGun :
	public Gun
{
private:

public:

	NormalGun(AudioManager& audio);
	~NormalGun() = default;

	void reloadSound();
	std::vector<Bullet> shootBullet();
};

