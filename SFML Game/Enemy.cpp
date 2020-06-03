
#include "stdafx.h"
#include "Enemy.h"



Enemy::Enemy(sf::Vector2f position, float size, enemyType type, sf::Texture* texture)
	:texture(texture),size(size), type(type)
{
	this->speed = 1;
	this->shootTimerMax = 0;
	
	this->sprite.setTexture(*this->texture);
	this->setPosition(position);
	this->sprite.scale(this->size, this->size);

	switch (type) {

	default:
	case enemyType::REGULAR:
		this->createAnimationComponent(Assets::Get().textures.at("REGULAR_ENEMY"));
		this->sprite.scale(6, 6);
		this->animationComponent->addAnimation("WALK_LEFT", 1, 1, 0, 1, 64, 32);
		this->animationComponent->addAnimation("WALK_RIGHT",  1, 2, 0, 1, 64, 32);
		this->animationComponent->addAnimation("WALK_FORWARD",  1, 0, 0, 1, 64, 32);
		this->canShoot = false;
		this->damage = 0;
		break;

	case enemyType::COMMANDO:
		this->sprite.scale(5, 5);
		this->createAnimationComponent(Assets::Get().textures.at("COMMANDO_ENEMY"));
		this->animationComponent->addAnimation("WALK_LEFT", 1, 0, 0, 1, 32, 32);
		this->animationComponent->addAnimation("WALK_RIGHT", 1, 0, 0, 1, 32, 32);
		this->animationComponent->addAnimation("WALK_FORWARD", 1, 0, 0, 1, 32, 32);
		this->canShoot = true;
		this->damage = 1;
		this->shootTimerMax = 2;
		this->points *= 2;
		break;

	case enemyType::STATIONARY:
		this->sprite.scale(6, 6);
		this->createAnimationComponent(Assets::Get().textures.at("STATIONARY_ENEMY"));
		this->animationComponent->addAnimation("WALK_LEFT", 1, 0, 0, 1, 32, 32);
		this->animationComponent->addAnimation("WALK_RIGHT", 1, 0, 0, 1, 32, 32);
		this->animationComponent->addAnimation("WALK_FORWARD", 1, 0, 0, 1, 32, 32);
		this->canShoot = true;
		this->damage = 2;
		this->shootTimerMax = 1;
		this->speed = 0;
		this->size *= 2;
		this->health *= 4;
		this->points *= 4;
		break;

	case enemyType::BOSS:
		this->sprite.scale(9, 9);
		this->createAnimationComponent(Assets::Get().textures.at("BOSS_ENEMY"));
		this->animationComponent->addAnimation("WALK_LEFT", 1, 0, 0, 1, 32, 32);
		this->animationComponent->addAnimation("WALK_RIGHT", 1, 0, 0, 1, 32, 32);
		this->animationComponent->addAnimation("WALK_FORWARD", 1, 0, 0, 1, 32, 32);
		this->canShoot = true;
		this->damage = 4;
		this->shootTimerMax = 5;
		this->speed /= 1.5;
		this->size *= 4;
		this->health *= 10;
		this->points *= 10;
		break;
	}


	
	
	
	this->speed = this->speed * (50 / this->size);
	this->health = static_cast<int>(100 * this->size);
	this->maxHealth = this->health;
	this->points = static_cast<int>(this->size * 2);
	
	this->createHitboxComponent(0, 0, this->sprite.getGlobalBounds().width, this->sprite.getGlobalBounds().height);
	this->createMovementComponent(this->speed, this->speed * 2, this->speed * 8, this->speed * 3);
	this->createAIComponent(this->type, this->state);
	
	this->initVariables();
}

void Enemy::initVariables()
{
	this->state = enemyState::FULL;

	this->text.setFont(Assets::Get().font);
	this->text.setCharacterSize(30 * this->size);
	this->text.setPosition(this->getPosition());
	this->text.setFillColor(sf::Color::Red);

	this->shootTimer = 0;	

}

bool Enemy::isDead()
{
	if (this->state == enemyState::DEAD)
		return true;
	else return false;
}

void Enemy::move(const sf::Vector2f& target, const float& dt)
{
	sf::Vector2f movementDir = target - this->getCenterPosition();
	float distance = this->getDistance(target);

	sf::Vector2f finalMove = this->aiComponent->nextMove(util::normalize(movementDir) , distance);

	this->movementComponent->move(finalMove.x, finalMove.y, dt);
	this->text.setPosition(this->getPosition());
}

void Enemy::shoot(sf::Vector2f targetPos)
{
	//Enemy has to aggro you, and shoot only every some time and be the type that can shoot
	if (this->aiComponent->isAggro() && this->shootTimer >= this->shootTimerMax && this->canShoot == true) {

		switch (this->type)
		{
		case enemyType::COMMANDO:{
			int spray_angle = -5 + rand() % 10;
			sf::Vector2f spray = this->rotateVector(this->calculateDir(targetPos), spray_angle);
			bullets.emplace_back(Bullet(this->getCenterPosition(), spray, sf::Color::Green,
				this->damage, 15, 800, 1500));
		}
			break;

		case enemyType::STATIONARY:
			bullets.emplace_back(Bullet(this->getCenterPosition(), this->calculateDir(targetPos), sf::Color::Yellow,
				this->damage,30,600, 2000));
			break;

		case enemyType::BOSS:
			bullets.emplace_back(Bullet(this->getCenterPosition(), this->calculateDir(targetPos), sf::Color::Black,
				this->damage, 50, 500,1000));
			break;
		}
		this->shootTimer = 0;
	}
}

void Enemy::updateBullets(const float dt)
{
	//Does nothing all bullet logic is handled in player's updatedeEnemyBullet
}

void Enemy::updateAnimations(const float& dt)
{
	

	if (this->movementComponent->getState(movement_states::IDLE) == true) {
		this->animationComponent->play("WALK_FORWARD", dt);
	}
	else if (this->movementComponent->getState(movement_states::MOVING_UP) == true) {
		this->animationComponent->play("WALK_FORWARD", dt);
	}
	else if (this->movementComponent->getState(movement_states::MOVING_DOWN) == true) {
		this->animationComponent->play("WALK_FORWARD", dt);
	}
	else if (this->movementComponent->getState(movement_states::MOVING_LEFT) == true) {
		this->animationComponent->play("WALK_LEFT", dt);
	}
	else if (this->movementComponent->getState(movement_states::MOVING_RIGHT) == true) {
		this->animationComponent->play("WALK_RIGHT", dt);
	}
		
	
}




void Enemy::updateState()
{
	//this->text.setString(std::to_string(this->health));
	float healthPercent = static_cast<float>(this->health) / this->maxHealth;
	
	if (healthPercent > 0.75)
		this->state = enemyState::FULL;
	else if (healthPercent > 0.50)
		this->state = enemyState::SCRATCHED;
	else if (healthPercent > 0.25)
		this->state = enemyState::DAMAGED;
	else if (healthPercent > 0)
		this->state = enemyState::GRAVE;
	else {
		this->state = enemyState::DEAD;
	}
}

void Enemy::updateColor()
{
	switch (this->state) {
	case enemyState::FULL:
		this->sprite.setColor(sf::Color(255, 255, 255, 255));
		break;
	case enemyState::SCRATCHED:
		this->sprite.setColor(sf::Color(220, 180, 100, 255));
		break;
	case enemyState::DAMAGED:
		this->sprite.setColor(sf::Color(200, 100, 50, 255));
		break;
	case enemyState::GRAVE:
		this->sprite.setColor(sf::Color(170, 30, 10, 255));
		break;
	case enemyState::DEAD:
		this->sprite.setColor(sf::Color::Black);
		break;
	}

}



void Enemy::update(const sf::Vector2f& target, const float& dt)
{
	this->shootTimer += dt;
	Entity::update(dt);
	this->movementComponent->update(dt);
	this->hitboxComponent->update();
	this->aiComponent->update(dt);
	this->updateAnimations(dt);

	this->updateState();
	this->updateColor();
	

	this->move(target, dt);
	this->shoot(target);
	this->updateBullets(dt);

	std::stringstream ss;
	ss << this->health << "\n";
	this->text.setString(ss.str());
	
}

void Enemy::render(sf::RenderTarget& target) const
{
	for (const Bullet& bullet : this->bullets) {
		target.draw(bullet.getShape());
	}
	target.draw(this->sprite);
	this->hitboxComponent->render(target);
	//this->aiComponent->render(target);
	//target.draw(this->text);

}

const int Enemy::getPoints() const
{
	return this->points;
}

const enemyType Enemy::getType() const
{
	return this->type;
}

std::vector<Bullet>& Enemy::getBullets()
{
	return this->bullets;
}
