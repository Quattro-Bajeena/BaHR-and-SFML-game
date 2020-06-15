#pragma once
#include "Button.h"

namespace gui {
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

		const sf::Vector2f orgScale;
	public:
		TextureSelector(float x, float y,
			float width, float height, sf::Vector2f scale,
			int grid_size, const sf::Texture* texture_sheet,
			sf::Font& font, std::string buttonText);
		~TextureSelector();

		//Accesors
		const bool& getActive();
		const sf::IntRect& getTextureRect() const;
		const bool GetKeytime();

		//Modifiers
		void scale(const sf::Vector2f& scale);

		//Functions
		void switchHidden();
		void updateKeytime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};
}


