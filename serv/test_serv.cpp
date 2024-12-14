#include "mylib.hpp"

using namespace std;

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t servlen = sizeof(servaddr);
    char id[15], sendbuff[4096];
    string todo;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, servlen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(14514);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    strcpy(id, argv[2]);
    Connect(sockfd, (SA *)&servaddr, servlen);

    snprintf(sendbuff, 4096, "0 %s", id);
    writen(sockfd, sendbuff, strlen(sendbuff));
    while (getline(cin, todo))
    {
        cout << "echo " << todo << "\n";
        const char *buff = todo.c_str();
        writen(sockfd, buff, strlen(buff));
    }
    cout << "close\n";
    close(sockfd);
    return 0;
}