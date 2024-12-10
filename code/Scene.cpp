#include "util.hpp"

Scene::Scene(
    std::string name
) : name(name),
    gameObjects(std::vector<GameObject*>()),
    clock(sf::Clock())
    {
        clock.restart();
    }

void Scene::setDeltaTime() {
    delta_time = clock.getElapsedTime().asSeconds();
    clock.restart();
}

std::string Scene::update() {
    setDeltaTime();
    for (auto gameObject : gameObjects) {
        gameObject->update(delta_time);
    }
    std::string next_scene = "";
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        next_scene = "Chat";
    }
    // next_scene = canva.update();
    return next_scene;
}

void Scene::blit(sf::RenderWindow* window) {
    window->clear(sf::Color::White);
    for (auto gameObject : gameObjects) {
        gameObject->blit(window);
    }
    // canva.blit(window);
    window->display();
}

void Scene::addGameObject(GameObject* gameObject) {
    gameObjects.emplace_back(gameObject);
}

void Scene::deleteGameObject(GameObject* gameObject) {
    for (auto _gameObject : gameObjects) {
        if (_gameObject == gameObject) {
            delete _gameObject;
        }
    }
    // Delete a non-exist game object
    ErrorHandler::printError(DELETE_NULL_GAMEOBJECT);
}