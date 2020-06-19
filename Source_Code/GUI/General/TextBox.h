#pragma once
namespace gui {
	class TextBox
	{
	private:
		guiStates state;

		bool pressed;
		const std::string defaultString;
		std::string string;
		sf::Text text;
		const sf::Font& font;
		const int characterLimit;
		const int fontSize;

		sf::RectangleShape shape;
		const sf::Vector2f orgPos;
		const sf::Vector2f orgSize;

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
		TextBox(float x, float y, float width, float height, const sf::Vector2f& scale,
			const sf::Font& font, const std::string default_text, int character_size, int character_limit,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_Color, sf::Color hover_Color, sf::Color active_Color,
			sf::Color outline_idle_Color = sf::Color::Black, sf::Color outline_hover_Color = sf::Color(10,10,0), sf::Color outline_active_Color = sf::Color(230, 200, 50),
			short unsigned id = 0);
		~TextBox();

		//Modifiers
		void scale(const sf::Vector2f& scale);
		void setString(const std::string string);

		//Accesors
		const sf::String getText() const;
		const bool isStringDefault() const;
		const bool isActive() const;

		void typeText(sf::Uint32 unicode_text);
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target) const;

	};
}



