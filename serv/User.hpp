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

class Player : public User
{
public:
    Player(User *);
    ~Player();
    void update_point();
};

#endif