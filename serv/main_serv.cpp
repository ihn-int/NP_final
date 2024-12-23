#include "mylib.hpp"
#include "User.hpp"
#include "RoomManager.hpp"

using namespace std;

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

    cout << "[LOBBY] Listening...\n";

    while ((connfd = Accept(listenfd, nullptr, 0)) > 0)
    {
        User *newuser = new User(connfd);
        lobby->join(newuser);
    }
    exit(0);
    return 0;
}