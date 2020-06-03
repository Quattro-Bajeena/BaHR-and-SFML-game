#include "stdafx.h"
#include "AnimationComponent.h"


AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet):
	sprite(sprite), textureSheet(texture_sheet), lastAnimation(nullptr), priorityAnimation(nullptr)
{

}

AnimationComponent::~AnimationComponent()
{
	for (auto& i : animations) {
		delete i.second;
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
	float animation_timer,
	int start_frame_x, int start_frame_y, int frames_x, int frames_y,
	int width, int height)
{
	this->animations[key] = new Animation(
		this->sprite, this->textureSheet,
		animation_timer,
		start_frame_x, start_frame_y, frames_x, frames_y, width, height);
}


const bool& AnimationComponent::play(std::string key, const float& dt, 
	const float& modifier, const float modifier_max, const bool priority)
{
	
	if (this->priorityAnimation) { //if there is a priority animation
		if (this->priorityAnimation == this->animations[key]) {
			if (this->lastAnimation != this->animations[key])
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



		this->animations[key]->play(dt, abs(modifier / modifier_max));
	}

	return this->animations[key]->isDone();
	
}


