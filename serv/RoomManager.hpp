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
    int timePoint;
    bool update;
    fd_set allset, rset;
    pthread_mutex_t *mutex;
    Timer timer;
};

class Lobby_Manager : public RoomManager
{
public:
    Lobby_Manager();
    ~Lobby_Manager();
    void updateListen();
    void join(User *);
    void leave(User *);
    int msg_wait();
    void msg_handle(int &);
    void CF_handle();
    vector<User *> users;
    vector<void *> CF_room;
    queue<User *> CF_queue;
};

void *lobby_run(void *);

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
    CF_RoomManager(Lobby_Manager *);
    ~CF_RoomManager();
    CF_Player *players[2];
    int current_round, ready_player;
    int board[7][6];
    bool die;
    Lobby_Manager *lobby;
    void updateMember();
    void join(User *);
    void leave(int, bool);
    void broadcast(string, int);
    void send(string, int);
    bool msg_wait();
    void msg_handle();
    void timeout();
};

void *CF_run(void *);

#endif