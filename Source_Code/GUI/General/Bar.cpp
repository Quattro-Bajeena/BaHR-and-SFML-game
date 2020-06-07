#include "stdafx.h"
#include "Bar.h"

gui::Bar::Bar(int x, int y, int width, int height, float value_max,
    bool horizontal, sf::Vector2f scale,
    sf::Color bar_color, sf::Color border_color, float outline_thickness,
    sf::Font& font)
    : valueMax(value_max), value(value_max), horizontal(horizontal), scale(scale)
{
    this->border.setPosition(x * scale.x, y*scale.y);
    this->border.setSize(sf::Vector2f(width, height));
    this->border.setScale(scale);
    
    this->border.setOutlineColor(border_color);
    this->border.setFillColor(sf::Color::Transparent);

    if (horizontal == true) {
        this->bar.setSize(sf::Vector2f(width, height));
        this->bar.setScale(scale);
        this->bar.setPosition(
            this->border.getPosition().x,
            (this->border.getPosition().y + this->border.getGlobalBounds().height - this->bar.getGlobalBounds().height) );
    }
    else {
        this->bar.setSize(sf::Vector2f(width, height));
        this->bar.setScale(-scale);
        this->bar.setPosition(
        (this->border.getPosition().x + this->border.getGlobalBounds().width - this->bar.getGlobalBounds().width) ,
            (this->border.getGlobalBounds().top + this->border.getGlobalBounds().height));

        
    }
    this->bar.setFillColor(bar_color);
    this->border.setOutlineThickness(static_cast<int>(outline_thickness * scale.x));

    this->fillPercent = this->valueMax / this->value;
    
    this->text.setFont(font);
    this->text.setCharacterSize(60 * scale.x);
    this->text.setFillColor(sf::Color(30,30,30));
    this->text.setString("test");
    this->text.setPosition(this->border.getPosition().x, this->border.getGlobalBounds().top - (this->text.getGlobalBounds().height * 1.3));
}

void gui::Bar::setPercent(float percent)
{
    this->fillPercent = percent;
}

void gui::Bar::setMaxValue(float value_max)
{
    this->valueMax = value_max;
}

void gui::Bar::update(const float value)
{
    
    this->value = value;
    this->fillPercent = this->value / this->valueMax;
    this->text.setString(std::to_string(this->fillPercent));

    if (this->horizontal == true) {
        this->bar.setScale(this->fillPercent * this->scale.x, this->scale.y);
        //this->bar.setSize(sf::Vector2f(this->border.getSize().x * this->fillPercent, this->bar.getSize().y));
    }
    else {
        this->bar.setScale(this->scale.x, -this->fillPercent * this->scale.y);
    }

}

void gui::Bar::render(bool text_display, sf::RenderTarget& target)
{
    target.draw(this->border);
    target.draw(this->bar);
    if(text_display == true)
        target.draw(this->text);
}
