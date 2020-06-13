#pragma once
#include "Gun.h"
class Revolver :
	public Gun
{
private:

public:

	Revolver(AudioManager& audio);
	~Revolver() = default;

	void reloadSound();
	std::vector<Bullet> shootBullet();
};

