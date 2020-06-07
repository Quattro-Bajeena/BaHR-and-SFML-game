#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H


class AnimationComponent
{
private:
	class Animation {
	public:
		sf::Sprite& sprite;
		sf::Texture& textureSheet;
		float framerate; //framerate FPS
		float timer;
		bool done;
		int width;
		int height;
		sf::IntRect startRect;
		sf::IntRect currentRect;
		sf::IntRect endRect;
		//TO DO CZHANGE HOW END RECT IS MADE
		Animation(sf::Sprite& sprite, sf::Texture& texture_sheet,
			float framerate,
			int start_frame_x, int start_frame_y, int frames,
			int width, int height)
			: sprite(sprite), textureSheet(texture_sheet), width(width), height(height),
			framerate(framerate), timer(0.f), done(false)
		{
			this->timer = 0.f;
			this->startRect = sf::IntRect(start_frame_x * width, start_frame_y * height, width, height);
			this->currentRect = this->startRect;
			this->endRect = sf::IntRect((start_frame_x + frames-1) * width, start_frame_y * height, width, height);

			this->sprite.setTexture(this->textureSheet, true);
			this->sprite.setTextureRect(this->startRect);

		}
		//Accesor
		const bool& isDone() const {
			return done;
		}

		//Functions

		const bool& play(const float& dt, float mod_percent = 1) {
			this->done = false;

			//update timer
			if (mod_percent < 0.2f)
				mod_percent = 0.2f;
			this->timer += mod_percent * dt;
			//std::cout<< mod_percent<<"\n";
			
			if (this->timer >= 1.f / this->framerate) { //Next frame

				//reset timer
				this->timer = 0.f;

				//Amimate
				if (this->currentRect.left != this->endRect.left) {
					this->currentRect.left += this->width;
				}
				else {//Reset
					
					this->currentRect.left = this->startRect.left;
					this->done = true;
				}
			}
			

			this->sprite.setTextureRect(this->currentRect);

			return this->done;
		}

		void reset() {
			this->timer = 0.f;
			this->currentRect = this->startRect;
			this->done = true;

		}

	};
	/////////////////////////////////////

	sf::Sprite& sprite;
	sf::Texture& textureSheet;
	std::map<std::string, Animation*> animations;
	Animation* lastAnimation;
	Animation* priorityAnimation;
	Animation* currentAnimation;

public:
	AnimationComponent(sf::Sprite& sprite, sf::Texture& texture_sheet);
	virtual ~AnimationComponent();

	//Accesor
	const bool& isDone(const std::string key);

	//Functions
	void resetPriority();
	void addAnimation(std::string key,
		float framerate,
		int start_frame_x, int start_frame_y, int frames,
		int width, int height);
	void setAnimation(std::string key, bool priority = false);
	const bool playCurrentAnimation(const float& dt);

	const bool& play(std::string key, const float& dt,
		const float& modifier = 1.f, const float modifier_max = 1.f, const bool priority = false);


};

#endif // !ANIMATIONCOMPONENT_H

