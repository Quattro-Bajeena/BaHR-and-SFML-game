
#include "stdafx.h"
#include "Player.h"


//Constructor===================================================================

Player::Player(std::string name, const sf::Vector2f& pos, AudioManager& audio)
	:name(name), startingPos(pos), Entity(audio)
{
	this->initVariables();
	this->setPosition(pos);
	
	this->createMovementComponent(500.f, 1200.f, 4800.f, 1500.f);
	this->createAnimationComponent(Assets::Get().textures.at("PLAYER_SHEET"));
	this->createHitboxComponent(50.f, 18.f, 90.f, 180.f);

	this->createShootingComponent(std::move(std::make_unique<NormalGun>(this->audio)), this->audio);
	this->sprite.scale(6, 6);
	
	this->animationComponent->addAnimation("WALK_STRAIGHT", 12.f, 0, 1, 13, 32, 32);
	this->animationComponent->addAnimation("WALK_LEFT", 12.f, 0, 3, 6, 32, 32);
	this->animationComponent->addAnimation("WALK_RIGHT", 12.f, 0, 2, 6, 32, 32);
	this->animationComponent->addAnimation("ROLL", 12.f, 0, 5, 8, 32, 32);
	this->animationComponent->addAnimation("ROLL_BACK", 12.f, 0, 6, 8, 32, 32);
	this->animationComponent->addAnimation("IDLE", 3.f, 0, 4, 4, 32, 32);

	this->hitboxComponent->addHitboxPreset("ROLL",60.f, 60.f, 70.f, 110.f);

	

}

Player::~Player() {
	
}

//Initializer
void Player::initVariables() {

	this->healthMax = 16;
	this->health = 16;
	this->dead = false;

	
	this->rolling = false;
	this->rollingUp = false;
	this->rollTimer = 1.f; //how much time bewteen rolls

	this->invincibility = true;
	this->invincibilityTimer = 0.f;
	this->invincibilityTimerMax = 0.5f; //how long default invinvibility lasts
	

	this->debugText.setFont(Assets::Get().font);
	this->debugText.setCharacterSize(30.f);
}

//internal Functions


//Functions


void Player::roll(const float& dt)
{

	if (this->rolling == false && this->rollTimer >= 1 && this->movementComponent->getState(movement_states::IDLE) == false) {

		this->rolling = true;
		this->rollTimer = 0;
		this->movementComponent->startRoll();
		this->hitboxComponent->setHitbox("ROLL");
		this->invincibility = true;

		if (this->movementComponent->getState(movement_states::MOVING_UP) && rollingUp == false) {
			rollingUp = true;
			//this->animationComponent->play("ROLL_BACK", dt, 1, 1, true);
			this->animationComponent->setAnimation("ROLL_BACK", true);

		}
		else {
			rollingUp = false;
			//this->animationComponent->play("ROLL", dt, 1, 1, true);
			this->animationComponent->setAnimation("ROLL", true);
		}
	}

}

void Player::endRoll()
{
	this->rolling = false;
	this->rollingUp = false;
	this->movementComponent->endRoll();
	this->hitboxComponent->reset();
	this->invincibility = false;
	this->animationComponent->resetPriority();
}


void Player::shoot(sf::Vector2f mousePos)
{

	this->shootingComponent->shoot();
	{
	/*

	switch (this->currPowerUp) {
	case powerUpType::DEFAULT:
		if (this->shootTimer >= this->shootTimerMax) {
			bullets.emplace_back(Bullet(this->BCenterPosition(), this->calculateDir(mousePos), sf::Color::White,
				5));
			this->shootTimer = 0;
		}
		break;

	case powerUpType::FAST:
		if (this->shootTimer >= this->shootTimerMax/16 && this->bulletCount.at(powerUpType::FAST) > 0) {
			bullets.emplace_back(Bullet(this->getCenterPosition(), this->calculateDir(mousePos),
				sf::Color::Yellow, 1, 12, 1200));
			this->shootTimer = 0;
			this->bulletCount.at(powerUpType::FAST) -= 1;
		}
		break;

	case powerUpType::THREE:
		if (this->shootTimer >= this->shootTimerMax/2 && this->bulletCount.at(powerUpType::THREE) > 0) {
			sf::Vector2f central = this->calculateDir(mousePos);

			sf::Vector2f left = this->rotateVector(central, 30.f);
			sf::Vector2f right = this->rotateVector(central, -30.f);

			bullets.emplace_back(Bullet(this->getCenterPosition(), central, sf::Color::Red, 4));
			bullets.emplace_back(Bullet(this->getCenterPosition(), left, sf::Color::Red, 4));
			bullets.emplace_back(Bullet(this->getCenterPosition(), right, sf::Color::Red, 4));

			this->shootTimer = 0;
			this->bulletCount.at(powerUpType::THREE) -= 1;
		}
		break;

	case powerUpType::ROUND:
		if (this->shootTimer >= this->shootTimerMax && this->bulletCount.at(powerUpType::ROUND) > 0) {
			sf::Vector2f central = this->calculateDir(mousePos);
			int bulletNum = 18;

			for (int i = 0; i < bulletNum; i++) {
				sf::Vector2f circle = this->rotateVector(central, 360.f/bulletNum * i);
				bullets.emplace_back(Bullet(this->getCenterPosition(), circle, sf::Color::Blue, 4));
			}
			this->shootTimer = 0;
			this->bulletCount.at(powerUpType::ROUND) -= 1;
		}
		break;

	case powerUpType::HEAVY:
		if (this->shootTimer >= this->shootTimerMax * 2.f && this->bulletCount.at(powerUpType::HEAVY) > 0) {
			bullets.emplace_back(Bullet(this->getCenterPosition(), this->calculateDir(mousePos),
				sf::Color::Black,
				80, 30.f, 900.f));
			this->shootTimer = 0.f;
			this->bulletCount.at(powerUpType::HEAVY) -= 1;
		}
		break;

	case powerUpType::RANDOM:
		if (this->shootTimer >= this->shootTimerMax / 4 && this->bulletCount.at(powerUpType::RANDOM) > 0) {
			sf::Vector2f central = this->calculateDir(mousePos);
			sf::Color rand_color = sf::Color(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55, 255);

			bullets.emplace_back(Bullet(this->getCenterPosition(),this->rotateVector(central, rand()%360 ),rand_color,
				rand()%5+1.f, rand()%20 + 10.f, rand()%400 + 800.f
				));

			this->shootTimer = 0;
			this->bulletCount.at(powerUpType::RANDOM) -= 1;
		}
		break;
	}

		
	*/
	}
}

void Player::reload()
{
	this->shootingComponent->reload();
}

void Player::recievePowerUp(powerUpType type)
{
	//this->shootingComponent->refillAmmo(0.25);
	
}

void Player::recieveAmmo(float percent)
{
	this->shootingComponent->refillAmmo(percent);
}

void Player::recieveGun(gunModels model)
{
	this->shootingComponent->addGun(model);
}

void Player::changeCurrGunAdjacent(int direction)
{
	this->shootingComponent->changeCurrGunAdjacent(direction);
}

void Player::changeCurrGunPos(int pos)
{
	this->shootingComponent->changeCurrGunPos(pos);
}

void Player::looseHealth(int damage)
{
	if (this->invincibility == false) {
		this->health -= damage;
		this->audio.play("player_hit", 20);
	}
}

const std::string Player::getName() const
{
	return this->name;
}




void Player::updateAnimations(const float& dt) //and movement/hitbox for rolling
{
	{
	/*if (this->rolling) {

		if (rollingUp == true) {
			if (this->animationComponent->play("ROLL_BACK", dt, 1, 1, true)) {
				this->rolling = false;
				this->rollingUp = false;
				this->movementComponent->endRoll();
				this->hitboxComponent->reset();
				this->invincibility = false;
			}
		}
		else {
			if (this->animationComponent->play("ROLL", dt, 1, 1, true)) {
				this->rolling = false;
				this->movementComponent->endRoll();
				this->hitboxComponent->reset();
				this->invincibility = false;
			}
		}

		if (this->movementComponent->getState(movement_states::IDLE)) {
			this->movementComponent->endRoll();
			this->animationComponent->resetPriority();
			//this->animationComponent->play("IDLE", dt);
		}
	}
	*/
	}

	if (this->movementComponent->getState(movement_states::IDLE) == true) {
		this->animationComponent->setAnimation("IDLE");
	 }
		
	else if (this->movementComponent->getState(movement_states::MOVING_UP) == true) {
		this->animationComponent->setAnimation("WALK_STRAIGHT");
	}
		
	else if (this->movementComponent->getState(movement_states::MOVING_DOWN) == true) {
		this->animationComponent->setAnimation("WALK_STRAIGHT");
	}
		
	else if (this->movementComponent->getState(movement_states::MOVING_LEFT) == true) {
		this->animationComponent->setAnimation("WALK_LEFT");
	}
		
	else if (this->movementComponent->getState(movement_states::MOVING_RIGHT) == true) {
		this->animationComponent->setAnimation("WALK_RIGHT");
	}

	//For memories
	//this->animationComponent->play("WALK_RIGHT", dt, this->movementComponent->getVelocity().x, this->movementComponent->getMaxVelocity());

	this->animationComponent->playCurrentAnimation(dt);

	 if (this->rolling == true) {
		 if (rollingUp == true && this->animationComponent->isDone("ROLL_BACK")) {

			this->endRoll();	 

		 }
		 else if (rollingUp == false && this->animationComponent->isDone("ROLL")) {
			 this->endRoll();
		 }
	}
	
	//99% not needed	
	if (this->movementComponent->getState(movement_states::IDLE)) {
		if (this->rolling == true && this->animationComponent->isDone("ROLL") && this->animationComponent->isDone("ROLL_BACK")) {
			this->endRoll();
			std::cout << "this second end roll, am I needed?\n";
		}
		
	}
}

void Player::updateBullets(const std::vector<Enemy*>& enemies, const float dt)
{
	this->shootingComponent->updateBullets(std::vector<Entity*>(enemies.begin(), enemies.end()), this->getCenterPosition(), dt);
}

void Player::updateWeapon(const sf::Vector2f& mousePos, const float& dt)
{
	this->shootingComponent->updateGun(this->getCenterPosition(), this->calculateDir(mousePos), dt);
}

void Player::update(const float& dt) {

	this->rollTimer += dt;
	this->invincibilityTimer += dt;

	Entity::update(dt);

	this->movementComponent->update(dt);
	this->hitboxComponent->update();
	this->shootingComponent->update(this->getCenterPosition(), dt);
	this->updateAnimations(dt);


	if (this->health <= 0) {
		this->dead = true;
	}
	if (this->dead == true) {
		
		this->audio.play("player_hit", 30, 0.6);
	}
}



void Player::render(sf::RenderTarget& target) const
{

	target.draw(this->sprite);
	this->shootingComponent->render(target);
	this->hitboxComponent->render(target);

}
