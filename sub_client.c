#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>
#include<unistd.h>
//#include <inttypes.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"sub_client.h"

void error_handling(char* message);

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
int readI16(const unsigned char * pData, int16_t *pValue)
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

int subscribe(sock sock,struct NSQMessage *msg, (*msg_callback)(struct NSQMessage *msg)){
    char message[1024*1024];
	//char * msgs = "  V2";
	char * v  = (char * ) malloc(4);
	memcpy(v, "  V2", 4);
	write(sock, v, 4);
	char b[120];
    size_t n;
	char * msg2 ="SUB test Struggle ZhenyudeMacBook-Pro ZhenyudeMacBook-Pro.local\n";
    n = sprintf(b, "%s", msg2);
	send(sock, b,strlen(msg2) ,0);
    char * rd =  "RDY 1\n";
	send(sock, rd,strlen(rd) ,0);

    errno = 0;
    while(1){

        printf("read前:%s\n", "哈哈");
        int l =  read(sock, message, sizeof(message));
        if (errno) {
            printf("errno = %d\n", errno); // errno = 33
            perror("sqrt failed"); // sqrt failed: Numerical argument out of domain
            printf("error: %s\n", strerror(errno)); // error: Numerical argument out of domain
        }
        printf("%d", l);
        if(l){
            //struct NSQMessage *msg;
            //msg = (struct NSQMessage *)malloc(sizeof(struct NSQMessage));
            //int32_t size,frameType ;
            //int16_t attempts;
            //int64_t timestamp;
            msg->message_id = (char * )malloc(17);
            memset(msg->message_id,'\0',17);
            readI32((const unsigned char *)message, &msg->size);
            readI32((const unsigned char *)message+4, &msg->frame_type);
            printf("size : %d \n", msg->size);
            printf("frameType : %d \n", msg->frame_type);

            if(msg->frame_type == 0){
                printf("%s","OK");
                printf("message:%s",message);
                if(msg->size == 15){
                    send(sock, "NOP\n",strlen("NOP\n") ,0);
                }
                //处理粘包
                if(l!=19 && l!=10){
                    printf("%s","哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈啊哈");
                    memmove(message,message+10,l-10);
                    message[l-10] = '\0';
                    goto message;
                }
            }else if(msg->frame_type == 2){
                message:
                //readI64( message+8, &timestamp);
				//msg->timestamp = (int64_t)ntoh64(message+8);
				msg->timestamp = (int64_t)ntoh64((const unsigned char *)message+8);
                printf("上%lld", msg->timestamp);

                //readI16(message+16, &msg->attempts);
                readI16((const unsigned char *)message+16, &msg->attempts);
                memcpy(msg->message_id, message+18, 16);
                printf("messageId : %s \n", msg->message_id);
                msg->body = (char * )malloc(msg->size+1);
                //重新读下size
                readI32((const unsigned char *)message, &msg->size);
                memset(msg->body,0x00,msg->size);
                memcpy(msg->body,message+34, msg->size);
                printf("Message from server : %s \n", msg->body);
                char  ack[220] = "FIN " ;
                printf("ack : %s \n", ack);
                //strcat(ack, messageId);

                sprintf(ack,"FIN %s\n",msg->message_id);
                printf("ack : %s \n", ack);
                send(sock, ack,strlen(ack) ,0);
                //send(sock,rd,strlen(rd) ,0);  
                printf("attempts : %d \n", msg->attempts);
            }
            memset(message,'\0',sizeof(message));
            continue ;
        }else{
            printf("%s","1");
            break ;
        }
        /*if (str_len == -1) {
    		error_handling("read() error");;
        }*/

        //close(sock);
    }

    return 0;


}

/*
int main(int argc, char **argv) {
	int sock;
	struct sockaddr_in serv_addr;
	char message[1024*1024];
    // char message[2560];
	int str_len;
	memset(&serv_addr, 0, sizeof(serv_addr));

	if (argc != 3)
	{
		printf("Usage: /%s <IP> <Port>\n",argv[0]);
		exit(1);
	}

	//创建用于internet的流协议(TCP)socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		error_handling("socket() error");
	}

	//设置一个sockint64_tet地址结构client_addr,代表客户机internet地址, 端口
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	//把socket和socket地址结构联系起来
	if( connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1) {
		error_handling("connect() error");
	}




	//char * msgs = "  V2";
	char * msgs  = (char * ) malloc(4);
	memcpy(msgs, "  V2", 4);
	write(sock, msgs, 4);
	char b[120];
    size_t n;
	char * msg2 ="SUB test Struggle ZhenyudeMacBook-Pro ZhenyudeMacBook-Pro.local\n";
    n = sprintf(b, "%s", msg2);
	send(sock, b,strlen(msg2) ,0);
    char * rd =  "RDY 1\n";
	send(sock, rd,strlen(rd) ,0);

    errno = 0;
    while(1){

        printf("read前:%s\n", "哈哈");
        int l =  read(sock, message, sizeof(message));
        if (errno) {
            printf("errno = %d\n", errno); // errno = 33
            perror("sqrt failed"); // sqrt failed: Numerical argument out of domain
            printf("error: %s\n", strerror(errno)); // error: Numerical argument out of domain
        }
        printf("%d", l);
        if(l){
            struct NSQMessage *msg;
            msg = (struct NSQMessage *)malloc(sizeof(struct NSQMessage));
            //int32_t size,frameType ;
            //int16_t attempts;
            //int64_t timestamp;
            msg->message_id = (char * )malloc(17);
            memset(msg->message_id,'\0',17);
            readI32(message, &msg->size);
            readI32(message+4, &msg->frame_type);
            printf("size : %d \n", msg->size);
            printf("frameType : %d \n", msg->frame_type);

            if(msg->frame_type == 0){
                printf("%s","OK");
                printf("message:%s",message);
                if(msg->size == 15){
                    send(sock, "NOP\n",strlen("NOP\n") ,0);
                }
                //处理粘包
                if(l!=19 && l!=10){
                    printf("%s","哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈啊哈");
                    memmove(message,message+10,l-10);
                    message[l-10] = '\0';
                    goto message;
                }
            }else if(msg->frame_type == 2){
                message:
                //readI64( message+8, &timestamp);
				msg->timestamp = (int64_t)ntoh64((const unsigned char *)message+8);
                printf("上%lld", msg->timestamp);

                readI16((const unsigned char *)message+16, &msg->attempts);
                memcpy(msg->message_id, message+18, 16);
                printf("messageId : %s \n", msg->message_id);
                msg->body = (char * )malloc(msg->size+1);
                //重新读下size
                readI32(message, &msg->size);
                memset(msg->body,0x00,msg->size);
                memcpy(msg->body,message+34, msg->size);
                printf("Message from server : %s \n", msg->body);
                char  ack[220] = "FIN " ;
                printf("ack : %s \n", ack);
                //strcat(ack, messageId);

                sprintf(ack,"FIN %s\n",msg->message_id);
                printf("ack : %s \n", ack);
                send(sock, ack,strlen(ack) ,0);
                //send(sock,rd,strlen(rd) ,0);  
                printf("attempts : %d \n", msg->attempts);
            }
            memset(message,'\0',sizeof(message));
            continue ;
        }else{
            printf("%s","1");
            break ;
        }
        if (str_len == -1) {
    		error_handling("read() error");;
        }

        //close(sock);
    }
	return 0;
}
    */
