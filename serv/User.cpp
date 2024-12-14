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
    close(fd);
    cout << "Socket " << fd << " close succ.\n";
}