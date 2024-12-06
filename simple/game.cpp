#include "Scene.hpp"
#include "UserScene.hpp"

int main(int argc, char** argv) {

    // Scene Manager
    SceneManager sceneManager("Test", 800, 600);

    // Start Scene
    StartScene startScene("Start");
    sceneManager.addScene(&startScene);

    // Set Start Scene
    sceneManager.changeScene("Start");

    /** /
    ChatScene chatScene("Chat");
    sceneManager.addScene(&chatScene);
    sceneManager.changeScene("Chat");
    /**/
    // Game Run
    sceneManager.runScene();

    return 0;
}