#pragma once
#include "Button.h"

namespace gui {
	class DropDownList {
	private:
		float keytime;
		float keytimeMax;
		sf::Font& font;
		Button* activeElement;
		std::vector<Button*> list;
		bool showList;
		sf::FloatRect bounds;


	public:
		DropDownList(float x, float y, float width, float height, sf::Vector2f scale,
			sf::Font& font, std::string list[], int nrOfElements, const int default_index);
		~DropDownList();

		//Accesors
		const unsigned short getActiveElementId() const;
		const bool isActive() const;

		//Modifiers
		void scale(sf::Vector2f scale);

		//Functions
		const bool getKeytime();
		void updateKeytime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};
}


