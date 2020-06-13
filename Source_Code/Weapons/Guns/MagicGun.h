#pragma once
#include "Gun.h"
class MagicGun :
	public Gun
{
private:

public:
	MagicGun(AudioManager& audio);
	~MagicGun() = default;

	void reloadSound();
	std::vector<Bullet> shootBullet();
};

