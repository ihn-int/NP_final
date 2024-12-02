//==========================================================
// include SFML library
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

//==========================================================
// Error handler, static class
#define RUNNING_NULL_SCENE 1

class ErrorHandler {
public:
    static PrintError(uint32_t error_code);
};


//==========================================================
// Game Scene
// A "Scene" is defined as a unit of an executing game
// A Sc
// A Scene uses a "Update" function to refreshing the screen
//
class Scene {
private:
    std::string tag;
    Canva canva;
    std::vector<GameObject> gameObjects;
    sf::Clock clock;
public:
    Scene() override;
    ~Scene() = default;
    virtual void Update() override;
    virtual void Blit() override;
};

//==========================================================
// Game Scene Manager
// Managing all the Scene, also running the game
class SceneManager {
private:
    std::string game_title;
    uint32_t width, height;
    std::vector<Scene> scenes;
    Scene* current_scene;
public:
    SceneManager(
        std::string game_title,
        uint32_t width,
        uint32_t height
    );
    ~SceneManager() = default;
    void RunScene();
    void ChangeScene(std::string tag);
};

//==========================================================



//==========================================================



//==========================================================


//==========================================================