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

void Scene::setRecvbuff(const char* sm_buff) {
    // SceneManager writes data into recvbuff
    std::strcpy(recvline, sm_buff);
}

void Scene::setRecvFlag(bool flag) {
    is_recv = flag;
}

void Scene::getSendbuff(char* sm_buff) {
    // SceneManager reads data from sendbuff
    std::strcpy(sm_buff, sendline);
}

bool Scene::getSendFlag() {
    return is_send;
}
