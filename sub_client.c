#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>
#include<unistd.h>
#include <inttypes.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"sub_client.h"

void error_handling(char* message);

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int readI16(const unsigned char * pData, uint16_t *pValue)
{
    *pValue = (pData[0] << 8) | pData[1];
    return 0;
}
int readI32(const unsigned char * pData, int32_t *pValue)
{
    *pValue = (pData[0] << 24) | (pData[1] << 16) | (pData[2] << 8) | pData[3];
    return 0;
}


int readI64(const unsigned char* data, int64_t *pValue)
{
    *pValue = ((uint64_t)data[0] << 56) | ((uint64_t)data[1] << 48) | ((uint64_t)data[2] << 40) |((uint64_t)data[3] << 32)|((uint64_t)data[4] << 24) |((uint64_t)data[5] << 16) | ((uint64_t)data[6] << 8) |(uint64_t) data[7];
    return 0;

}
uint64_t ntoh64(const uint8_t *data) {
    return (uint64_t)(data[7]) | (uint64_t)(data[6])<<8 |
        (uint64_t)(data[5])<<16 | (uint64_t)(data[4])<<24 |
        (uint64_t)(data[3])<<32 | (uint64_t)(data[2])<<40 |
        (uint64_t)(data[1])<<48 | (uint64_t)(data[0])<<56;
}


sock connect_nsqd_with_lookupd(const char *address, const char* port){
    int sock;
    struct sockaddr_in serv_addr;
    // char message[2560];
    int str_len;
    memset(&serv_addr, 0, sizeof(serv_addr));

    /*
    if (argc != 3)
    {
        printf("Usage: /%s <IP> <Port>\n",argv[0]);
        exit(1);
    }
    */

    //创建用于internet的流协议(TCP)socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    //设置一个sockint64_tet地址结构client_addr,代表客户机internet地址, 端口
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(address);
    serv_addr.sin_port = htons(atoi(port));

    //把socket和socket地址结构联系起来
    if( connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }
    return sock;

}

int subscribe(sock sock,struct NSQMessage *msg, int (*msg_callback)(struct NSQMessage *msg)){
    char * v  = (char * ) malloc(4);
    memcpy(v, "  V2", 4);
    write(sock, v, 4);
    free(v);
    char b[120];
    size_t n;
    char * msg2 ="SUB test Struggle ZhenyudeMacBook-Pro ZhenyudeMacBook-Pro.local\n";
    n = sprintf(b, "%s", msg2);
    send(sock, b,strlen(msg2) ,0);
    char * rd =  "RDY 2\n";
    send(sock, rd,strlen(rd) ,0);

    errno = 0;
    int i = 0;
    while(1){

        //读取msg长度
        char * msg_size = malloc(4);
        memset(msg_size,0x00,4);
        int size_l =  read(sock, msg_size, 4);
        readI32((const unsigned char *) msg_size ,  &msg->size);

        //读取相应长度的msg内容
        char * message = malloc(msg->size +1);
        memset(message,0x00,msg->size);
        int l =  read(sock, message, msg->size);
        if (errno) {
            printf("errno = %d\n", errno); // errno = 33
            perror("sqrt failed"); // sqrt failed: Numerical argument out of domain
            printf("error: %s\n", strerror(errno)); // error: Numerical argument out of domain
        }
        if(l){
            msg->message_id = (char * )malloc(17);
            memset(msg->message_id,'\0',17);
            readI32((const unsigned char *)message, &msg->frame_type);

            if(msg->frame_type == 0){
                printf("%s","OK");
                if(msg->size == 15){
                    send(sock, "NOP\n",strlen("NOP\n") ,0);
                }
            }else if(msg->frame_type == 2){
                msg->timestamp = (int64_t)ntoh64((const unsigned char *)message+4);
                readI16((const unsigned char *)message+12,  &msg->attempts);
                memcpy(msg->message_id, message+14, 16);
                msg->body = (char * )malloc(msg->size-30);
                memset(msg->body,'\0',msg->size-30);
                memcpy(msg->body,message+30, msg->size-30);
                char  ack[22] = "FIN " ;
                //strcat(ack, messageId);
                sprintf(ack,"FIN %s\n",msg->message_id);
                send(sock, ack,strlen(ack) ,0);
                if(i< msg->rdy){
                    i++;
                }else{
                    send(sock,rd,strlen(rd) ,0);  
                    i =0;
                }
                msg_callback(msg);
                free(msg->body);
            }
            free(message);
            free(msg_size);
            free(msg->message_id);
            continue ;
        }else{
            printf("%s","socket close");
            break ;
        }
        /*if (str_len == -1) {
          error_handling("read() error");;
          }*/

        //close(sock);
    }

    return 0;


}
