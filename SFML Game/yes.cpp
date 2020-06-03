#include "stdafx.h"


int main_boring() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");



    sf::CircleShape circle(100.0);
    circle.setPosition(100.0, 300.0);
    circle.setFillColor(sf::Color(100, 250, 50));

    sf::RectangleShape rectangle(sf::Vector2f(240.f, 120.f));
    rectangle.setPosition(500.0, 400.0);
    rectangle.setFillColor(sf::Color(100, 50, 250));

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0.0, 0.0));
    triangle.setPoint(1, sf::Vector2f(0.0, 100.0));
    triangle.setPoint(2, sf::Vector2f(140.0, 40.0));
    triangle.setOutlineColor(sf::Color::Red);
    triangle.setOutlineThickness(5);
    triangle.setPosition(600.0, 100.0);

    sf::Clock clock;

    srand(time(NULL));
    float dt = 0;
    float bumpTimer = 0;
    float rectangleVelocityX = 200.f;
    float rectangleVelocityY = 250.f;
    float rectangleAngularVelocity = 10.f;

    float windowX = window.getSize().x;
    float windowY = window.getSize().y;

    sf::FloatRect windowTop(0, 1, windowX, 1);
    sf::FloatRect windowBottom(0, windowY, windowX, 1);
    sf::FloatRect windowRight(windowX, 0, 1, windowY);
    sf::FloatRect windowLeft(-1, 0, 1, windowY);


    while (window.isOpen()) {

        dt = clock.restart().asSeconds();
        bumpTimer += dt;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }

        }

        sf::FloatRect rectangleBounds = rectangle.getGlobalBounds();


        if (rectangleBounds.intersects(windowTop) || rectangleBounds.intersects(windowBottom)) {
            if (bumpTimer > 0.05) {
                rectangleVelocityY *= -1;
                bumpTimer = 0;
                rectangle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255, 255));
            }


        }

        if (rectangleBounds.intersects(windowRight) || rectangleBounds.intersects(windowLeft)) {
            if (bumpTimer > 0.05) {
                rectangleVelocityX *= -1;
                bumpTimer = 0;
                rectangle.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255, 255));
            }

        }

        sf::Vector2f rectangleVelocity(rectangleVelocityX, rectangleVelocityY);
        std::cout << dt << "\n";
        rectangle.move(rectangleVelocity * dt);
        rectangle.rotate(rectangleAngularVelocity * dt);



        window.clear(sf::Color::Black);


        window.draw(circle);
        window.draw(rectangle);
        window.draw(triangle);


        window.display();
    }

    return 1;
}


int main_yes()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;


    sf::RenderWindow greatWindow(sf::VideoMode(1920, 1080), "Mein greatWindow", sf::Style::Titlebar | sf::Style::Close, settings);
    greatWindow.setFramerateLimit(60);


    sf::Font testFont;
    testFont.loadFromFile("Assets/Fonts/impact.ttf");
    sf::Text testText;
    testText.setFont(testFont);
    testText.setString("Hello world");
    testText.setCharacterSize(50);
    testText.setFillColor(sf::Color::Yellow);
    testText.setStyle(sf::Text::Bold | sf::Text::StrikeThrough);
    testText.setPosition(500, 566);
    testText.setRotation(43);






    sf::Text testText2;
    testText2.setFont(testFont);
    testText2.setString("GRAPHIC DESIGN IS (NOT) MY PASSION \n 6.66");
    testText2.setCharacterSize(100);
    testText2.setFillColor(sf::Color::Magenta);
    testText2.setStyle(sf::Text::Italic | sf::Text::Underlined);
    testText2.setPosition(25, 800);

    sf::Sprite testSprite;
    sf::Texture testTexture;
    testTexture.loadFromFile("Assets/Textures/wypierdalaj.jpg");
    testTexture.setSmooth(true);
    testTexture.setRepeated(true);

    //testSprite.setOrigin(20, 100);
    testSprite.setPosition(1100, 100);
    testSprite.setTexture(testTexture);
    testSprite.scale(1.5, 1.5);

    sf::Texture texture2;
    texture2.loadFromFile("Assets/Textures/yes.png");

    sf::Texture texture3;
    texture3.loadFromFile("Assets/Textures/why 2.png");

    sf::CircleShape shape(200.0f);
    shape.setFillColor(sf::Color(100, 250, 50));

    shape.setPosition(960, 540);
    shape.setOutlineThickness(30.0f);
    shape.setOutlineColor(sf::Color(250, 150, 100));
    shape.setPointCount(10);

    shape.setTexture(&texture2);
    shape.setTextureRect(sf::IntRect(3, 3, 716, 697));
    shape.setOrigin(shape.getRadius(), shape.getRadius());

    sf::RectangleShape rectang(sf::Vector2f(200.f, 500.f));
    rectang.setPosition(40.f, 99.f);
    rectang.setFillColor(sf::Color(72, 12, 78));

    sf::ConvexShape convex;
    convex.setPointCount(5);

    convex.setPoint(0, sf::Vector2f(0.f, 0.f));
    convex.setPoint(1, sf::Vector2f(50.f, 7.f));
    convex.setPoint(2, sf::Vector2f(56.f, 30.f));
    convex.setPoint(3, sf::Vector2f(30.f, 35.f));
    convex.setPoint(4, sf::Vector2f(2.f, 10.f));
    convex.setScale(10.f, 10.f);
    convex.setFillColor(sf::Color(5, 240, 120, 160));
    convex.setPosition(1120, 600);
    convex.setTexture(&texture3);



    sf::VertexArray triangle(sf::Triangles, 3);

    triangle[0].position = sf::Vector2f(100.f, 100.f);
    triangle[1].position = sf::Vector2f(1000.f, 200.f);
    triangle[2].position = sf::Vector2f(800.f, 1100.f);

    triangle[0].color = sf::Color::Red;
    triangle[1].color = sf::Color::Blue;
    triangle[2].color = sf::Color::Green;


    sf::VertexArray quad(sf::Quads, 4);

    quad[0].position = sf::Vector2f(1500.f, 10.f);
    quad[1].position = sf::Vector2f(1920.f, 10.f);
    quad[2].position = sf::Vector2f(1920.f, 500.f);
    quad[3].position = sf::Vector2f(1500.f, 500.f);

    quad[0].texCoords = sf::Vector2f(0.f, 10.f);
    quad[1].texCoords = sf::Vector2f(1500.f, 30.f);
    quad[2].texCoords = sf::Vector2f(1300.f, 1000.f);
    quad[3].texCoords = sf::Vector2f(50.f, 1300.f);

    quad[0].color = sf::Color::Cyan;

    sf::Texture texture4;
    if (!texture4.loadFromFile("Assets/Textures/nauka programowania.png")) {
        std::cout << "lel error\n";
    }

    sf::Transform transform;
    transform.scale(0.9f, 1.05f);
    transform.rotate(3);

    sf::RenderStates states;
    states.transform = transform;
    states.texture = &texture4;

    sf::Clock clock;

    //Main program loop//////
    srand(time(NULL));
    float dt;
    float bumpTimer = 0;
    float sprite_velocity_x = 450.f;
    float sprite_velocity_y = 400.f;
    float sprite_angular_velocity = 10.f;


    float windowX = greatWindow.getSize().x;
    float windowY = greatWindow.getSize().y;

    sf::FloatRect windowTop(0, 1, windowX, 1);
    sf::FloatRect windowBottom(0, windowY, windowX, 1);
    sf::FloatRect windowRight(windowX, 0, 1, windowY);
    sf::FloatRect windowLeft(-1, 0, 1, windowY);

    while (greatWindow.isOpen()) {
        dt = clock.restart().asSeconds();
        bumpTimer += dt;


        sf::Event wEvent;
        while (greatWindow.pollEvent(wEvent)) {
            switch (wEvent.type) {
            case sf::Event::Closed:
                greatWindow.close();
                break;

            case sf::Event::KeyReleased:
                std::cout << "yo" << "\n";
                if (wEvent.key.code == sf::Keyboard::Space)
                    main_boring();
                if (wEvent.key.code == sf::Keyboard::Escape)
                    greatWindow.close();
                break;

            case sf::Event::TextEntered:
                if (wEvent.text.unicode < 128) {
                    printf("%c", wEvent.text.unicode);
                }
                break;

            default:
                break;

            }
        }


        sf::FloatRect sprite_bounds = testSprite.getGlobalBounds();


        if (sprite_bounds.intersects(windowTop) || sprite_bounds.intersects(windowBottom)) {
            if (bumpTimer > 0.05) {
                sprite_velocity_y *= -1;
                bumpTimer = 0;
                testSprite.setColor(sf::Color(rand() % 205 + 50, rand() % 205 + 50, rand() % 205 + 50, 255));

            }
        }

        if (sprite_bounds.intersects(windowRight) || sprite_bounds.intersects(windowLeft)) {
            if (bumpTimer > 0.05) {
                sprite_velocity_x *= -1;
                bumpTimer = 0;
                testSprite.setColor(sf::Color(rand() % 205 + 50, rand() % 205 + 50, rand() % 205 + 50, 255));

            }

        }
        sf::Vector2f spriteVelocity(sprite_velocity_x, sprite_velocity_y);


        testSprite.move(spriteVelocity * dt);
        testSprite.rotate(sprite_angular_velocity * dt);



        greatWindow.clear(sf::Color::Blue);

        greatWindow.draw(triangle);
        greatWindow.draw(quad, states);
        greatWindow.draw(testSprite);
        greatWindow.draw(testText);
        greatWindow.draw(shape);
        greatWindow.draw(rectang);
        greatWindow.draw(testText2);
        greatWindow.draw(convex);

        greatWindow.display();
    }

    return 0;
}