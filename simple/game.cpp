#include "Scene.hpp"
#include "UserScene.hpp"

int main() {
    SceneManager sceneManager("Test", 800, 600);
    ChatScene chatScene("Chat");
    sceneManager.addScene(&chatScene);
    sceneManager.changeScene("Chat");
    sceneManager.runScene();

    return 0;
}