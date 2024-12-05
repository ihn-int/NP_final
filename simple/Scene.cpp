// Base Scene
#include "Scene.hpp"

Scene::Scene(
    sf::String name
) : name(name),
    clock(sf::Clock())
    {}

void Scene::setDeltaTime() {
    delta_time = clock.getElapsedTime().asSeconds();
    clock.restart();
}

// Other functions should be override
