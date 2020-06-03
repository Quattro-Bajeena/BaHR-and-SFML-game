#pragma once
#include "Bar.h"
#include "Gun.h"

namespace gui {
	class WeaponSlot
	{
	private:
		//GUI
		sf::RectangleShape background;
		sf::Text ammoCount;
		std::unique_ptr<gui::Bar> ammoBar;

		gunModels currentGun;
		sf::Sprite currGunSprite;

		//Resources
		std::map<gunModels, const sf::Texture* >& weaponTextures;

		Magazine::MagazineInfo magazineInfo;
		
		sf::Vector2f scale;

	public:
		WeaponSlot(int x, int y, int width, int height,
			int bar_width, int bar_height,
			Magazine::MagazineInfo magazine, std::map<gunModels, const sf::Texture* >& weapon_textures, gunModels model,
			sf::Vector2f scale, sf::Font& font);

		//Accesors

		//Functions
		void setCurrentGun(gunModels current_gun, const Magazine::MagazineInfo& magazine);

		void update(const Magazine::MagazineInfo& magazine, const float& dt);
		void render(sf::RenderTarget& target) const;
			
	};
}


