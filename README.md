# struggle_nsq_c
nsq client for c language

#### build
1. cmake .
2. make 

Or :

sub : gcc test.c sub_client.c -o test

pub : gcc -g  pub_test.c pub_client.c -o pub


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

int msg_callback_m(struct NSQMessage *msg){

    printf("test message handler:%s\n", msg->body);
    return 0;
}


int main()
{
    int sock;
    sock =  connect_nsqd_with_lookupd("127.0.0.1", "4150");
    struct NSQMessage *msg;
    msg = (struct NSQMessage *)malloc(sizeof(struct NSQMessage));
    msg->topic = "test";
    msg->channel = "struggle";
    msg->rdy = 2;
    int (*msg_callback)(struct NSQMessage *msg) = msg_callback_m;
    subscribe(sock, msg, msg_callback);
    free(msg);

}


```
