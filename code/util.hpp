//==========================================================
// include SFML library
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// stadard C++ library
#include <string>
#include <cstdio>
#include <cstddef>

//==========================================================
// Error handler, static class
#define RUNNING_NULL_SCENE   1
#define CHANGE_TO_NULL_SCENE 2

class ErrorHandler {
public:
    static void PrintError(uint32_t error_code);
};


//==========================================================
// Game Scene
// A "Scene" is defined as a unit of an executing game
// A Sc
// A Scene uses a "Update" function to refreshing the screen
//
class Scene {
public:
    // All the things are public due to convenience
    // Fields
    std::string tag;
    Canva canva;
    std::vector<GameObject> gameObjects;

    // Member functions
    Class();
    ~Class();
    void update();
    void blit();
    void enter();
    void addGameObjects(GameObject gameObject);
    
    const std::string getTag() const { return tag; }
};

//==========================================================
// Game Scene Manager
// Managing all the Scene, also running the game
class SceneManager {
private:
    std::vector<Scene> scenes;
    Scene* current_scene;

    // Below is what a SFML game needs
    std::string game_title;
    uint32_t width, height;
    sf::RenderWindow window;

public:
    SceneManager(
        std::string game_title,
        uint32_t width,
        uint32_t height
    );
    ~SceneManager() = default;
    void runScene();
    void changeScene(std::string tag);
};

//==========================================================
// Game Object for game sprite


//==========================================================
// Canva for GUI


//==========================================================


//==========================================================