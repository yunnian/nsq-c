#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(char* message);

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int ReadI32(const char * pData, int *pValue)
{
    *pValue = (pData[0] << 24) | (pData[1] << 16) | (pData[2] << 8) | pData[3];
    return 0;
}

int connect_nsqd(const char *ip, const char* port){
	int sock;
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));


	//创建用于internet的流协议(TCP)socket
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		error_handling("socket() error");
	}

	//设置一个socket地址结构client_addr,代表客户机internet地址, 端口
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(port));

	//把socket和socket地址结构联系起来
	if( connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1) {
		error_handling("connect() error");
	}
	char * msgs  = (char * ) malloc(4);
	memcpy(msgs, "  V2", 4);
	write(sock, msgs, 4);  
    free(msgs);
    return sock;
}


int publish(int sock, char *topic, char *msg){
	char buf[1024*1024];
    size_t n;
	char * pub_command = malloc(strlen(topic) + strlen("PUB \n"));
    sprintf(pub_command, "%s%s%s", "PUB ",topic, "\n");
	int  len = htonl(strlen(msg));
    n = sprintf(buf, "%s", pub_command);
	memcpy(&buf[strlen(pub_command)], &len, 4);
    n = sprintf(&buf[strlen(pub_command)+4], "%s", msg);
	int sendLen = strlen(pub_command) + strlen(msg)+4;
	send(sock, buf,sendLen ,0);  

	char message[30];
	int str_len;
    while(1) {
        int l = read(sock, &message, 2);
        if(strcmp(message,"OK")==0){
            break;
        }
        if(l == 0){
            break;
        }
    }

    if(strcmp(message,"OK")==0){
        return 0;
    }else{
        return -1;
    }
}
