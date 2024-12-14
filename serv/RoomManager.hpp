#ifndef __ROOM_MANAGER__
#define __ROOM_MANAGER__
#include "mylib.hpp"
#include "User.hpp"
#include "Timer.hpp"

using namespace std;

class RoomManager
{
public:
    int current_user, maxfdp1;
    int game_state;
    bool update;
    fd_set allset;
    pthread_mutex_t mutex;
    Timer timer;
    void join(User *);
};

class Lobby_Manager : public RoomManager
{
public:
    Lobby_Manager();
    ~Lobby_Manager();
    void updateListen();
    void join(User *);
    void leave(User *);
    vector<User *> users;
};

class BJ_RoomManager : public RoomManager
{
public:
    BJ_RoomManager(Lobby_Manager *lobby)
    {
        for (int i = 0; i <= 3; i++)
        {
            players[i] = nullptr;
        }
        this->lobby = lobby;
    }
    ~BJ_RoomManager()
    {
        for (int i = 1; i <= current_user; i++)
        {
            User *u = players[i]->original_user;
            u->pt = players[i]->pt;
            u->state = 1;
            lobby->update = 1;
            delete players[i];
        }
    }
    BJ_Player *players[4];
    Lobby_Manager *lobby;
};

class CF_RoomManager : public RoomManager
{
public:
    CF_RoomManager(Lobby_Manager *lobby)
    {
        for (int i = 0; i <= 1; i++)
        {
            players[i] = nullptr;
        }
        current_round = 0;
        this->lobby = lobby;
    }
    ~CF_RoomManager()
    {
        for (int i = 0; i <= 1; i++)
        {
            User *u = players[i]->original_user;
            u->pt = players[i]->pt;
            u->state = 1;
            lobby->update = 1;
            delete players[i];
        }
    }
    CF_Player *players[2];
    int current_round;
    Lobby_Manager *lobby;
};

#endif