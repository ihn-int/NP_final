#include "Scene.hpp"
#include "UserScene.hpp"

#define USAGE "usage: ./game <IPaddress> <UserId>"



int main(int argc, char** argv) {
    // Socket related
    // Use a stub system now
    if (argc < 3) {
        std::printf(USAGE);
        ErrorHandler::printError(INVALID_USAGE);
    }
    std::printf("User %s is connecting to server: %s ...\n",
        argv[2], argv[1]
    );

    // Scene Manager
    SceneManager sceneManager("Test", 800, 600);

    // Start Scene
    StartScene startScene("Start");
    sceneManager.addScene(&startScene);

    // BlackJack Scene
    BJScene bjScene("Blackjack");
    sceneManager.addScene(%bjScene);

    // Set Start Scene
    sceneManager.changeScene("Start");

    // Set BlackJack Scene

    /** /
    ChatScene chatScene("Chat");
    sceneManager.addScene(&chatScene);
    sceneManager.changeScene("Chat");
    /**/
    // Game Run
    sceneManager.runScene();

    return 0;
}