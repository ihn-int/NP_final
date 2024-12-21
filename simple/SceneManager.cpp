#include "Scene.hpp"

SceneManager::SceneManager(
    sf::String game_title,
    uint32_t width,
    uint32_t height,
    int sockfd
) : game_title(game_title),
    width(width),
    height(height),
    sockfd(sockfd),
    read_num(0)
    {
        init();
    }

void SceneManager::init() {

    // Create window
    glEnable(GL_TEXTURE_2D);
    sf::ContextSettings settings(24, 8, 4, 4, 3, 0, false);
    window.create(
        sf::VideoMode(width, height),
        game_title,
        sf::Style::Titlebar | sf::Style::Close,
        settings
    );
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    // Scene container
    scenes = std::vector<Scene*>(0);
}

void SceneManager::addScene(Scene* scene) {
    scenes.emplace_back(scene);
}

void SceneManager::changeScene(sf::String name) {
    std::printf("[INFO] Jump to %s scene.\n",
        name.toAnsiString().c_str());
    for (auto scene : scenes) {
        if (scene->getName() == name) {
            current_scene = scene;
            current_scene->init();
            return;
        }
    }
    ErrorHandler::printError(CHANGE_TO_NULL_SCENE);
}

void SceneManager::runScene() {
    // Check current scene
    if (current_scene == nullptr) {
        ErrorHandler::printError(RUNNING_NULL_SCENE);
    }

    // Check if wondow open
    while (window.isOpen()) {
        // Check socket
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        FD_ZERO(&recv_set);
        FD_SET(sockfd, &recv_set);
        read_num = select(sockfd+1, &recv_set, NULL, NULL, &timeout);
        if (read_num > 0 && FD_ISSET(sockfd, &recv_set)){
            // Try to invoke buffer
            readline(sockfd, recvline, MAXLINE);
            std::printf("[RECV] %s\n", recvline);
            current_scene->setRecvbuff(recvline);
            current_scene->setRecvFlag(true);
        }
        else {
            current_scene->setRecvFlag(false);
        }

        // Invoking update function
        sf::String next_scene = current_scene->update(&window);
        
        // Send data if needed
        if (current_scene->getSendFlag()) {
            current_scene->getSendbuff(sendline);
            writen(sockfd, sendline, strlen(sendline));
            std::printf("[SEND] %s\n", sendline);
        }
        
        // Blit the window
        current_scene->blit(&window);

        // Change next scene if needed
        if (next_scene != "") {
            if (next_scene == "Exit") {
                // User quit
                sprintf(sendline, "999\n");
                writen(sockfd, sendline, strlen(sendline));
                std::printf("[SEND] %s\n", sendline);
                close(sockfd);
                window.close();
            }
            else {
                changeScene(next_scene);
            }
        }
    }
}