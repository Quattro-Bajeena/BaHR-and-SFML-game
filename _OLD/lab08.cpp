#include "stdafx.h"


class CustomRectangleShape : public sf::RectangleShape {
private:
    sf::Vector2f speed;
    float rotSpeed;
    sf::FloatRect bounds;
    bool isSelected;
    sf::Color defaultColor;
public:
    CustomRectangleShape(sf::Vector2f size, sf::Vector2f position)
        :RectangleShape(size), speed(0,0), rotSpeed(0), bounds(0,0,0,0), isSelected(false)
    {
        this->setPosition(position);
        this->defaultColor = sf::Color(200, 250, 50);
        this->setFillColor(this->defaultColor);
    }

    void setSpeed(float speed_x, float speed_y, float rot_speed) {
        this->speed.x = speed_x;
        this->speed.y = speed_y;
        this->rotSpeed = rot_speed;
    }

    void setBounds(int left, int right, int top, int bottom) {
        this->bounds = sf::FloatRect(left, top, right - left, bottom - top);
    }

    void setBounds(sf::FloatRect rect) {
        this->bounds = rect;
    }

    void animate(const float& dt) {
        if (this->isSelected == false) {
            this->move(speed * dt);
            this->rotate(rotSpeed * dt);
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                this->move(0, -500 * dt);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                this->move(0, 500 * dt);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                this->move(-500 * dt, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                this->move(500 * dt,0 );
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                this->rotate(30 * dt);
            }

        }
        
    }

    void bounce() {
        if (this->isSelected == false) {
            if (this->getGlobalBounds().left <= this->bounds.left) {
                this->speed.x = abs(this->speed.x);
            }

            else if (this->getGlobalBounds().left + this->getGlobalBounds().width >= this->bounds.left + this->bounds.width) {
                this->speed.x = -1 * abs(this->speed.x);
            }

            if (this->getGlobalBounds().top <= this->bounds.top) {
                this->speed.y = abs(this->speed.y);
            }
            else if (this->getGlobalBounds().top + this->getGlobalBounds().height >= this->bounds.top + this->bounds.height) {
                this->speed.y = -1 * abs(this->speed.y);
            }
        }
        else {
            if (this->getGlobalBounds().left < this->bounds.left) {
                this->move(-this->getGlobalBounds().left + this->bounds.left, 0); 
            }
            else if (this->getGlobalBounds().left + this->getGlobalBounds().width > this->bounds.left + this->bounds.width) {
                this->move( 
                (this->bounds.left + this->bounds.width) - (this->getGlobalBounds().left + this->getGlobalBounds().width),
                    0);
            }

            if (this->getGlobalBounds().top < this->bounds.top) {
                this->move(0, -this->getGlobalBounds().top + this->bounds.top);
            }
            else if (this->getGlobalBounds().top + this->getGlobalBounds().height > this->bounds.top + this->bounds.height) {
                this->move(0,
                    (this->bounds.top  + this->bounds.height) - (this->getGlobalBounds().top + this->getGlobalBounds().height)
                    );
            }
        }
        
    }

    void select() {
        this->isSelected = true;
    }
    void unselect() {
        this->isSelected = false;
    }

    void updateSelect(sf::Vector2f mousePosView) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getGlobalBounds().contains(mousePosView)) {
            this->select();
            this->setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getGlobalBounds().contains(mousePosView)) {
            this->unselect();
            this->setFillColor(this->defaultColor);
        }
    }
};


int main_() {
    srand(time(NULL));
    //Systems
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "lab08", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    sf::Event sfEvent;
    sf::Clock clock;
    float dt = 0; //delta time == elapsed
    sf::FloatRect screenBounds(0, 0, window.getSize().x, window.getSize().y);

    sf::Vector2f mousePosView;

    //Objects
    std::vector<CustomRectangleShape> myRectangles;

    for (int i = 0; i < 10; i++) {
        CustomRectangleShape myRectangle(
            sf::Vector2f(200, 100),
            sf::Vector2f(
                rand()% (window.getSize().x-200),
                rand()% (window.getSize().y-300)
                )
            );
        myRectangle.setSpeed(100, 500, 10);
        myRectangle.setBounds(screenBounds);
        myRectangles.push_back(myRectangle);
    }
   


    //Main game loop
    while (window.isOpen()) {
        dt = clock.restart().asSeconds();

        while (window.pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed) {
                window.close();
            }
        }

        mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        

        for (auto& rectangle : myRectangles) {
            rectangle.animate(dt);
            rectangle.bounce();
            rectangle.updateSelect(mousePosView);
        }

        //Rendering
        window.clear(sf::Color(10,20,30));

        for (const auto& rectangle : myRectangles) {
            window.draw(rectangle);
        }

        window.display();
    }


}