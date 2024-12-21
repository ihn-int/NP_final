extern "C" {
    #include "unp.h"
}

#include <iostream>
#include <cstring>
#include <string>

void runConnect(int sockfd) {
    std::string msg;
    char sendline[MAXLINE], recvline[MAXLINE];
    int maxfdp1 = std::max(fileno(stdin), sockfd) + 1;
    struct timeval timeout;
    fd_set rset;
    while(true) {
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        FD_ZERO(&rset);
        FD_SET(fileno(stdin), &rset);
        FD_SET(sockfd, &rset);
        if (select(maxfdp1, &rset, NULL, NULL, &timeout)) {
            if (FD_ISSET(sockfd, &rset)) {
                int n = readline(sockfd, recvline, MAXLINE);
                if (n == 0) return;
                if (std::strncmp(recvline, "999", 3) == 0) return;
                std::printf("[RECV] %s\n", recvline);
            }
            if (FD_ISSET(fileno(stdin), &rset)) {
                Fgets(sendline, MAXLINE, stdin);
                std::printf("[SEND] %s\n", sendline);
                writen(sockfd, sendline, strlen(sendline));
            }
        }
    }
}

int main() {
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;
    char sendline[MAXLINE];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(14514);

    bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

    for (;;) {
        if ((connfd = accept(listenfd, (SA*)&cliaddr, &clilen)) < 0){
            if (errno == EINTR) continue;
            else std::cout << "Error" << std::endl;
            
        }
        std::cout << "[SERV] get user " << connfd << std::endl;
        runConnect(connfd);
    }
}