#include "mylib.hpp"
#include "StringBuff.hpp"

using namespace std;

int main(int argc, char **argv)
{
    int sockfd, maxfdp1, n, rdnum;
    struct sockaddr_in servaddr;
    struct timeval tv, ctv;
    tv.tv_sec = 0;
    tv.tv_usec = 30000; // 30ms per update
    socklen_t servlen = sizeof(servaddr);
    char id[15], buff[4096];
    fd_set allset, rset;
    StringBuff myBuff;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, servlen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(14514);
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    strcpy(id, argv[2]);
    Connect(sockfd, (SA *)&servaddr, servlen);

    snprintf(buff, 4096, "0 %s\n", id);
    writen(sockfd, buff, strlen(buff));
    maxfdp1 = sockfd + 1;
    FD_SET(0, &allset);
    FD_SET(sockfd, &allset);
    while (true)
    {
        rset = allset;
        ctv = tv;
        rdnum = Select(maxfdp1, &rset, nullptr, nullptr, &ctv);
        if (rdnum)
        {
            if (FD_ISSET(0, &rset))
            {
                n = read(0, buff, 4096);
                if (n == 0)
                {
                    break;
                }
                buff[n] = '\0';
                myBuff.write(0, buff);
            }
            if (FD_ISSET(sockfd, &rset))
            {
                n = read(sockfd, buff, 4096);
                buff[n] = '\0';
                myBuff.write(sockfd, buff);
            }
        }
        string cmd;
        if ((cmd = myBuff.read(0)) != "")
        {
            cout << "[ECHO] " << cmd;
            strcpy(buff, cmd.c_str());
            writen(sockfd, buff, strlen(buff));
        }
        if ((cmd = myBuff.read(sockfd)) != "")
        {
            cout << "[SERV] " << cmd;
        }
    }
    cout << "close\n";
    close(sockfd);
    return 0;
}