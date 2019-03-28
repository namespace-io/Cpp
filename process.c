#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
int main(){
    // int fd[2];
    // if(pipe(fd) == -1){
    //     printf("create pipe failed\n");
    //     return -1;
    // }

    char str[256];
    FILE* fp = fopen("./Cprimer.txt", "r");

    if(fp == NULL) return 0;
    fgets(str, sizeof(str), fp);
    str[strlen(str)] = '\0';

    printf("读取数据为: %s\n", str);
    fclose(fp);
    // fd[0]只读端, fd[1]只写端

    // if (write(fd[0], str, strlen(str) + 1) < 0)
    // {
    //     printf("write pipe failed: %s\n ", strerror(errno));
    // }


    
    return 0;
}