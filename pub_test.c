
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/socket.h>
#include "pub_client.h"


int main()
{
    int sock;
    sock = connect_nsqd("127.0.0.1","4150");
    for (int i =0; i<=100; i++){
        int result = publish(sock,"test","stuggle 加油"); //0 is sucess , -1 fail
        if(result == 0){
            printf("%s\n", "sucess");
        }
    }
	close(sock);

}

