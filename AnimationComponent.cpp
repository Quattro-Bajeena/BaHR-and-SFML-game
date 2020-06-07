#include "stdafx.h"
#include "AnimationComponent.h"


AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet) :
	sprite(sprite), textureSheet(texture_sheet), lastAnimation(nullptr), priorityAnimation(nullptr),currentAnimation(nullptr)
{

}

AnimationComponent::~AnimationComponent()
{
	for (auto& [key, animation] : animations) {
		delete animation;
	}
}

//Accessors
const bool& AnimationComponent::isDone(const std::string key)
{
	return this->animations[key]->isDone();
}

//Functions
void AnimationComponent::resetPriority()
{
	this->priorityAnimation = nullptr;
}

void AnimationComponent::addAnimation(std::string key,
	float framerate,
	int start_frame_x, int start_frame_y, int frames,
	int width, int height)
{
	this->animations[key] = new Animation(
		this->sprite, this->textureSheet,
		framerate,
		start_frame_x, start_frame_y, frames, width, height);
}

void AnimationComponent::setAnimation(std::string key, bool priority)
{
	this->currentAnimation = animations.at(key);
	if (priority == true)
		this->priorityAnimation = this->currentAnimation;
	
}

const bool AnimationComponent::playCurrentAnimation(const float& dt)
{
	if (this->priorityAnimation) { //if there is a priority animation
		
		if (this->priorityAnimation == this->currentAnimation) { //if this animtion is a priority aimation, Okay, in the system as it is know this never happens
			if (this->lastAnimation != this->currentAnimation) //if this is a new animation, reset the old
			{
				if (this->lastAnimation != nullptr) {
					this->lastAnimation->reset();
				}
					
				this->lastAnimation = this->currentAnimation;
			}


			//play the current animation (which happens to be priorit), if the priority animtion is done remove it
			if (this->currentAnimation->play(dt)) {
				this->priorityAnimation->reset();
				this->priorityAnimation = nullptr;
			}

		}
		else {
			//std::cout << "non pririty animation tried to be played during priority one, but it wasnt\n";
			if (this->lastAnimation != currentAnimation) //if this is a new animation, reset the old
			{
				if (this->lastAnimation != nullptr) {
					this->lastAnimation->reset();
				}
					
				this->lastAnimation = currentAnimation;
			}

			//I'm not sure if this works
			//play the priority animtion instead, if the priority animtion is done remove it
			if (this->priorityAnimation->play(dt)) {
				this->priorityAnimation->reset();
				this->priorityAnimation = nullptr;
					
			}
		}
	}
	else { //Play animtion if no other priority animtion is set

		
		if (this->lastAnimation != this->currentAnimation) //if this is a new animation, reset the old
		{
			if (this->lastAnimation != nullptr) {
				this->lastAnimation->reset();
			}
				
			this->lastAnimation = this->currentAnimation;
		}

		this->currentAnimation->play(dt); //Play the animation
	}

	return this->currentAnimation->isDone(); //animation done = currently on first frame
}

//Legacy playing animtion system, TO DO what to do with this??
const bool& AnimationComponent::play(std::string key, const float& dt,
	const float& modifier, const float modifier_max, const bool priority)
{

	if (this->priorityAnimation) { //if there is a priority animation
		if (this->priorityAnimation == this->animations[key]) { //if this animtion is a priority aimation
			if (this->lastAnimation != this->animations[key]) //if this is a new animation, reset the old
			{
				if (this->lastAnimation != nullptr)
					this->lastAnimation->reset();
				this->lastAnimation = this->animations[key];
			}


			//if the priority animtion is done remove it
			if (this->animations[key]->play(dt, abs(modifier / modifier_max))) {
				this->priorityAnimation = nullptr;
			}
			
		}
		else {
			//std::cout << "non pririty animation tried to be played during priority one, but it wasnt\n";
			if (this->lastAnimation != this->animations[key]) //if this is a new animation, reset the old
			{
				if (this->lastAnimation != nullptr)
					this->lastAnimation->reset();
				this->lastAnimation = this->animations[key];
			}

			//I'm not sure if this works
			//play the priority animtion instead, if the priority animtion is done remove it
			if (this->priorityAnimation->play(dt)) {
				
				this->priorityAnimation = nullptr;
			}
		}
	}
	else { //Play animtion if no other priority animtion is set

		if (priority) { //if this is priority animation, set it
			this->priorityAnimation = this->animations[key];
		}
		if (this->lastAnimation != this->animations[key]) //if this is a new animation, reset the old
		{
			if (this->lastAnimation != nullptr)
				this->lastAnimation->reset();
			this->lastAnimation = this->animations[key];
		}



		this->animations[key]->play(dt, abs(modifier / modifier_max)); //Play the animation
	}

	return this->animations[key]->isDone(); //animation done = currently on first frame

}



