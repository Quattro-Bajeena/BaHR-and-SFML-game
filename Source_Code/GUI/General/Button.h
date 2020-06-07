#pragma once

//enum class guiStates { IDLE = 0, HOVER, ACTIVE, RELEASED };
namespace gui {
	class Button
	{
	private:
		guiStates buttonState;
		short unsigned id;

		sf::RectangleShape shape;
		const sf::Font* font;
		sf::Text text;
		const sf::Vector2f orgPos;
		const sf::Vector2f orgSize;
		const int orgFontSize;

		const sf::Color textIdleColor;
		const sf::Color textHoverColor;
		const sf::Color textActiveColor;

		const sf::Color idleColor;
		const sf::Color hoverColor;
		const sf::Color activeColor;

		const sf::Color outlineIdleColor;
		const sf::Color outlineHoverColor;
		const sf::Color outlineActiveColor;

	public:
		Button(float x, float y, float width, float height, sf::Vector2f scale,
			const sf::Font* font, std::string text, int character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_Color, sf::Color hover_Color, sf::Color active_Color,
			sf::Color outline_idle_Color = sf::Color::Transparent, sf::Color outline_hover_Color = sf::Color::Transparent, sf::Color outline_active_Color = sf::Color::Transparent,
			short unsigned id = 0);
		~Button();

		//Accesors
		const bool isPressed() const; //not used anymore, for deletion
		const bool isReleased() const;
		const sf::String getText() const;
		const short unsigned& getId() const;

		//Modifiers
		void scale(sf::Vector2f scale);
		void setText(const sf::String text);
		void setId(const short unsigned id);


		//Function
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};
}

