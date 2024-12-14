#include "mylib.hpp"
#include "RoomManager.hpp"

using namespace std;

Lobby_Manager::Lobby_Manager()
{
    pthread_mutex_init(&mutex, 0);
    users.clear();
    FD_ZERO(&allset);
    cout << "Create lobby\n";
}

Lobby_Manager::~Lobby_Manager()
{
    for (auto u : users)
    {
        delete u;
    }
}

void Lobby_Manager::join(User *u)
{
    pthread_mutex_lock(&mutex);
    users.push_back(u);
    current_user++;
    pthread_mutex_unlock(&mutex);
    update = true;
    cout << "A new user join.\n";
    return;
}

void Lobby_Manager::leave(User *u)
{
    pthread_mutex_lock(&mutex);
    auto it = find(users.begin(), users.end(), u);
    users.erase(it);
    current_user--;
    FD_CLR(u->fd, &allset);
    pthread_mutex_unlock(&mutex);
    cout << u->name << " leave the lobby. Left " << current_user << " users.\n";
    delete u;
    return;
}

void Lobby_Manager::updateListen()
{
    FD_ZERO(&allset);
    pthread_mutex_lock(&mutex);
    maxfdp1 = 0;
    for (auto u : users)
    {
        /*
        if (u->state == 2 || u->state == 3)
        {
            continue;
        }
        */
        FD_SET(u->fd, &allset);
        maxfdp1 = max(maxfdp1, u->fd + 1);
    }
    pthread_mutex_unlock(&mutex);
    update = false;
    cout << "Update listen fd.\n";
    return;
}