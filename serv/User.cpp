#include "mylib.hpp"
#include "User.hpp"

using namespace std;

User::User(int fd)
{
    this->fd = fd;
    this->pt = 10000;
    this->state = 0;
    this->original_user = this;
}

User::~User()
{
    if (fd >= 0)
    {
        close(fd);
        cout << "Socket " << fd << " close succ.\n";
    }
}

CF_Player::CF_Player(User *u)
{
    fd = u->fd;
    pt = u->pt;
    state = 0;
    original_user = u;
    bet = 0;
    name = u->name;
    first = false;
}

CF_Player::~CF_Player()
{
    fd = -1;
    original_user = nullptr;
}