#include "mylib.hpp"
#include "User.hpp"
#include "RoomManager.hpp"

using namespace std;

void *lobby_run(void *ptr)
{
    // Note: this function should not be finished forever
    Lobby_Manager *lobby = (Lobby_Manager *)ptr;
    int rdnum, slen, op;
    char buf[4096];
    string cmd, attr;
    stringstream ss;
    struct timeval tv, ctv;
    tv.tv_sec = 0;
    tv.tv_usec = 30000; // 30ms per update
    fd_set rset;
    vector<User *> toDel;
    while (true)
    {
        if (lobby->update)
        {
            lobby->updateListen();
        }
        rset = lobby->allset;
        ctv = tv;
        rdnum = Select(lobby->maxfdp1, &rset, nullptr, nullptr, &ctv);
        if (rdnum == 0)
        {
            continue;
        }
        for (auto u : lobby->users)
        {
            if (FD_ISSET(u->fd, &rset))
            {
                rdnum--;
                slen = read(u->fd, buf, 4096);
                if (slen == 0)
                {
                    cout << "no cmd\n";
                    toDel.push_back(u);
                    continue;
                }
                buf[slen] = 0;
                cmd = buf;
                cout << "socket " << u->fd << " get cmd: " << cmd << "\n";
                ss.str(string());
                ss.clear();
                ss << cmd;
                while (true)
                {
                    ss >> op;
                    if (ss.fail())
                    {
                        break;
                    }
                    if (op == 999)
                    {
                        toDel.push_back(u);
                        break;
                    }
                    if (op == 1 && u->state == 1)
                    {
                        ss >> attr;
                        if (attr[0] == 'B')
                        {
                            u->state = 2;
                        }
                        else if (attr[0] == 'F')
                        {
                            u->state = 3;
                        }
                        continue;
                    }
                    if (op == 0 && u->state == 0)
                    {
                        ss >> attr;
                        u->name = attr;
                        u->state = 1;
                        // strcpy(buf, "ack");
                        // writen(u->fd, buf, strlen(buf));
                        cout << "socket " << u->fd << " set name: " << u->name << "\n";
                        continue;
                    }
                }
            }
            if (u->state == 0 || u->state == 1)
            {
                continue;
            }
            else if (u->state == 2)
            {
                cout << u->name << " join BJ. \n";
                u->state = 1;
                // TODO, BJ
            }
            else if (u->state == 3)
            {
                cout << u->name << " join CF. \n";
                u->state = 1;
                // TODO, CF
            }
        }
        while (!toDel.empty())
        {
            User *u = *toDel.rbegin();
            lobby->leave(u);
            toDel.pop_back();
        }
    }
    return nullptr;
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    socklen_t servlen = sizeof(servaddr);
    pthread_t lobby_t;
    Lobby_Manager *lobby = new Lobby_Manager();

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, servlen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(14514);

    Bind(listenfd, (SA *)&servaddr, servlen);
    Listen(listenfd, 100);

    int th_state = pthread_create(&lobby_t, NULL, lobby_run, (void *)lobby);
    if (th_state != 0)
    {
        cout << "Thread create failed\n";
        exit(0);
    }
    pthread_detach(lobby_t);

    cout << "Listening...\n";

    while ((connfd = Accept(listenfd, nullptr, 0)) > 0)
    {
        User *newuser = new User(connfd);
        lobby->join(newuser);
    }
    exit(0);
    return 0;
}