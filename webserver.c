#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

void error_die(const char *s){
    perror(s);
    exit(1);
}


int startup(u_short* port){

    struct sockaddr_in name;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    // 获取一个socket描述符并与主机绑定    

    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port); // 设置端口号
    name.sin_addr.s_addr = htons(INADDR_ANY); // 准备绑定IP地址为主机地址
    if(listenfd  == -1) error_die("Start Up");
    
    if(bind(listenfd, (struct sockaddr*)&name, sizeof(name)) < 0) error_die("Bind"); // 将fd与socket绑定起来

    if(*port == 0){
        socklen_t len =  sizeof(name);
        if(getsockname(listenfd, (struct sockaddr*)&name, &len) == -1)
            error_die("listen");
        *port = name.sin_port;
    }

    if(listen(listenfd, 5) < 0)
        error_die("listen");
    
    return listenfd;
}


int get_line(int sock, char* buf, int size){
    int i = 0;
    char c = '\0';
    int n ;
    while(i < size || c == '\n'){
        n = recv(sock, &c, 1, 0);

        if(n > 0){
             if(c == '\r'){
                 n = recv(sock, &c, 1, MSG_PEEK);
                 if(n > 0 && c == '\n')
                    recv(sock, &c, 1, MSG_PEEK);
                 else c ='\n';
             }

             buf[i] = c;
             ++i;
             
        } else c = '\n';
    }

    buf[i]='\0';
    return i;
}
void accept_request(int client_sock){
    char c = 'c';
    get_line(client_sock, &c, 1);
    printf("Char is %c \n", c);
    send(client_sock, &c, 1, 0);
}

int main(){

    u_short port = 8080;
    // 准备开启服务器8080端口
    int listenfd = startup(&port);
    printf("Start Up\n");
    char c = 'R';
    struct sockaddr_in clientInfo;
    int client_sock = -1;
    socklen_t len = sizeof(clientInfo);
    while(1){
        client_sock = accept(listenfd, (struct sockaddr*)&clientInfo, &len); 
        //从listenfd接受请求，并获取请求方的socket, 将客户端信息存在clientInfo中，返回一个新socket,使用新socket与client交互
        if(client_sock == -1) error_die("accept");
        accept_request(client_sock);
    }

    close(listenfd);
    return 0;
}