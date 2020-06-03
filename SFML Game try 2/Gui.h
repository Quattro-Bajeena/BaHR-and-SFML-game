#ifndef GUI_H
#define GUI_H


enum class button_states{ BTN_IDLE=0, BTN_HOVER, BTN_ACTIVE, BTN_RELEASED };

namespace gui {
	class Button
	{
	private:
		button_states buttonState;
		short unsigned id;

		/*bool pressed;
		bool hover;*/


		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;

	public:
		Button(float x, float y, float width, float height,
			sf::Font* font, std::string text, int character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_Color, sf::Color hover_Color, sf::Color active_Color,
			sf::Color outline_idle_Color = sf::Color::Transparent , sf::Color outline_hover_Color = sf::Color::Transparent, sf::Color outline_active_Color = sf::Color::Transparent,
			short unsigned id = 0);
		~Button();

		//Accesors
		const bool isPressed() const; //not used anymore, for deletion
		const bool isReleased() const;
		const sf::String getText() const;
		const short unsigned& getId() const;

		//Modifiers
		void setText(const sf::String text);
		void setId(const short unsigned id);


		//Function
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};


	//DropDownList================
	class DropDownList {
	private:
		float keytime;
		float keytimeMax;
		sf::Font& font;
		Button* activeElement;
		std::vector<Button*> list;
		bool showList;


	public:
		DropDownList(float x, float y, float width, float height, sf::Font& font, std::string list[], int nrOfElements, const int default_index = 0);
		~DropDownList();

		//Accesors
		const unsigned short getActiveElementId() const;

		//Functions
		const bool getKeytime();
		void updateKeytime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};


	class TextureSelector {
	private:
		float keytime;
		float keytimeMax;
		int gridSize;
		bool active;
		bool hidden;
		Button* hideBtn;
		sf::RectangleShape bounds;
		sf::Sprite sheet;
		sf::RectangleShape selector;
		sf::Vector2u mousePosGrid;
		sf::IntRect textureRect;
		

	public:
		TextureSelector(float x, float y,
			float width, float height,
			int grid_size, const sf::Texture* texture_sheet,
			sf::Font& font, std::string buttonText);
		~TextureSelector();

		//Accesors
		const bool& getActive();
		const sf::IntRect& getTextureRect() const;
		const bool GetKeytime();

		//Functions
		
		void updateKeytime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};
}


#endif // !GUI_H