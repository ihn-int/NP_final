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
    User() {};
    User(int);
    ~User();
};

class BJ_Player : public User
{
public:
    int card[5];
    int bet;
};

class CF_Player : public User
{
public:
    int bet;
    bool first;
    CF_Player(User *);
    ~CF_Player();
};

#endif