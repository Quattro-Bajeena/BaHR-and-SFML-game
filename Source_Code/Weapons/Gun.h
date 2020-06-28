#pragma once
#include "Bullet.h"
#include "Assets.h"
#include "Utility.h"
#include "AudioManager.h"


class Magazine
{
private:
	
	
	bool infinite;

	const int clipSize;
	int bulletsLoaded;
	int bulletsAll;
	const int capacity;

	int bulletsPerShot;

public:
	struct MagazineInfo {
	public:
		MagazineInfo(int clip_size, int bullets_loaded, int bullets_all, int capacity, bool infinite)
			:clipSize(clip_size), bulletsLoaded(bullets_loaded), bulletsAll(bullets_all), capacity(capacity),
			clipSizeStr(std::to_string(clip_size)), bulletsLoadedStr(std::to_string(bullets_loaded)), bulletsAllStr(std::to_string(bullets_all)), capacityStr(std::to_string(capacity))
		{
			if (infinite == true) {
				this->clipSizeStr = L"\u221E";
				this->bulletsLoadedStr = L"\u221E";
				this->bulletsAllStr = L"\u221E";;
				this->capacityStr = L"\u221E";
			}
		}
		int clipSize;
		int bulletsLoaded;
		int bulletsAll;
		int capacity;

		sf::String clipSizeStr;
		sf::String bulletsLoadedStr;
		sf::String bulletsAllStr;
		sf::String capacityStr;

	};

	Magazine(int clip_size, int capacity, int bullets_loaded, int bullets_all, bool infinite);

	//Accesors
	const float loadPercent() const;
	const float fullPercent() const;
	const bool isFull() const;
	const bool isEmpty() const;
	const bool noBullets() const;
	const MagazineInfo getMagazineInfo() const;

	void refillAmmo();
	void refillAmmo(float percent);
	void loadMagazine();
	void reload();
	bool shoot(int amount);
	
	
};

class Gun
{
private:

protected:
	AudioManager& audio;

	gunModels model;
	sf::Sprite sprite;

	std::string name;
	sf::Color bulletColor;
	sf::FloatRect defaultBounds;
	std::unique_ptr<Magazine> magazine;

	sf::Vector2f direction;
	sf::Vector2f startPosition;
	sf::Vector2f gunTip;
	float gunDistance;

	//Gameplay variables
	int damage;
	float damageModifier;

	float shootTimer;
	float shootTimerMax;

	float reloadTimeMax;
	float reloadTimer;
	bool reloading;

	float bulletRadius;

	float maxSpeed;
	float maxDistance;

	bool requestingShoot;
	bool readyToShoot;

	float emptySoundTimer;
	float emptySoundTimerMax;

public:
	//Constructor destructor
	Gun(std::string name, sf::Texture& texture,
		int clip_size, int capacity, int damage,
		float shoot_time_max, float reload_time_max, float bullet_radius,
		float max_speed, float max_distance, sf::Color bullet_color, AudioManager& audio);
	Gun(AudioManager& audio);
	virtual ~Gun();

	//Accesors
	const float loadPercent() const; //not really needed, use getMagazine
	const float fullPercent() const; //not really needed, use getMagazine
	const float getReloadTime() const;
	const bool canReload() const;
	const std::unique_ptr<Magazine>& getMagazine() const;
	const gunModels getModel() const;

	//Sounds
	virtual void reloadSound();
	virtual void emptyMagazineSound();

	//Functions
	virtual std::vector<Bullet> shootBullet() = 0;
	virtual void reload();
	void loadMagazine();
	void refillAmmo();
	void refillAmmo(float percent);
	void update(sf::Vector2f start_pos, sf::Vector2f dir, const float& dt);
	virtual void updateGunPosition();
	void render(sf::RenderTarget& target) const;

};

