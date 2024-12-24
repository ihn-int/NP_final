#include "mylib.hpp"
#include "RoomManager.hpp"

using namespace std;

Lobby_Manager::Lobby_Manager()
{
    fdBuff = new StringBuff();
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
            if (room->game_state == 0 && room->current_user + room->joinNum <= 1)
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
    srand(time(NULL));
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
    for (int col = 0; col < 8; col++)
    {
        for (int row = 0; row < 7; row++)
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
    joinNum = 0;
    maxfdp1 = 0;
    die = false;
    fin = false;
    nextRound = false;
    surr = false;
    update = false;
    updateListen = false;
    this->lobby = lobby;
    fdBuff = lobby->fdBuff;
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
        leave(i, false);
    }
    lobby->update = 1;
    fdBuff = nullptr;
}
bool CF_RoomManager::checkWin(int r, int c)
{
    // find top most
    int tr, tc, cr, cc, counter;
    int dr[4] = {1, 1, 0, -1}, dc[4] = {0, 1, 1, 1};
    for (int d = 0; d <= 3; d++)
    {
        tr = r, tc = c, cr = r + dr[d], cc = c + dc[d];
        while (cr >= 1 && cr <= 6 && cc >= 1 && cc <= 7)
        {
            if (board[cc][cr] != current_round)
            {
                break;
            }
            tr = cr, tc = cc;
            cr += dr[d], cc += dc[d];
        }
        counter = 1;
        cr = tr - dr[d], cc = tc - dc[d];
        while (cr >= 1 && cr <= 6 && cc >= 1 && cc <= 7)
        {
            if (board[cc][cr] != current_round)
            {
                break;
            }
            counter++;
            cr -= dr[d], cc -= dc[d];
        }
        if (counter >= 4)
        {
            return true;
        }
    }
    return false;
}
void CF_RoomManager::join(User *u)
{
    cout << "[CF] join: " << u->name << "\n";
    CF_Player *p = new CF_Player(u);
    joinQueue.push(p);
    joinNum++;
    lobby->update = true;
    update = true;
    updateListen = true;
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
    fdBuff->clear(players[n]->fd);
    delete players[n];
    current_user--;
    players[n] = nullptr;
    lobby->update = true;
    update = true;
    updateListen = true;
}
void CF_RoomManager::send(string s, int n)
{
    char buff[4096];
    if (players[n] == nullptr)
    {
        cout << "[CF] send fail\n";
        return;
    }
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
        // handle join queue
        if (joinNum > 0)
        {
            while (!joinQueue.empty())
            {
                CF_Player *p = joinQueue.front();
                joinQueue.pop();
                if (players[0] == nullptr)
                {
                    players[0] = p;
                }
                else if (players[1] == nullptr)
                {
                    players[1] = p;
                }
            }
            joinNum = 0;
        }

        // update listening
        if (updateListen)
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
        }
        pthread_mutex_unlock(mutex);

        // update state
        if (game_state == 1 && ready_player == 2)
        {
            current_round = rand() % 2;
            send("550 Y\n555 20\n102 0011\n101 [INFO] Your turn.\n", current_round);
            send("550 C\n555 20\n102 0001\n", 1 - current_round);
            timer.restart();
            game_state = 2;
        }
        else if (updateListen && (game_state == 0 || game_state == 1))
        {
            int tmp = 0;
            switch (current_user)
            {
            case 0:
                die = true;
                break;

            case 1:
                timer.stop();
                game_state = 0;
                ready_player = 0;
                if (players[0] != nullptr)
                {
                    tmp = 0;
                }
                else if (players[1] != nullptr)
                {
                    tmp = 1;
                }
                players[tmp]->state = 0;
                send("102 0100\n550 M\n555 -1\n101 [INFO] Player change, go to matching state.\n3 " + to_string(tmp) + " " + players[tmp]->name + "\n17 " + to_string(players[tmp]->pt) + "\n", tmp);
                break;

            case 2:
                broadcast("102 1000\n550 W\n555 15\n101 [INFO] Two players join, please ready in 15 secs.\n");
                broadcast("3 0 " + players[0]->name + "\n" + "3 1 " + players[1]->name + "\n");
                send("17 " + to_string(players[0]->pt) + "\n", 0);
                send("17 " + to_string(players[1]->pt) + "\n", 1);
                timer.restart();
                game_state = 1;
                break;
            }
        }
        else if (updateListen && game_state == 2)
        {
            switch (current_user)
            {
            case 0:
                die = true;
                break;

            case 1:
                int w;
                if (players[0] != nullptr)
                {
                    w = 0;
                }
                if (players[1] != nullptr)
                {
                    w = 1;
                }
                players[w]->pt += 1000;
                send("18 W\n17 " + to_string(players[w]->pt) + "\n550 F\n555 5\n102 0100\n", w);
                send("101 [INFO] A player quit this game. You win! +1000 pt\n101 [INFO] The game is finish, you will return to the lobby in 5 secs. Bye~~\n", w);
                timer.restart();
                game_state = 3;
                break;

            default:
                break;
            }
        }
        else if (game_state == 2)
        {
            if (fin)
            {
                broadcast("101 [INFO] " + players[current_round]->name + " CONNECT FOUR!!!\n102 0100\n");
                players[current_round]->pt += 1000;
                send("18 W\n17 " + to_string(players[current_round]->pt) + "\n550 F\n555 5\n", current_round);
                send("101 [INFO] You win! +1000 pt\n101 [INFO] The game is finish, you will return to the lobby in 5 secs. Bye~~\n", current_round);
                send("18 L\n550 F\n555 5\n", 1 - current_round);
                send("101 [INFO] You Lose...\n101 [INFO] The game is finish, you will return to the lobby in 5 secs. Bye~~\n", 1 - current_round);
                timer.restart();
                game_state = 3;
            }
            else if (surr)
            {
                broadcast("101 [INFO] " + players[current_round]->name + " surrend...\n102 0100\n");
                players[1 - current_round]->pt += 1000;
                send("18 W\n17 " + to_string(players[1 - current_round]->pt) + "\n550 F\n555 5\n", 1 - current_round);
                send("101 [INFO] You win! +1000 pt\n101 [INFO] The game is finish, you will return to the lobby in 5 secs. Bye~~\n", 1 - current_round);
                send("18 L\n550 F\n555 5\n", current_round);
                send("101 [INFO] You Lose...\n101 [INFO] The game is finish, you will return to the lobby in 5 secs. Bye~~\n", current_round);
                timer.restart();
                game_state = 3;
            }
            else if (nextRound)
            {
                nextRound = false;
                current_round = 1 - current_round;
                send("550 Y\n555 20\n102 0011\n101 [INFO] Your turn.\n", current_round);
                send("550 C\n555 20\n102 0000\n", 1 - current_round);
                timer.restart();
            }
        }
        else if (game_state == 3 && current_user == 0)
        {
            die = true;
        }

        update = false;
        updateListen = false;
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
    int slen, op;
    char buff[4096];
    string attr, cmd;
    stringstream ss;
    for (int i = 0; i <= 1; i++)
    {
        if (players[i] == nullptr)
        {
            continue;
        }

        // store into buffer
        if (FD_ISSET(players[i]->fd, &rset))
        {
            slen = readline(players[i]->fd, buff, 4096);
            // ctrl + C
            if (slen == 0)
            {
                cout << "[SOCK " << players[i]->fd << "] no command\n";
                leave(i, true);
                continue;
            }
            buff[slen] = 0;
            fdBuff->write(players[i]->fd, buff);
        }

        // read a line from buffer
        if ((cmd = fdBuff->read(players[i]->fd)) != "")
        {
            cout << "[CF SOCK " << players[i]->fd << "] command: " << cmd;
            ss.str(string());
            ss.clear();
            ss << cmd;
            ss >> op;

            // read fail
            if (ss.fail() || ss.eof())
            {
                send("101 [INFO] illegal command\n", i);
                continue;
            }
            switch (game_state)
            {
            case 0:
                if (op == -1)
                {
                    leave(i, false);
                }
                else if (op == 999)
                {
                    leave(i, true);
                }
                else if (op == 100)
                {
                    getline(ss, attr);
                    attr += "\n";
                    broadcast("101" + attr);
                }
                else
                {
                    send("101 [INFO] illegal command\n", i);
                }
                break;

            case 1:
                if (op == 999)
                {
                    leave(i, true);
                }
                else if (op == 2 && players[i]->state == 0)
                {
                    players[i]->state = 1;
                    broadcast("4 " + to_string(i) + "\n");
                    send("102 0000\n", i);
                    ready_player++;
                    update = true;
                }
                else if (op == 100)
                {
                    getline(ss, attr);
                    attr += "\n";
                    broadcast("101" + attr);
                }
                else
                {
                    send("101 [INFO] illegal command\n", i);
                }
                break;

            case 2:
                if (op == 999)
                {
                    leave(i, true);
                }
                else if (op == 100)
                {
                    getline(ss, attr);
                    attr += "\n";
                    broadcast("101" + attr);
                }
                else if (op == 13 && current_round == i)
                {
                    int col, row;
                    ss >> col;
                    if (ss.fail() || ss.eof() || col <= 0 || col > 7)
                    {
                        row = 0;
                    }
                    else
                    {
                        for (row = 6; row >= 1; row--)
                        {
                            if (board[col][row] != -1)
                            {
                                continue;
                            }
                            board[col][row] = current_round;
                            send("15 0 " + to_string(row) + to_string(col) + "\n", i);
                            send("15 1 " + to_string(row) + to_string(col) + "\n", 1 - i);
                            if (checkWin(row, col))
                            {
                                fin = true;
                            }
                            else
                            {
                                nextRound = true;
                            }
                            update = true;
                            break;
                        }
                    }
                    if (row == 0)
                    {
                        send("101 [INFO] illegal column. Please try again.\n", i);
                    }
                }
                else if (op == 14 && current_round == i)
                {
                    surr = true;
                    update = true;
                }
                else
                {
                    send("101 [INFO] illegal command.\n", i);
                }
                break;

            case 3:
                if (op == -1)
                {
                    leave(i, false);
                }
                if (op == 999)
                {
                    leave(i, true);
                }
                else if (op == 100)
                {
                    getline(ss, attr);
                    attr += "\n";
                    broadcast("101" + attr);
                }

            default:
                break;
            }
        }
    }
    return;
}
void CF_RoomManager::time_handle()
{
    int TO;
    long long t;
    switch (game_state)
    {
    case 1:
        TO = 15;
        break;

    case 2:
        TO = 20;
        break;

    case 3:
        TO = 5;
        break;

    default:
        TO = -1;
        break;
    }
    if (timer.get_end(TO))
    {
        timeout();
    }
    if ((t = timer.get_second()) >= 0)
    {
        string ts = to_string(TO - t);
        broadcast("555 " + ts + "\n");
    }
}
void CF_RoomManager::timeout()
{
    switch (game_state)
    {
    case 1:
        for (int i = 0; i <= 1; i++)
        {
            if (players[i] == nullptr || players[i]->state == 1)
            {
                continue;
            }
            leave(i, false);
        }
        break;

    case 2:
        send("101 [INFO] Timeout! auto surrend.\n", current_round);
        surr = true;
        update = true;
        break;

    case 3:
        for (int i = 0; i <= 1; i++)
        {
            if (players[i] == nullptr)
            {
                continue;
            }
            leave(i, false);
        }
        break;

    default:
        break;
    }
}

void *CF_run(void *p)
{
    srand(time(NULL));
    CF_RoomManager *manager = (CF_RoomManager *)p;
    string cmd, attr;
    stringstream ss;

    while (true)
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
        manager->time_handle();
    }
    return nullptr;
}