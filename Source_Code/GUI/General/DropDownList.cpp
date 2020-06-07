#include "stdafx.h"
#include "DropDownList.h"

gui::DropDownList::DropDownList(float x, float y,
	float width, float height, sf::Vector2f scale,
	const sf::Font& font, std::vector<std::string> list,
	const int default_index)
	:font(font), showList(false), keytimeMax(1.f), keytime(0.f), released(false), initialSize(x,y,width,height), orgScale(scale)
{
	int nrOfElements = list.size();
	this->activeElement = new Button(
		x, y, width, height, scale,
		&this->font, list[default_index], 36 ,
		sf::Color(230, 230, 15, 240), sf::Color(255, 255, 230, 255), sf::Color(230, 230, 15, 240),
		sf::Color(150, 150, 150, 120), sf::Color(120, 120, 90, 255), sf::Color(150, 150, 150, 120),
		sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 50), 
		default_index
		);

	for (int i = 0; i < nrOfElements; i++) {
		this->list.push_back(
			new Button(
				x, y + 3 + (height * (i + 1)), width, height, scale,
				&this->font, list[i], 32 ,
				sf::Color(15, 15, 15, 250), sf::Color(250, 250, 250, 200), sf::Color(10, 10, 10, 255),
				sf::Color(150, 150, 130, 240), sf::Color(100, 100, 80, 255), sf::Color(10, 10, 10, 50),
				sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
				i
				)
			);
	}

	this->scale(scale);
	this->bounds = sf::FloatRect(x * scale.x, y * scale.y, width * scale.x, (height * (nrOfElements + 1) + 3) * scale.y);
}

gui::DropDownList::~DropDownList()
{
	delete this->activeElement;
	for (auto& i : this->list) {
		delete i;
	}
}




void gui::DropDownList::reload(const std::vector<std::string>& list)
{
	/*
	int nrOfElements = list.size();
	this->list.push_back(
		new Button(
			this->initialSize.left, this->initialSize.top + 3 + (this->initialSize.height * (nrOfElements -1 + 1)), this->initialSize.width, this->initialSize.height,
			this->orgScale,
			&this->font, list[nrOfElements-1], 32,
			sf::Color(15, 15, 15, 250), sf::Color(250, 250, 250, 200), sf::Color(10, 10, 10, 255),
			sf::Color(150, 150, 130, 240), sf::Color(100, 100, 80, 255), sf::Color(10, 10, 10, 50),
			sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
			nrOfElements-1
		)
	);
	this->bounds.height += this->initialSize.height * this->orgScale.y;
	*/
}

//Accesors
const bool gui::DropDownList::getKeytime()
{
	if (this->keytime >= this->keytimeMax) {
		this->keytime = 0.f;
		return true;
	}
	else return false;
}


const unsigned short gui::DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}

const std::string gui::DropDownList::getActiveText() const
{
	return this->activeElement->getText();
}

const bool gui::DropDownList::isActive() const
{
	return this->showList;
}

const bool gui::DropDownList::isReleased() const
{
	return this->released;
}

void gui::DropDownList::scale(sf::Vector2f scale)
{
	this->activeElement->scale(scale);
	for (const auto& button : this->list) {
		button->scale(scale);
	}
}

//Functions
void gui::DropDownList::updateKeytime(const float& dt)
{
	//useless for me 
	if (this->keytime < this->keytimeMax)
		this->keytime += dt;
}

void gui::DropDownList::update(const sf::Vector2i& mousePosWindow)
{

	this->activeElement->update(mousePosWindow);
	this->released = false;

	//Show and hide the list
	if (this->activeElement->isReleased()) {
		if (this->showList)
			this->showList = false;
		else
			this->showList = true;
	}
	if (this->bounds.contains(static_cast<sf::Vector2f>(mousePosWindow)) == false && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		this->showList = false;
	}
	
	if (this->showList == true) {
		for (auto& button : this->list) {
			button->update(mousePosWindow);
			if (button->isReleased()) {
				this->showList = false;
				this->activeElement->setText(button->getText());
				this->activeElement->setId(button->getId());
				this->released = true;

			}
		}
	}



}

void gui::DropDownList::render(sf::RenderTarget& target)
{
	this->activeElement->render(target);
	if (this->showList == true) {
		for (auto& i : this->list) {
			i->render(target);
		}
	}
}