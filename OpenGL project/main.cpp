
//
//  main.cpp
//  sfml_opengl
//
//  Created by Jakub Tomczyński on 27/02/2019.
//  Copyright © 2019 Jakub Tomczyński. All rights reserved.
//

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <vector>
#include <fstream>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#ifdef __APPLE__
#include <OpenGL/glu.h> // macOS
#else
#include <GL/glu.h> // Windows/Linux
#endif



void draw_cube(double size, sf::Vector3f color) {
    double half_cube_size = size / 2.0;

    // bottom
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, -half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, -half_cube_size);
    glEnd();

    // top
    glColor3d(color.x, color.y, color.z);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glEnd();

    // left
    glColor3d(color.x, color.y, color.z);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(-half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, -half_cube_size);
    glEnd();

    // right
    glColor3d(color.x, color.y, color.z);
    glBegin(GL_POLYGON);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, -half_cube_size);
    glEnd();

    // front
    glColor3d(color.x, color.y, color.z);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, -half_cube_size, -half_cube_size);
    glVertex3d(-half_cube_size, -half_cube_size, -half_cube_size);
    glEnd();

    // back
    glColor3d(color.x, color.y, color.z);
    glBegin(GL_POLYGON);
    glVertex3d(-half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, half_cube_size);
    glVertex3d(half_cube_size, half_cube_size, -half_cube_size);
    glVertex3d(-half_cube_size, half_cube_size, -half_cube_size);
    glEnd();
}

void set_viewport(int width, int height) {
    //Proporcje ekranu
    const float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-ar, ar, -1.0, 1.0, 2, 10000.0);
    gluPerspective(45.f, ar, 1, 100000.0);
    gluLookAt(1, 8000, 2000, 0, 0, 0, 0, 0, 1);
    

    //    glMatrixMode(GL_MODELVIEW);
    //    glLoadIdentity();
}

class CelestialBody {
private:
    std::string name;
    
    float sunDistance;
    float diameter;
    float circularPeriod;
    float rotationPeriod;
    
    sf::Vector3f color;

    
    float rotation = 0;
    float circularRotation = 0;
public:
    CelestialBody(std::string name, float sun_distance, float diameter, float rotation_period,float circular_period,sf::Vector3f color)
        :name(name), diameter(diameter), sunDistance(sun_distance), rotationPeriod(rotation_period), circularPeriod(circular_period), color(color)
    {
        //Converting from 0-255 values to 0-1 values
        this->color.x /= 255.0;
        this->color.y /= 255.0;
        this->color.z /= 255.0;

    }
    void step(float time) {
        
        if (this->circularPeriod != 0)
            this->circularRotation += time * (360.0 / this->circularPeriod);
        else this->circularRotation = 0;

        this->rotation += time * (360 / this->rotationPeriod);
    }

    void draw() {
        glPushMatrix(); 

        glRotated(this->circularRotation, 0.0, 0.0, 1.0);
        glTranslated(0, this->sunDistance, 0);
        glRotated(this->rotation, 0.0, 0.0, 1.0);
           
        draw_cube(this->diameter, this->color);

        glPopMatrix();
    }

};

std::vector<CelestialBody> createPlanets() {
    std::vector<CelestialBody> planets;
    std::ifstream in_file("solar_system.txt", std::ios::in);

    if (in_file.is_open()) {
        in_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string name;
        float distance;
        float diameter;
        float spin_time;
        float orbit_time;
        float gravity;
        float moons;
        sf::Vector3f color;

        while (in_file >> name >> distance >> diameter >> spin_time >> orbit_time >> gravity >> moons >> color.x >> color.y >> color.z) {

            std::cout << name << " " << distance << " " << diameter << " " << spin_time
                << " " << orbit_time << " " << gravity << " "
                << moons << " " << color.x << " " << color.y << " " << color.z << "\n";

            planets.emplace_back(CelestialBody(name, distance, diameter, spin_time, orbit_time, color));
        }

    }
    else std::cout << "Couldn't open the file" << "\n";

    return planets;
}


int main() {
    // create the window
    sf::Window window(sf::VideoMode(1920, 1080), "Planetarium", sf::Style::Default, sf::ContextSettings(24));
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // set viewport according to current window size
    set_viewport(window.getSize().x, window.getSize().y);

    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    glShadeModel(GL_SMOOTH);


    // setup lights

    GLfloat light_position[] = { 2.0, 0.0, 2.0, 1.0 };
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat global_ambient[] = { 0.3, 0.3, 0.3, 0.1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_NORMALIZE);

    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running = true;



    //Camera setup
    sf::Vector2f cameraPos{ 0,0 };
    float cameraSpeed = 100;
    float camZoom = 1;

    sf::Clock clk;
    sf::Clock clock;
    float dt;

    std::vector<CelestialBody> planets = createPlanets();

    while (running) {
        //One second is ~one month
        dt = clock.restart().asSeconds();

        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized) {
                // adjust the viewport when the window is resized
                set_viewport(event.size.width, event.size.height);
            }
           

        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            cameraPos.y += cameraSpeed * dt;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            cameraPos.y += -cameraSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            cameraPos.x += cameraSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            cameraPos.x += -cameraSpeed * dt;
        }
   
        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);

        
        // draw stuff
        for (CelestialBody& planet : planets) {
            planet.step(dt);
        }

        //Camera movement
        glPushMatrix();
        glTranslated(cameraPos.x, cameraPos.y, 0);
        glScaled(camZoom, camZoom, camZoom);


        for (CelestialBody& planet : planets) {
            planet.draw();
        }

        glPopMatrix();
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    return 0;
}

