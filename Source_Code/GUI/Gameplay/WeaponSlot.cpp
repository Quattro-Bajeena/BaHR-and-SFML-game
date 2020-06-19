#include "stdafx.h"
#include "WeaponSlot.h"

gui::WeaponSlot::WeaponSlot(int x, int y, int width, int height,
    int bar_width, int bar_height,
    Magazine::MagazineInfo magazine, std::map<gunModels, const sf::Texture*>& weapon_textures, gunModels model,
    sf::Vector2f scale, sf::Font& font)
    :magazineInfo(magazine),weaponTextures(weapon_textures), scale(scale)
{
    this->background.setPosition(x * scale.x, y*scale.y);
    this->background.setSize(sf::Vector2f(width * 7 / 10, height));
    this->background.setFillColor(sf::Color(100, 100, 100, 200));
    this->background.setScale(scale);

    this->ammoBar = std::make_unique<gui::Bar>(x + width*7.5/10, y, width*2.5/10, height, this->magazineInfo.clipSize, false, scale, sf::Color(230, 200, 50), sf::Color::Black, 5, font);
    this->ammoBar->update(this->magazineInfo.bulletsLoaded);
    this->currentGun = model;

    this->ammoCount.setString(this->magazineInfo.bulletsAllStr + "/" + this->magazineInfo.capacityStr);
    this->ammoCount.setPosition(x* scale.x, (y + 0.1* height)*scale.y);
    this->ammoCount.setFont(font);
    this->ammoCount.setCharacterSize(50 * scale.x);


    this->currGunSprite.setTexture(*this->weaponTextures.at(model), true);
    this->currGunSprite.setScale(scale);
    this->currGunSprite.setPosition((this->background.getPosition().x + this->background.getGlobalBounds().width/2 - this->currGunSprite.getGlobalBounds().width/2),
        (this->background.getPosition().y + this->background.getGlobalBounds().height/2 - this->currGunSprite.getGlobalBounds().height/2));
    
   
}

void gui::WeaponSlot::setCurrentGun(gunModels current_gun, const Magazine::MagazineInfo& magazine )
{
    this->magazineInfo = magazine;
    this->currentGun = current_gun;
    this->currGunSprite.setTexture(*this->weaponTextures.at(current_gun),true);
    this->currGunSprite.setPosition((this->background.getPosition().x + this->background.getGlobalBounds().width / 2 - this->currGunSprite.getGlobalBounds().width / 2),
        (this->background.getPosition().y + this->background.getGlobalBounds().height / 2 - this->currGunSprite.getGlobalBounds().height / 2));
    
}

void gui::WeaponSlot::update(const Magazine::MagazineInfo& magazine, const float& dt)
{
    this->magazineInfo = magazine;
    this->ammoBar->setMaxValue(this->magazineInfo.clipSize);
    this->ammoBar->update(this->magazineInfo.bulletsLoaded);
    this->ammoCount.setString(this->magazineInfo.bulletsAllStr + "/" + this->magazineInfo.capacityStr);

}

void gui::WeaponSlot::render(sf::RenderTarget& target) const
{
    target.draw(this->background);
    target.draw(this->currGunSprite);
    this->ammoBar->render(false,target);
    target.draw(this->ammoCount);
}
