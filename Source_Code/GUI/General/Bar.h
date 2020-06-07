#pragma once
namespace gui {
	class Bar
	{
	private:
		sf::Text text;
		sf::RectangleShape bar;
		sf::RectangleShape border;

		float value;
		float valueMax;
		float fillPercent;
		bool horizontal;

		sf::Vector2f scale;
	public:
		Bar(int x, int y, int width, int height, float value_max,
			bool horizontal, sf::Vector2f scale,
			sf::Color bar_color, sf::Color border_color, float outline_thickness,
			sf::Font& font);

		~Bar() = default;

		void setPercent(float percent);
		void setMaxValue(float value_max);
		void update(const float value);
		void render(bool text_display, sf::RenderTarget& target);
	};
}


