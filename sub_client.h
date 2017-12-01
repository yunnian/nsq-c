//
// Created by Zhenyu Wu on 2017/9/13.
//

#ifndef STRUGGLE_NSQ_SUB_CLIENT_H
#define STRUGGLE_NSQ_SUB_CLIENT_H

struct NSQMessage {
    const char * topic;
    const char * channel;
    int32_t frame_type;
    int64_t timestamp;
    uint16_t attempts;
    char *message_id;
    int32_t size;
    char *body;
    int rdy;
    int (*callback)(struct NSQMessage *msg);
};
struct NSQDConnection {
    char * topic;
};

typedef int sock;
//param is the nsqlookeupd's ip and port ,return the socket fd
int subscribe(const char *address, const char * port, struct NSQMessage *msg);

#endif //STRUGGLE_NSQ_SUB_CLIENT_H
