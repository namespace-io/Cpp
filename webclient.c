#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>

int main(){
    int sockfd;
    struct sockaddr_in serverInfo;
    char c = 'K';
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverInfo.sin_port = htons(8080);

    if(connect(sockfd, (struct sockaddr*)&serverInfo, sizeof(serverInfo)) == -1){
        perror("Oops: Client");
        exit(1);
    }

    write(sockfd, &c, 1);

    read(sockfd, &c, 1);
    printf("char from server is %c\n", c);

    close(sockfd);
    
    return 0;
}