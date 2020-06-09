#pragma once
namespace gui {
	class CheckBox
	{
	private:
		guiStates state;
		bool checked;
		int id;
		float ratio;

		sf::RectangleShape boxShape;
		sf::RectangleShape checkShape;

		const sf::Vector2f orgPos;
		const sf::Vector2f orgSize;


		const sf::Color boxIdleColor;
		const sf::Color boxHoverColor;
		const sf::Color boxActiveColor;

		const sf::Color checkIdleColor;
		const sf::Color checkHoverColor;
		const sf::Color checkActiveColor;

		const sf::Color outlineIdleColor;
		const sf::Color outlineHoverColor;
		const sf::Color outlineActiveColor;

	public:
		CheckBox(float x, float y, float width, float height, sf::Vector2f scale,
			sf::Color check_idle_Color, sf::Color check_hover_Color, sf::Color check_active_Color,
			sf::Color outline_idle_Color, sf::Color outline_hover_Color , sf::Color outline_active_Color,
			sf::Color box_idle_Color = sf::Color::White, sf::Color box_hover_Color = sf::Color::White, sf::Color box_active_Color = sf::Color::White,
			int id = 0);
		

		//Accesors
		const bool isChecked() const;
		const int getId() const;

		//Modifiers
		void check();
		void setCheck(bool condition);
		void scale(sf::Vector2f scale);
		void setId(const int id);


		//Function
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);

	};
}


