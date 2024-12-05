//==========================================================
// include SFML library
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

// stadard C++ library
#include <string>
#include <cstdio>
#include <cstddef>
#include <deque>
#include <vector>

//==========================================================
// Error handler, static class
#define RUNNING_NULL_SCENE   1
#define CHANGE_TO_NULL_SCENE 2
#define DELETE_NULL_GAMEOBJECT 3
#define FONT_NOT_FOUND 4

#ifndef __ErrorHandler
#define __ErrorHandler
class ErrorHandler {
public:
    static void printError(uint32_t error_code);
};
#endif

//==========================================================
// Socket related ultilities
//
#define SOCK_SIZE 4096
#define INET4_ADDR_SIZE 16

//==========================================================
// Game Scene
// A "Scene" is defined as a unit of an executing game
// A Scene should perform fetching event, but the data of 
//  scoket buffer should be set by SceneManager
// We use the string libarary by SFML to handle wchar and
//  char idealy
// A Scene uses a "update" function to refreshing the screen
//  and get the next scene.
// A Scene uses a "blit" function to draw on the window
#ifndef __Scene
#define __Scene

#define SF_BG_COLOR sf::Color::White

class Scene {
private:
    sf::Clock clock;
    float delta_time = 0.0;
    char recvbuff[SOCK_SIZE], sendbuff[SOCK_SIZE];
    sf::String data;
    sf::String name;
public:

    // Member functions
    // [(de)(con)]structor
    Scene(sf::String name) : name(name) {};
    ~Scene() = default;

    // Socket connection related
    void setRecvbuff(char* sm_buff);
    void getSendbuff(char* sm_buff); 

    // Game execution related
    void setDeltaTime();
    virtual void init() = 0;
    virtual sf::String update(sf::RenderWindow* window) = 0;
    virtual void blit(sf::RenderWindow* window) = 0;
    const sf::String getName() const { return name; }
};
#endif

//==========================================================
// Game Scene Manager
// Managing all the Scene, also running the game
// The SceneManager object uses a "init" function to
//  initialize itself but not the constructor
#ifndef __SceneManager
#define __SceneManager
class SceneManager {
private:
    std::vector<Scene*> scenes;
    Scene* current_scene;

    // Socket connection related
    //char

    // SFML game related
    sf::String game_title;
    uint32_t width, height;
    sf::RenderWindow window;

public:
    // [(de)(con)]structor
    SceneManager(
        sf::String game_title,
        uint32_t width,
        uint32_t height
    );
    ~SceneManager() = default;


    // Game execution related
    void init();
    void runScene();
    void addScene(Scene* scene);
    void changeScene(sf::String name);
};
#endif