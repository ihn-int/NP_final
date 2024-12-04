//==========================================================
// include SFML library
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

// stadard C++ library
#include <string>
#include <cstdio>
#include <cstddef>

//==========================================================
// Error handler, static class
#define RUNNING_NULL_SCENE   1
#define CHANGE_TO_NULL_SCENE 2
#define DELETE_NULL_GAMEOBJECT 3

#ifndef __ErrorHandler
#define __ErrorHandler
class ErrorHandler {
public:
    static void printError(uint32_t error_code);
};
#endif

//==========================================================
// Game Object for game sprite
#ifndef __GameObject
#define __GameObject
class GameObject {
private:
    std::string tag;
    std::string name;
public:
    virtual std::string update(float delta_time) = 0;
    virtual void blit(sf::RenderWindow* window) = 0;
    std::string getTag() { return tag; }
    std::string getName() { return name; }
    void setTag(std::string _tag) { tag = _tag; }
    void setName(std::string _name) { name = _name; }
};
#endif
//==========================================================
// Game Scene
// A "Scene" is defined as a unit of an executing game
// A Sc
// A Scene uses a "Update" function to refreshing the screen
//
#ifndef __Scene
#define __Scene
class Scene {
private:
    sf::Clock clock;
    float delta_time = 0.0;
public:
    // All the things are public due to convenience
    // Fields
    std::string name;
    // Canva canva;
    std::vector<GameObject*> gameObjects;

    // Member functions
    Scene(std::string name);
    ~Scene() = default;
    std::string update();
    void setDeltaTime();
    void blit(sf::RenderWindow* window);
    void addGameObject(GameObject* gameObject);
    void deleteGameObject(GameObject* gameObject);
    const std::string getName() const { return name; }
};
#endif

//==========================================================
// Game Scene Manager
// Managing all the Scene, also running the game
#ifndef __SceneManager
#define __SceneManager
class SceneManager {
private:
    std::vector<Scene*> scenes;
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
    void addScene(Scene* scene);
    void changeScene(std::string name);
};
#endif
//==========================================================
// Canva for GUI


//==========================================================


//==========================================================