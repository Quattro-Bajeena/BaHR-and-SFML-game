#ifndef MOVEMENT_COMPONENT
#define MOVEMENT_COMPONENT


enum class movement_states{IDLE = 0, MOVING, MOVING_LEFT, MOVING_RIGHT, MOVING_UP, MOVING_DOWN};


class MovementComponent
{
private:
	sf::Sprite& sprite;

	float maxVelocity;
	float maxRollVelocity;
	float maxDefaultVelocity;
	float acceleration;
	float deceleration;
	bool rolling;

	float maxVelocityMultiplier;

	sf::Vector2f velocity;
	//Initializer functions

public:
	MovementComponent(sf::Sprite& sprite, float maxVelocity, float maxRollVelocity, float acceleration, float deceleration);
	virtual ~MovementComponent();
	std::map<std::string, bool> movementStates; // alternative to moving_x functions do rozwazenia

	//Accesors
	const float getMaxVelocity() const;
	const sf::Vector2f& getVelocity() const;

	//Functions
	const bool getState(const movement_states state) const;
	void stopVelocity();
	void stopVelocityX();
	void stopVelocityY();

	void startRoll();
	void endRoll();
	void move(const float dir_x, const float dir_y, const float& dt);
	void update(const float& dt);
};

#endif // !MOVEMENT_COMPONENT