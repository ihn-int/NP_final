// This file consists of all of the user defined game
// objects. Users must implement the below functions:
//   1. Constructor
//   2. Destructor
//   3. update
//   4. blit

#include "util.hpp"

class TestObject : public GameObject {
private:
    sf::CircleShape circle;
    bool direction;
    float speed;
public:
    TestObject(std::string tag, std::string name);
    ~TestObject();
    std::string update(float delta_time) override;
    void blit(sf::RenderWindow*) override;
};