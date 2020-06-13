#pragma once
#include "Gun.h"
class Revolver :
	public Gun
{
private:

public:

	Revolver(AudioManager& audio);
	~Revolver() = default;

	std::vector<Bullet> shootBullet();
};

