#include "stdafx.h"
#include "Gun.h"


Magazine::Magazine(int clip_size, int capacity, int bullets_loaded, int bullets_all, bool infinite)
	:clipSize(clip_size), capacity(capacity),
	bulletsLoaded(bullets_loaded), bulletsAll(bullets_all), infinite(infinite)
{
	if (bullets_loaded > clip_size) {
		std::cout << "ERROR there cant be more bullets in a magainze than magainze capcity\n";
	}
	this->bulletsPerShot = 1;
}

void Magazine::refillAmmo()
{
	this->bulletsAll = this->capacity;
}

void Magazine::refillAmmo(float percent)
{
	int received_bullets = static_cast<int>(this->capacity * percent);
	int diff = this->capacity - this->bulletsAll;

	if (received_bullets < diff) {
		this->bulletsAll += received_bullets;

	}
	else this->bulletsAll = this->capacity;
}


void Magazine::reload(){

	if (this->bulletsAll > 0) {

		int diff = this->clipSize - this->bulletsLoaded;

		if (this->bulletsAll >= diff) {
			this->bulletsAll -= diff;
			this->bulletsLoaded = this->clipSize;
		}
		else {
			this->bulletsLoaded = this->bulletsAll;
			this->bulletsAll = 0;
		}
	}
	
	
	
}

bool Magazine::shoot(int amount)
{
	if (this->bulletsLoaded >= amount) {
		this->bulletsLoaded -= amount;
		return true;
	}
	else return false;
}

const float Magazine::loadPercent() const
{
	return static_cast<float>(this->bulletsLoaded) / this->clipSize;
}

const float Magazine::fullPercent() const
{
	return static_cast<float>(this->bulletsAll) / this->capacity;
}

const bool Magazine::isFull() const
{
	return this->bulletsLoaded == this->clipSize;
}

const bool Magazine::isEmpty() const
{
	return this->bulletsLoaded == 0;
}

const bool Magazine::noBullets() const
{
	return this->bulletsAll == 0;
}

const Magazine::MagazineInfo Magazine::getMagazineInfo() const
{
	return MagazineInfo(this->clipSize, this->bulletsLoaded, this->bulletsAll, this->capacity, this->infinite);
}

//GUN==============================================



Gun::Gun(std::string name, sf::Texture& texture, int clip_size, int capacity,
	int damage, float shoot_time_max, float reload_time_max,
	float bullet_radius, float max_speed, float max_distance, sf::Color bullet_color, AudioManager& audio)
	:name(name),
	damage(damage),shootTimerMax(shoot_time_max), reloadTimeMax(reload_time_max), bulletRadius(bullet_radius),
	maxSpeed(max_speed), maxDistance(max_distance), bulletColor(bullet_color),
	audio(audio)
{
	this->sprite.setTexture(texture);
	this->magazine = std::make_unique< Magazine>(clip_size, capacity, 0, 0, false);
	this->damageModifier = 1.f;
	this->reloadTimer = 0.f;
	this->reloading = false;
	this->shootTimer = 0.f;
	this->defaultBounds = this->sprite.getGlobalBounds();
	this->gunDistance = 85.f;


}

Gun::Gun(AudioManager& audio)
	:audio(audio)
{
	this->damageModifier = 1.f;
	this->reloadTimer = 0.f;
	this->reloading = false;
	this->shootTimer = 0.f;
	this->requestingShoot = false;
	this->readyToShoot = true;
	this->emptySoundTimer = 0.f;
	this->emptySoundTimerMax = 1.f;
	
}

Gun::~Gun()
{

}

const float Gun::loadPercent() const
{
	return this->magazine->loadPercent();
}

const float Gun::fullPercent() const
{
	return this->magazine->fullPercent();
}

const float Gun::getReloadTime() const
{
	return this->reloadTimeMax;
}

const bool Gun::canReload() const
{
	if (this->reloading == false && this->magazine->isFull() == false
		&& this->magazine->noBullets() == false) {
		return true;
	}
	else return false;
}

const std::unique_ptr<Magazine>& Gun::getMagazine() const
{
	return this->magazine;
}

const gunModels Gun::getModel() const
{
	return this->model;
}

void Gun::reloadSound()
{
	this->audio.play("reload");
}

void Gun::emptyMagazineSound()
{
	if (this->emptySoundTimer >= this->emptySoundTimerMax) {
		this->audio.playRandom("empty_weapon",30);
		this->emptySoundTimer = 0.f;
	}
	
}



void Gun::reload()
{
	if (this->reloading == false && this->magazine->isFull() == false 
		&& this->magazine->noBullets() == false) {
		this->reloadSound();
		this->reloading = true;
	}
	
}

void Gun::refillAmmo()
{
	this->magazine->refillAmmo();
}

void Gun::refillAmmo(float percent)
{
	this->magazine->refillAmmo(percent);
}


void Gun::update(sf::Vector2f start_pos, sf::Vector2f dir, const float& dt)
{
	this->emptySoundTimer += dt;
	if (this->reloading == true) {
		//std::cout << "reloading\n";
		this->reloadTimer += dt;

		if (this->reloadTimer >= this->reloadTimeMax) {
			this->magazine->reload();
			this->reloading = false;
			this->reloadTimer = 0.f;
		}
	}

	if (this->shootTimer <= this->shootTimerMax) {
		this->shootTimer += dt;
	}

	if (this->requestingShoot == false) {
		this->readyToShoot = true;
	}
	this->requestingShoot = false;


	this->direction = dir;
	this->startPosition = start_pos;
	this->updateGunPosition();
}

void Gun::updateGunPosition()
{

	this->startPosition.x += this->direction.x * this->gunDistance;
	this->startPosition.y += this->direction.y * this->gunDistance;

	this->startPosition.y -= this->defaultBounds.height / 2;

	double angle_rad = std::atan2(this->direction.x, this->direction.y);
	double angle = angle_rad * 180. / M_PI;
	
	sf::Vector2f scale(1, 1);

	this->gunTip.x = this->startPosition.x + this->direction.x * this->defaultBounds.width * 0.9f;
	this->gunTip.y = this->startPosition.y + this->direction.y * this->defaultBounds.width * 0.9f;


	if (angle < 0) {
		scale.y = -1;
		this->gunTip = util::rotateVector(this->gunTip, this->startPosition, -10.f);
	}
	else {
		this->gunTip = util::rotateVector(this->gunTip, this->startPosition, 10.f);
	}
	angle -= 90.;
	angle *= -1;

	this->sprite.setPosition(this->startPosition);
	this->sprite.setScale(scale);
	this->sprite.setRotation(angle);
}

void Gun::render(sf::RenderTarget& target) const
{
	target.draw(this->sprite);
}


