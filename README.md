#nsq-c  
nsq client for c language  ,c 语言的nsq客户端

#### build
1. cmake .
2. make 

Or :


sub : gcc  example_sub.c sub_client.c -o  sub_client -levent

pub : gcc  example_pub.c pub_client.c -o pub_client


#### Quick Start :

###### example for pub:

```

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


```


###### example for sub: 
```
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
    subscribe("127.0.0.1", "4150", msg, &msg_callback_m); //现在只是 直连nsqd 的地址,lookupd地址支持 以后上
    free(msg);

}

```

###### Dependencies:

libevent


###### TODO List:

1 根据lookupd 链接nsqd

The discover that Find nsqd according to lookupd 

2 多个topic连接支持

Multiple topic connection


