
#include "stdafx.h"
#include "Enemy.h"



Enemy::Enemy(sf::Vector2f position, AudioManager& audio)
	:Entity(audio)
{

	
	this->setPosition(position);
	this->initVariables();
}

void Enemy::initVariables()
{
	this->state = enemyState::FULL;

	this->text.setFont(Assets::Get().font);
	this->text.setCharacterSize(50);
	this->text.setPosition(this->getPosition());
	this->text.setFillColor(sf::Color::Yellow);
	this->text.setOutlineColor(sf::Color::Black);
	this->text.setOutlineThickness(5.f);

	this->animationSwitchTimer = 0.f;
	this->animationSwitchTimerMax = 1.f;
	this->shootTimer = 0;	

}

bool Enemy::isDead()
{
	if (this->state == enemyState::DEAD)
		return true;
	else return false;
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

void Enemy::move(const sf::Vector2f& target, const float& dt)
{
	sf::Vector2f movementDir = target - this->getCenterPosition();
	float distance = this->getDistance(target);

	sf::Vector2f finalMove = this->aiComponent->nextMove(util::normalize(movementDir) , distance);
	//std::cout << finalMove.x << " | " << finalMove.y << "\n";
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
			int spray_angle = RNG::get().randomF(-5,5);
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



void Enemy::updateAnimations(const float& dt)
{
	if (this->animationSwitchTimer >= this->animationSwitchTimerMax) {
		//Bad idea if the animtions actually have more frames, only for single iamges swicthing directions, prevents stutering on edges when ~45 degress of player


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
		
		this->animationSwitchTimer = 0.f;
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
	this->animationSwitchTimer += dt;

	Entity::update(dt);
	this->movementComponent->update(dt);
	this->hitboxComponent->update();
	this->aiComponent->update(dt);
	this->updateAnimations(dt);

	this->updateState();
	this->updateColor();
	

	this->move(target, dt);
	this->shoot(target);

	std::stringstream ss;
	ss << std::fixed << std::setprecision(0) <<this->movementComponent->getVelocity().x <<" | "<< this->movementComponent->getVelocity().y << "\n";
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


