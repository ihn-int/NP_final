//==========================================================
// include SFML library
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

// stadard C++ library
#include <string>
#include <cstring>
#include <cstdio>
#include <cstddef>
#include <deque>
#include <vector>
#include <sstream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <chrono>
#include <algorithm>

// Socket related definition
extern "C" {
    #include "unp.h"
}

//==========================================================
// Error handler, static class
#define RUNNING_NULL_SCENE 1
#define CHANGE_TO_NULL_SCENE 2
#define DELETE_NULL_GAMEOBJECT 3
#define FONT_NOT_FOUND 4
#define IMAGE_NOT_FOUND 5
#define INVALID_USAGE 6
#define UNKNOWN_OPCODE 7

#ifndef __ErrorHandler
#define __ErrorHandler
class ErrorHandler {
public:
    static void printError(uint32_t error_code);
};
#endif

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
    sf::String name;

protected:
    // Socket related
    char recvline[MAXLINE], sendline[MAXLINE];
    bool is_send, is_recv;
        // The bool flags are asserted only if the tasks
        // should be handled.

public:

    // Member functions
    // [(de)(con)]structor
    Scene(sf::String name);
    ~Scene() = default;

    // Socket connection related
    void setRecvbuff(const char* sm_buff);
    void setRecvFlag(bool flag);
    void getSendbuff(char* sm_buff);
    bool getSendFlag();

    // Game execution related
    void setDeltaTime();
    virtual void init() = 0;
    virtual sf::String update(sf::RenderWindow* window) = 0;
    virtual void blit(sf::RenderWindow* window) = 0;
    const sf::String getName() const { return name; }
};
#endif
//==========================================================
#ifndef __StringBuff
#define __StringBuff

class StringBuff
{
public:
    std::map<int, std::string> buff;
    void write(int, const char *);
    std::string read(int);
    void clear(int);
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
    int sockfd;
    int read_num;
    char recvline[MAXLINE], sendline[MAXLINE];
    StringBuff str_buff;
    fd_set recv_set;
    struct timeval timeout;

    // SFML game related
    sf::String game_title;
    uint32_t width, height;
    sf::RenderWindow window;
public:
    // [(de)(con)]structor
    SceneManager(
        sf::String game_title,
        uint32_t width,
        uint32_t height,
        int sockfd
    );
    ~SceneManager() = default;


    // Game execution related
    void init();
    void runScene();
    void addScene(Scene* scene);
    void changeScene(sf::String name);
};
#endif