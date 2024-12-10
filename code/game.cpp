//#include <SFML/Graphics.hpp>
#include "util.hpp"
#include "GameObject.hpp"


int main() {
    SceneManager sceneManager = SceneManager("Test", 800, 600);
    Scene startScene("Start");
    TestObject test("test", "test");
    startScene.addGameObject(&test);

    Scene chatScene("Chat");
    

    sceneManager.addScene(&startScene);
    sceneManager.changeScene("Start");
    sceneManager.runScene();
}
