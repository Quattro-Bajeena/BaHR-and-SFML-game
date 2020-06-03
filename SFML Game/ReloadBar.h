#pragma once
namespace gui {
	class ReloadBar
	{
	private:
		sf::Vector2f offset;
		sf::Vector2f scale;

		sf::RectangleShape background;
		sf::RectangleShape line;
		sf::RectangleShape verticalBar;

		float reloadTimer;
		float reloadTimeMax;
		bool reloading;

		float speed;
		float distance;
	public:
		ReloadBar(int offset_x, int offset_y, int width, int height, sf::Vector2f scale);

		void reloadAnimationStart(const float reload_time_max);
		void update(sf::Vector2f taret_pos, const float& dt);
		void render(sf::RenderTarget& target);
	};

}

