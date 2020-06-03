#include "stdafx.h"
#include "HealthBar.h"

gui::HealthBar::HealthBar(float x, float y, float width, float height, sf::Vector2f scale,
	sf::Texture& texture, int health)
	: texture(texture), heartScale(scale)
{
	this->currentHealth = 0;
	this->bounds.setPosition(x * scale.x, y * scale.y);
	this->bounds.setSize(sf::Vector2f(width, height));
	this->bounds.scale(scale);

	this->bounds.setFillColor(sf::Color(30, 30, 30, 100));
	this->bounds.setOutlineColor(sf::Color::Black);
	this->bounds.setOutlineThickness( static_cast<int>(2.f * scale.x));

	
	sf::Sprite heart;
	heart.setTexture(this->texture);
	heart.setScale( 4* heartScale.x, 4 *heartScale.y );

	int full_hearts = this->currentHealth / 4;
	int no_hearths = 0;

	this->update(health);
	
}

void gui::HealthBar::scale(const sf::Vector2f scale)
{
	

}

void gui::HealthBar::update(const int health)
{
	if (this->currentHealth != health) {
		this->currentHealth = health;
		sf::Sprite heart;
		heart.setTexture(this->texture);
		heart.setScale(4 * heartScale.x, 4 * heartScale.y);

		int full_hearts = this->currentHealth / 4;
		int no_hearths = 0;
		hearts.clear();

		while (full_hearts > 0) {
			heart.setTextureRect(sf::IntRect(0, 0, 16, 16));
			heart.setPosition(
				this->bounds.getPosition().x + no_hearths * 1.2f * heart.getGlobalBounds().width,
				this->bounds.getPosition().y);

			this->hearts.push_back(heart);
			no_hearths++;
			full_hearts--;
		}
		if (this->currentHealth % 4 != 0) {
			int hearth_part = 4 - health % 4;

			heart.setTextureRect(sf::IntRect(hearth_part * 16, 0, 16, 16));
			heart.setPosition(
				this->bounds.getPosition().x + no_hearths * 1.2f * heart.getGlobalBounds().width,
				this->bounds.getPosition().y);

			this->hearts.push_back(heart);
			no_hearths++;
		}
	}


}
	

void gui::HealthBar::render(sf::RenderTarget* target)
{
	target->draw(this->bounds);
	for (sf::Sprite& heart : this->hearts) {
		target->draw(heart);
	}
}
