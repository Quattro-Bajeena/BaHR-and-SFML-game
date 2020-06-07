#pragma once

namespace gui {

	class HealthBar
	{
	private:

		sf::Texture& texture;
		sf::RectangleShape bounds;
		std::vector<sf::Sprite> hearts;
		int currentHealth;
		sf::Vector2f heartScale;

	public:
		HealthBar(float x, float y, float width, float height, sf::Vector2f scale,
			sf::Texture& texture, int health
			);

		void scale(const sf::Vector2f scale);
		void update(const int health);
		void render(sf::RenderTarget* target);

	};
}


