#ifndef __USER__
#define __USER__

#include "mylib.hpp"

using namespace std;

class User
{
public:
    int fd;
    string name;
    int pt;
    int state; // 0: login 1: waiting, 2: gaming
    User *original_user;
    User(int);
    ~User();
};

class BJ_Player : public User
{
public:
    int card[5];
    int game_state;
    int bet;
};

class CF_Player : public User
{
public:
    int game_state;
    int bet;
    bool first;
};

#endif