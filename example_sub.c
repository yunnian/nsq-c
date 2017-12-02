#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "sub_client.h"

int msg_callback_m(NSQMsg *msg){

    printf("test message handler:%s\n", msg->body);
    return 0;
}


int main()
{

    struct NSQMsg *msg;
    msg = malloc(sizeof(NSQMsg));
    msg->topic = "test";
    msg->channel = "struggle_everyday";
    msg->rdy = 2;
    //int (*msg_callback)(struct NSQMsg *msg) = msg_callback_m;

    subscribe("127.0.0.1", "4150", msg, &msg_callback_m); //现在只是 直连nsqd 的地址,lookupd地址支持 以后上
    //int re = subscribe(sock, msg, msg_callback);
    //printf("re:%d",re);
    free(msg);

}

