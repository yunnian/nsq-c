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
};
struct NSQDConnection {
    char * topic;
};

typedef int sock;
//param is the nsqlookeupd's ip and port ,return the socket fd
sock connect_nsqd_with_lookupd(const char *address, const char * port);
int subscribe(sock, struct NSQMessage *msg, int (*msg_callback)(struct NSQMessage *msg));

#endif //STRUGGLE_NSQ_SUB_CLIENT_H
