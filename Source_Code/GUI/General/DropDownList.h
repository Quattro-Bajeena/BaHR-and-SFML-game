#pragma once
#include "Button.h"

namespace gui {
	class DropDownList {
	private:
		float keytime;
		float keytimeMax;
		const sf::Font& font;
		Button* activeElement;
		std::vector<Button*> list;
		bool showList;
		bool released;
		sf::FloatRect bounds;
		sf::FloatRect initialSize;
		sf::Vector2f orgScale;


	public:
		DropDownList(float x, float y, float width, float height, sf::Vector2f scale,
			const sf::Font& font, std::vector<std::string> list, const int default_index);
		~DropDownList();

		//Accesors
		const unsigned short getActiveElementId() const;
		const std::string getActiveText() const;
		const bool isActive() const;
		const bool isReleased() const;

		//Modifiers
		void scale(sf::Vector2f scale);

		//Functions
		void reload(const std::vector<std::string>& list);
		const bool getKeytime();
		void updateKeytime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};
}


