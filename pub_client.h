//
// Created by Zhenyu Wu on 2017/9/14.
//

#ifndef STRUGGLE_NSQ_PUB_CLIENT_C_H
#define STRUGGLE_NSQ_PUB_CLIENT_C_H
int connect_nsqd(const char *ip, const char* port);
int publish(int sock, char *topic, char *msg);

#endif //STRUGGLE_NSQ_PUB_CLIENT_C_H
