#include "stdafx.h"
#include "ShootingComponent.h"

std::unique_ptr<Gun> ShootingComponent::createGun(gunModels model)
{
	switch (model) {
	case gunModels::NORMAL:
		return std::move(std::make_unique<NormalGun>(this->audio));
		break;

	case gunModels::REVOLVER:
		return std::move(std::make_unique<Revolver>(this->audio));
		break;
	case gunModels::SPREAD:
		return std::move(std::make_unique<SpreadGun>(this->audio));
		break;
	case gunModels::SHOTGUN:
		return std::move(std::make_unique<ShotGun>(this->audio));
		break;
	case gunModels::CANNON:
		return std::move(std::make_unique<CanonGun>(this->audio));
		break;
	case gunModels::MAGIC:
		return std::move(std::make_unique<MagicGun>(this->audio));
		break;
	default:
		return nullptr;
	}
}

ShootingComponent::ShootingComponent(AudioManager& audio)
	:audio(audio)
{
	this->currentGunPos = 0;
	this->swapTime = 0.f;
	this->swapTimeMax = 0.1f;


	this->currentGun = nullptr;

	this->debugText.setFont(Assets::Get().font);
	this->debugText.setCharacterSize(30.f);
}

//Public
ShootingComponent::ShootingComponent(std::unique_ptr<Gun> default_gun, AudioManager& audio)
	:audio(audio)
{
	ShootingComponent::ShootingComponent(audio);
	this->guns.emplace_back(std::move(default_gun));
	this->currentGun = this->guns.back().get();
	this->currentGun->refillAmmo();


	
	
}

ShootingComponent::~ShootingComponent()
{
	this->currentGun = nullptr;
}

std::vector<Bullet>* const ShootingComponent::getBullets()
{
	return &this->bullets;
}

const Magazine::MagazineInfo ShootingComponent::getMagazineInfo() const
{
	if (this->currentGun != nullptr)
		return this->currentGun->getMagazine()->getMagazineInfo();
	else return Magazine::MagazineInfo(0, 0, 0, 0, false);
}

const gunModels ShootingComponent::getCurrentGunModel() const
{
	if (this->currentGun != nullptr)
		return this->currentGun->getModel();
	else return gunModels::null;
}

const bool ShootingComponent::canReload() const
{
	if (this->currentGun != nullptr)
		return this->currentGun->canReload();
	else return false;
}

const float ShootingComponent::getReloadTime() const
{
	if (this->currentGun != nullptr)
		return this->currentGun->getReloadTime();
	else return -1;
}

void ShootingComponent::changeGunSound()
{
	this->audio.playRandom("change_weapon");

}

void ShootingComponent::shoot()
{
	if (this->currentGun != nullptr) {
		std::vector<Bullet> bullets_shot = this->currentGun->shootBullet();
		this->bullets.insert(this->bullets.end(), bullets_shot.begin(), bullets_shot.end());
	}
	
}

void ShootingComponent::reload()
{
	if (this->currentGun != nullptr)
		this->currentGun->reload();
	else std::cerr << "you don't have gun to reload\n";
}

void ShootingComponent::changeCurrGunAdjacent(int direction)
{
	if (this->currentGun != nullptr) {
		if (this->swapTime >= this->swapTimeMax) {
			if (direction < 0) {
				if (this->currentGunPos + 1 == this->guns.size()) {
					this->currentGunPos = 0;
				}
				else {
					this->currentGunPos++;
				}

			}
			else if (direction > 0) {
				if (this->currentGunPos == 0) {
					this->currentGunPos = this->guns.size() - 1;
				}
				else {
					this->currentGunPos--;
				}
			}
			this->changeGunSound();
			this->currentGun = this->guns[this->currentGunPos].get();
			this->swapTime = 0.f;
		}
	}
	
	
}

void ShootingComponent::changeCurrGunPos(int pos)
{
	if (this->currentGun != nullptr) {
		if (pos >= 0 && pos < this->guns.size()) {
			this->changeGunSound();
			this->currentGunPos = pos;
			this->currentGun = this->guns[this->currentGunPos].get();
		}
	}
	
}


void ShootingComponent::refillAmmo(float percent)
{
	
	if (this->currentGun != nullptr)
		this->currentGun->getMagazine()->refillAmmo(percent);
}

void ShootingComponent::addGun(gunModels model)
{
	this->guns.emplace_back(this->createGun(model));
	this->currentGun = this->guns.back().get();
	this->currentGun->refillAmmo(0.2);
	this->currentGunPos = this->guns.size() - 1;
}


void ShootingComponent::updateBullets(const std::vector<Entity*>& entities, sf::Vector2f center_pos, const float& dt)
{
	//Damaging the enemies
	for (auto bullet = bullets.begin(); bullet != bullets.end(); )
	{
		bullet->move(dt);
		for (auto enemy = entities.begin(); enemy != entities.end(); ++enemy) {

			if (bullet->checkCollision((*enemy)->getGlobalBounds())) {
				(*enemy)->looseHealth(bullet->getDamage());
				bullet = bullets.erase(bullet);
				break;
			}
		}
		if (bullet != bullets.end()) {
			++bullet;
			//without that after the last bullet was destroyed the itearor was last and when incremented by it would go above bound
		}
	}

	//Erasing bullets that are far from the player
	this->bullets.erase(
		std::remove_if(
			bullets.begin(),
			bullets.end(),
			[this, center_pos](Bullet& b) {return b.DistanceLimit(center_pos); }
			),
		bullets.end()
		);
}


void ShootingComponent::updateGun(sf::Vector2f start_pos, sf::Vector2f dir, const float& dt)
{
	if (this->currentGun != nullptr)
		this->currentGun->update(start_pos, dir, dt);
}

void ShootingComponent::render(sf::RenderTarget& target)
{
	for (const Bullet& bullet : this->bullets) {
		target.draw(bullet.getShape());
	}
	if(this->currentGun != nullptr)
		this->currentGun->render(target);
	//target.draw(this->debugText);
}

void ShootingComponent::update(sf::Vector2f curr_pos, const float& dt)
{
	if (this->swapTime <= this->swapTimeMax) {
		this->swapTime += dt;
	}

	if (this->currentGun != nullptr) {
		this->debugText.setPosition(curr_pos);
		this->debugText.setString(
			std::to_string(this->currentGun->loadPercent()) + "\n" + std::to_string(this->currentGun->fullPercent()) + "\n" + std::to_string(this->bullets.size())
		);
	}
	
}
