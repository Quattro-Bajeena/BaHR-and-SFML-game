#pragma once
#include "Gun.h"
class SpreadGun :
	public Gun
{
private:

public:
	SpreadGun(AudioManager& audio);
	~SpreadGun() = default;

	void reloadSound() override;
	std::vector<Bullet> shootBullet();
};

