#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include "parser_frontend.h"
using namespace std;
char real_command[100000];
char result[100000];
char real_result[100000];
stringstream real_result_stream;
int main()
{
    int slisten = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(slisten, (struct sockaddr*)&sin, sizeof(sin)) == -1)
        printf("bind error!");
    if (listen(slisten, 5) == -1)
    {
        printf("listen error!");
        return 0;
    }
    int sClient;//创建客户端socket
    sockaddr_in remoteAddr;//客户端的地址信息
    socklen_t nAddrlen = sizeof(remoteAddr);
    char revData[99000];//接收数据缓冲区
    while (1)
    {
    	memset(real_command, 0, sizeof(real_command));
    	memset(result, 0, sizeof(result));
        memset(real_result, 0, sizeof(real_result));
        sClient = accept(slisten, (struct sockaddr*)&remoteAddr, &nAddrlen);
        if (sClient == -1)
        {
            printf("accept error!");
            continue;
        }
        if (strcmp(inet_ntoa(remoteAddr.sin_addr), "49.234.112.112"))
        {
            printf("server attack from outside!\n");
            continue;
        }
        int ret = recv(sClient, revData, 99000, 0);
        stringstream sstr(revData);
        int cmd_length = 0;
        sstr >> cmd_length;
        int ii = 0;
        int tmp_length = cmd_length;
        for (; tmp_length != 0; ii++)
            tmp_length /= 10;
        strncpy(real_command, revData+ii, cmd_length);
        printf("read command: %s\n", real_command);
        get_command_frontend(real_command, result);
        real_result_stream << setw(6) << setfill('0') << strlen(result)-1;
        real_result_stream << result;
        strcpy(real_result, real_result_stream.str().c_str());
        printf("real_result:%s----\n", real_result);
        send(sClient, real_result, strlen(real_result), 0);
        real_result_stream.clear();
        real_result_stream.str("");
    }
    return 0;
}
