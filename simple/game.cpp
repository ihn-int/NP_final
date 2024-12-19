#include "Scene.hpp"
#include "UserScene.hpp"
extern "C" {
    #include "unp.h"
}
#define USAGE "usage: ./game <IPaddress> <UserId>\n"



int main(int argc, char** argv) {
    //============================================
    // Socket related
    // Check parameters
    if (argc < 3) {
        std::printf(USAGE);
        ErrorHandler::printError(INVALID_USAGE);
    }
    std::printf("User %s is connecting to server: %s ...\n",
        argv[2], argv[1]
    );

    // Declare socket variable
    int sockfd;
    struct sockaddr_in servaddr;

    // Create Socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(14514);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    // Connect Socket
    // Connect(sockfd, (SA*)&servaddr, sizeof(servaddr));
    std::printf("Socket is created, with fd be %i.\n", sockfd);
    
    // end of Socket related
    //============================================

    //============================================
    // Send  User Id
    std::printf("Send user id: %s to server...\n", argv[2]);
    std::string sendline = "0 " + std::string(argv[2]);
    // writen(sockfd, sendline.c_str(), sendline.length());
    std::printf("[SOCK]: send: \n\"%s\"\n", sendline.c_str());
    // end of send user id
    //============================================

    //============================================
    // Scene Manager
    SceneManager sceneManager(std::string(argv[2]), "Test",
        800, 600, sockfd);

    // Start Scene
    StartScene startScene("Start");
    sceneManager.addScene(&startScene);

    // BlackJack Scene
    // BJScene bjScene("Blackjack");
    // sceneManager.addScene(%bjScene);

    // Set Start Scene
    sceneManager.changeScene("Start");

    // Set BlackJack Scene

    /** /
    ChatScene chatScene("Chat");
    sceneManager.addScene(&chatScene);
    sceneManager.changeScene("Chat");
    /**/
    // Game Run
    sceneManager.runScene();

    return 0;
}