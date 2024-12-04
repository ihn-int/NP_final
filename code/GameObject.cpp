// This file consists of all of the user defined game
// objects. Users must implement the below functions:
//   1. Constructor
//   2. Destructor
//   3. update
//   4. blit

#include "GameObject.hpp"

TestObject::TestObject(
    std::string tag,
    std::string name
) {
    setTag(tag);
    setName(name);
    direction =true;
    speed = 200.0;
    circle = sf::CircleShape(100.0);
    circle.setFillColor(sf::Color::Black);
    circle.setPosition(100.0, 300.0);
}

TestObject::~TestObject() {}

std::string TestObject::update(float delta_time) {
    float xPos = circle.getPosition().x;
    if (xPos >= 500.0) {
        direction = false;
    }
    else if (xPos <= 100) {
        direction = true;
    }
    if (direction) {
        circle.move(speed * delta_time, 0);
    }
    else {
        circle.move(-speed * delta_time, 0);
    }
    return "";
}

void TestObject::blit(sf::RenderWindow* window) {
    window->draw(circle);
}