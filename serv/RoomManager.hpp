#ifndef __ROOM_MANAGER__
#define __ROOM_MANAGER__
#include "mylib.hpp"
#include "User.hpp"
#include "Timer.hpp"
#include "StringBuff.hpp"

using namespace std;

class RoomManager
{
public:
    int current_user, maxfdp1;
    int game_state;
    int joinNum;
    bool update;
    fd_set allset, rset;
    pthread_mutex_t *mutex;
    Timer timer;
    StringBuff *fdBuff;
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
    void BJ_handle();
    void CF_handle();
    vector<User *> users;
    vector<void *> BJ_room;
    queue<User *> BJ_queue;
    vector<void *> CF_room;
    queue<User *> CF_queue;
};

void *lobby_run(void *);

class BJ_RoomManager : public RoomManager
{
public:
    BJ_RoomManager(Lobby_Manager *);
    ~BJ_RoomManager();
    queue<Player *> joinQueue;
    vector<int> cards;
    Player *players[4];
    int current_round, ready_player, hand[4][5], bet[4];
    bool die, newround, updateListen;
    Lobby_Manager *lobby;
    void updateMember();
    void join(User *);
    void leave(int, bool);
    void broadcast(string, int);
    void send(string, int);
    void setcard();
    void sethand();
    void getcard(int, int);
    void msg_wait();
    void msg_handle();
    void time_handle();
    void timeout();
    void open();
    int point_compute(int n);
};

void *BJ_run(void *);

class CF_RoomManager : public RoomManager
{
public:
    CF_RoomManager(Lobby_Manager *);
    ~CF_RoomManager();
    queue<Player *> joinQueue;
    Player *players[2];
    int current_round, ready_player;
    int board[8][7];
    bool die, fin, nextRound, surr, updateListen;
    Lobby_Manager *lobby;
    void updateMember();
    void join(User *);
    void leave(int, bool);
    void broadcast(string, int);
    void send(string, int);
    void msg_wait();
    void msg_handle();
    void time_handle();
    void timeout();
    bool checkWin(int, int);
};

void *CF_run(void *);

#endif