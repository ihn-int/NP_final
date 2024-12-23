#include "mylib.hpp"
#include "RoomManager.hpp"

using namespace std;

Lobby_Manager::Lobby_Manager()
{
    mutex = new pthread_mutex_t();
    pthread_mutex_init(mutex, 0);
    users.clear();
    FD_ZERO(&allset);
    cout << "[LOBBY] Create lobby\n";
}
Lobby_Manager::~Lobby_Manager()
{
    for (auto u : users)
    {
        delete u;
    }
    pthread_mutex_destroy(mutex);
}
void Lobby_Manager::join(User *u)
{
    pthread_mutex_lock(mutex);
    users.push_back(u);
    current_user++;
    pthread_mutex_unlock(mutex);
    update = true;
    cout << "[LOBBY] new user join.\n";
    return;
}
void Lobby_Manager::leave(User *u)
{
    pthread_mutex_lock(mutex);
    auto it = find(users.begin(), users.end(), u);
    if (it == users.end())
    {
        cout << "[LOBBY] vector erase error.\n";
    }
    users.erase(it);
    current_user--;
    FD_CLR(u->fd, &allset);
    pthread_mutex_unlock(mutex);
    cout << "[LOBBY] " << u->name << " leave the lobby. Left " << current_user << " users.\n";
    delete u;
    return;
}
void Lobby_Manager::updateListen()
{
    if (update)
    {
        FD_ZERO(&allset);
        pthread_mutex_lock(mutex);
        maxfdp1 = 0;
        for (auto u : users)
        {
            if (u->state == 2 || u->state == 3)
            {
                continue;
            }
            FD_SET(u->fd, &allset);
            maxfdp1 = max(maxfdp1, u->fd + 1);
        }
        pthread_mutex_unlock(mutex);
        update = false;
        cout << "[LOBBY] Update listen fd.\n";
    }
    return;
}
int Lobby_Manager::msg_wait()
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 30000; // 30ms per update
    rset = allset;
    int rdnum = Select(maxfdp1, &rset, nullptr, nullptr, &tv);
    return rdnum;
}
void Lobby_Manager::msg_handle(int &rdnum)
{
    int slen, op;
    char buff[4096];
    string cmd, attr;
    stringstream ss;
    vector<User *> toDel;
    for (auto u : users)
    {
        if (FD_ISSET(u->fd, &rset))
        {
            cmd = "";
            rdnum--;
            slen = readline(u->fd, buff, 4096);
            if (slen == 0)
            {
                cout << "[SOCKET " << u->fd << "] no cmd\n";
                toDel.push_back(u);
                continue;
            }
            buff[slen] = 0;
            cmd = buff;
            cout << "[SOCKET " << u->fd << "] command: " << cmd;
            ss.str(string());
            ss.clear();
            ss << cmd;
            ss >> op;
            switch (op)
            {
            case 999:
                toDel.push_back(u);
                break;

            case 0:
                if (u->state != 0)
                {
                    break;
                }
                ss >> attr;
                u->name = attr;
                u->state = 1;
                cout << "[SOCKET " << u->fd << "] name: " << u->name << "\n";
                break;

            case 1:
                if (u->state != 1)
                {
                    break;
                }
                ss >> attr;
                if (attr[0] == 'B')
                {
                    cout << "[" << u->name << "] join BJ.\n";
                    u->state = 1;
                    // TODO
                }
                else if (attr[0] == 'F')
                {
                    cout << "[" << u->name << "] join CF.\n";
                    u->state = 3;
                    CF_queue.push(u);
                }
                break;
            }
        }
    }
    while (!toDel.empty())
    {
        User *u = *toDel.rbegin();
        leave(u);
        toDel.pop_back();
    }
    return;
}
void Lobby_Manager::CF_handle()
{
    while (!CF_queue.empty())
    {
        User *u = CF_queue.front();
        CF_queue.pop();
        bool suc = false;
        for (auto it = CF_room.begin(); it != CF_room.end();)
        {
            CF_RoomManager *room = (CF_RoomManager *)*it;
            pthread_mutex_t *tmplock = room->mutex;
            pthread_mutex_lock(tmplock);
            if (room->die)
            {
                cout << "[CF] kill die room.\n";
                delete room;
                pthread_mutex_unlock(tmplock);
                pthread_mutex_destroy(tmplock);
                it = CF_room.erase(it);
                continue;
            }
            if (room->game_state == 0)
            {
                room->join(u);
                pthread_mutex_unlock(tmplock);
                suc = true;
                break;
            }
            pthread_mutex_unlock(tmplock);
            it = next(it);
        }
        if (!suc)
        {
            CF_RoomManager *newroom = new CF_RoomManager(this);
            pthread_t cf_t;
            CF_room.push_back((void *)newroom);
            newroom->join(u);
            pthread_create(&cf_t, nullptr, CF_run, (void *)newroom);
            pthread_detach(cf_t);
        }
    }
    return;
}

void *lobby_run(void *ptr)
{
    // Note: this function should not be finished forever
    Lobby_Manager *lobby = (Lobby_Manager *)ptr;
    int rdnum;
    while (true)
    {
        lobby->updateListen();
        rdnum = lobby->msg_wait();
        if (rdnum == 0)
        {
            continue;
        }
        lobby->msg_handle(rdnum);
        lobby->CF_handle();
    }
    return nullptr;
}

CF_RoomManager::CF_RoomManager(Lobby_Manager *lobby)
{
    cout << "[CF] new room\n";
    for (int i = 0; i <= 1; i++)
    {
        players[i] = nullptr;
    }
    for (int col = 0; col <= 6; col++)
    {
        for (int row = 0; row <= 5; row++)
        {
            board[col][row] = -1;
        }
    }
    mutex = new pthread_mutex_t();
    pthread_mutex_init(mutex, 0);
    current_round = 0;
    game_state = 0;
    current_user = 0;
    ready_player = 0;
    maxfdp1 = 0;
    die = false;
    this->lobby = lobby;
    FD_ZERO(&allset);
}
CF_RoomManager::~CF_RoomManager()
{
    for (int i = 0; i <= 1; i++)
    {
        if (players[i] == nullptr)
        {
            continue;
        }
        User *u = players[i]->original_user;
        u->pt = players[i]->pt;
        u->state = 1;
        if (game_state == 1 && players[i]->state == 0)
        {
            send("-2 0\n", i);
        }
        else if (game_state == 1 && players[i]->state == 1)
        {
            send("-2 1\n", i);
        }
        delete players[i];
    }
    lobby->update = 1;
}
void CF_RoomManager::join(User *u)
{
    int i = 0;
    cout << "[CF] join: " << u->name << "\n";
    CF_Player *p = new CF_Player(u);
    if (players[0] == nullptr)
    {
        i = 0;
    }
    else
    {
        i = 1;
    }
    players[i] = p;
    lobby->update = 1;
    update = 1;
}
void CF_RoomManager::leave(int n, bool q = false)
{
    cout << "[CF] leave: " << players[n]->name << "\n";
    if (q)
    {
        lobby->leave(players[n]->original_user);
    }
    else
    {
        send("-2 0\n", n);
        players[n]->original_user->state = 1;
    }
    delete players[n];
    players[n] = nullptr;
    cout << "[CF] delete player\n";
    lobby->update = 1;
    update = 1;
}
void CF_RoomManager::send(string s, int n)
{
    char buff[4096];
    strcpy(buff, s.c_str());
    printf("[CF SEND %d] %s", players[n]->fd, buff);
    writen(players[n]->fd, buff, strlen(buff));
}
void CF_RoomManager::broadcast(string s, int n = -1)
{
    char buff[4096];
    strcpy(buff, s.c_str());
    if (n == -1)
    {
        printf("[CF BROADCAST] %s", buff);
    }
    else
    {
        printf("[CF BROADCAST WITHOUT %d] %s", players[n]->fd, buff);
    }
    for (int i = 0; i <= 1; i++)
    {
        if (i == n || players[i] == nullptr)
        {
            continue;
        }
        writen(players[i]->fd, buff, strlen(buff));
    }
}
void CF_RoomManager::updateMember()
{
    pthread_mutex_lock(mutex);
    if (!update)
    {
        pthread_mutex_unlock(mutex);
    }
    else
    {
        maxfdp1 = 0;
        current_user = 0;
        FD_ZERO(&allset);
        if (players[0] != nullptr)
        {
            maxfdp1 = max(maxfdp1, players[0]->fd + 1);
            current_user++;
            FD_SET(players[0]->fd, &allset);
        }
        if (players[1] != nullptr)
        {
            maxfdp1 = max(maxfdp1, players[1]->fd + 1);
            current_user++;
            FD_SET(players[1]->fd, &allset);
        }
        pthread_mutex_unlock(mutex);
        update = false;
        if (game_state == 0 || game_state == 1)
        {
            switch (current_user)
            {
            case 0:
                die = true;
                break;

            case 1:
                timer.stop();
                if (game_state == 1)
                {
                    broadcast("-2 1\n101 [INFO] someone quit, back to matching.\n");
                }
                game_state = 0;
                ready_player = 0;
                broadcast("550 M\n555 -1\n", -1);
                if (players[0] != nullptr)
                {
                    players[0]->state = 0;
                    broadcast("3 0 " + players[0]->name + "\n", -1);
                }
                if (players[1] != nullptr)
                {
                    players[1]->state = 0;
                    broadcast("3 1 " + players[1]->name + "\n");
                }
                break;

            case 2:
                timer.start();
                game_state = 1;
                broadcast("550 W\n555 15\n");
                broadcast("3 0 " + players[0]->name + "\n" + "3 1 " + players[1]->name + "\n");
                break;
            }
        }
        cout << "[CF] Update.\n";
    }
    return;
}
bool CF_RoomManager::msg_wait()
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 30000; // 30ms per update
    rset = allset;
    int rdnum = Select(maxfdp1, &rset, nullptr, nullptr, &tv);
    if (rdnum == 0)
    {
        return false;
    }
    return true;
}
void CF_RoomManager::msg_handle()
{
    int slen, op, toLeave[2] = {0, 0};
    char buff[4096];
    string attr, cmd;
    stringstream ss;
    for (int i = 0; i <= 1; i++)
    {
        if (players[i] == nullptr)
        {
            continue;
        }
        if (FD_ISSET(players[i]->fd, &rset))
        {
            slen = readline(players[i]->fd, buff, 4096);
            if (slen == 0)
            {
                cout << "[SOCK " << players[i]->fd << "] no command\n";
                toLeave[i] = 1;
                continue;
            }
            buff[slen] = 0;
            cmd = buff;
            cout << "[CF SOCK " << players[i]->fd << "] command: " << cmd;
            ss.str(string());
            ss.clear();
            ss << cmd;
            ss >> op;
            switch (game_state)
            {
            case 0:
                if (op == -1)
                {
                    toLeave[i] = 2;
                }
                else if (op == 999)
                {
                    toLeave[i] = 1;
                }
                else
                {
                    send("101 [INFO] illegal command\n", i);
                }
                break;

            case 1:
                if (op == 999)
                {
                    toLeave[i] = 1;
                }
                else if (op == 2)
                {
                    cout << players[i]->name << " ready\n";
                    players[i]->state = 1;
                    ready_player++;
                    broadcast("4 " + to_string(i) + "\n");
                }
                else
                {
                    send("101 [INFO] illegal command\n", i);
                }
                break;

            default:
                break;
            }
        }
    }
    for (int i = 0; i <= 1; i++)
    {
        if (toLeave[i] == 0)
        {
            continue;
        }
        else if (toLeave[i] == 1)
        {
            leave(i, true);
        }
        else if (toLeave[i] == 2)
        {
            leave(i, false);
        }
    }
    return;
}
void CF_RoomManager::timeout()
{
    switch (game_state)
    {
    case 1:
        for (int i = 0; i <= 1; i++)
        {
            if (players[i] == nullptr)
            {
                continue;
            }
            if (players[i]->state == 0)
            {
                pthread_mutex_lock(mutex);
                leave(i);
                pthread_mutex_unlock(mutex);
            }
        }
        break;

    default:
        break;
    }
}

void *CF_run(void *p)
{
    CF_RoomManager *manager = (CF_RoomManager *)p;
    long long t;
    string cmd, attr;
    stringstream ss;

    // matching / waiting state
    while (manager->game_state <= 1)
    {
        manager->updateMember();
        if (manager->die)
        {
            return nullptr;
        }
        if (manager->msg_wait())
        {
            manager->msg_handle();
        }
        if (manager->ready_player == 2)
        {
            manager->game_state = 2;
            break;
        }
        if (manager->timer.get_end(15))
        {
            manager->timeout();
        }
        if ((t = manager->timer.get_second()) >= 0)
        {
            string ts = to_string(15 - t);
            manager->broadcast("555 " + ts + "\n");
        }
    }
    cout << "[CF] gaming!\n";
    manager->leave(0, false);
    manager->leave(1, false);
    manager->die = true;
    return nullptr;
}