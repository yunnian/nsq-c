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

int main(int argc, char **argv) {
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
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

	//设置一个socket地址结构client_addr,代表客户机internet地址, 端口
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
	/*
	char * msg2 ="PUB test\n";
	char * msg3 ="{\"msg\":1,\"run\":\"0d6658215404905adda083e7811632da\"}";
	int  len = htonl(strlen(msg3));
	//printf("%d",ww);
	//char *l = to23(ww);
	int sendLen = strlen(msg2) + strlen(msg3)+4;
	char* c = (char*)malloc(sendLen + 1);
	memset(c,0x00,sendLen + 1);

	//strcpy(c, msg2);
	int off = 0;
	memcpy(c, msg2, strlen(msg2));
	off = off+9;
	memcpy(c+off, &len,4);

	printf("%d\n",len);
	printf("%d\n",strlen(msg3));
	printf("%d\n",strlen(msg2));
	printf("%d\n",sizeof(msg2));
	printf("%d\n",sizeof(msg3));
	printf("%d\n",sizeof(int)*4);
	memcpy(c+strlen(msg2)+4, msg3, strlen(msg3));
	//WriteU32((char *)(c+strlen(msg2)),htonl(strlen(msg3)));
	//WriteString((char *)(c+strlen(msg2)+4),msg3, strlen(msg3));
	//strcat(c, &ww); 
	//strcat(c, msg3); 

	printf("%s",c);
	//printf("%x",c);
	//sprintf(c, "%s", l);

	send(sock, c,sendLen ,0);  
	free(c);
	 */
	char b[120];
    size_t n;
	char * msg2 ="PUB test\n";
	char * msg3 ="{\"msg\":1,\"run\":\"0d6658215404905adda083e7811632da\"}";
	int  len = htonl(strlen(msg3));

    n = sprintf(b, "%s", msg2);
	memcpy(&b[strlen(msg2)], &len, 4);
    n = sprintf(&b[strlen(msg2)+4], "%s", msg3);
	int sendLen = strlen(msg2) + strlen(msg3)+4;
	send(sock, b,sendLen ,0);  

	str_len = read(sock, message, sizeof(message) - 1);
	if (str_len == -1) {
//		error_handling("read() error");
	}

	printf("Message from server : %s \n", message);
	//close(sock);
	return 0;
}
