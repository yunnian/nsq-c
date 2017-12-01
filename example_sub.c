#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "sub_client.h"

int msg_callback_m(struct NSQMessage *msg){

    printf("test message handler:%s\n", msg->body);
    return 0;
}


int main()
{
    int sock;
connect:
    sock =  connect_nsqd_with_lookupd("127.0.0.1", "4150"); //现在只是 直连nsqd 的地址,lookupd地址支持 以后上
    printf("sock:%d",sock);
    if(!sock){
        goto connect;
        
    }
    struct NSQMessage *msg;
    msg = (struct NSQMessage *)malloc(sizeof(struct NSQMessage));
    msg->topic = "test";
    msg->channel = "struggle";
    msg->rdy = 2;
    int (*msg_callback)(struct NSQMessage *msg) = msg_callback_m;
    int re = subscribe(sock, msg, msg_callback);
    printf("re:%d",re);
    if(!re){
        goto connect;
    }
    free(msg);

}

