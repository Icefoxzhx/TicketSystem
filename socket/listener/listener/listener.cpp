#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#pragma comment(lib, "ws2_32.lib") 
using namespace std;
int main(int argc, char* argv[])
{
    system("chcp 65001");
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0) return 0;
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (slisten == INVALID_SOCKET)
    {
        printf("socket error!");
        return 0;
    }
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error!");
    }

    if (listen(slisten, 5) == SOCKET_ERROR)
    {
        printf("listen error!");
        return 0;
    }
    SOCKET sClient;//创建客户端socket
    sockaddr_in remoteAddr;//客户端的地址信息
    int nAddrlen = sizeof(remoteAddr);
    char revData[255];//接收数据缓冲区
    while (1)
    {
        sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
        if (sClient == INVALID_SOCKET)
        {
            printf("accept error!");
            continue;
        }
        if (strcmp(inet_ntoa(remoteAddr.sin_addr), "127.0.0.1"))
        {
            printf("server attack from outside!\n");
            continue;
        }
        int ret = recv(sClient, revData, 255, 0);
        stringstream sstr(revData);
        int cmd_length = 0;
        sstr >> cmd_length;
    //    cout << cmd_length << endl;
        int ii = 0;
        int tmp_length = cmd_length;
        for (; tmp_length != 0; ii++)
            tmp_length /= 10;
    //    cout << ii << endl;
        char real_command[1000];
        strncpy_s(real_command, revData+ii, cmd_length);
        printf("read command: %s\n", real_command);
        if (1)
        {
            char result[1000] = "HAPPY_TRAIN G\n上院 xx-xx xx:xx -> 07-01 19:19 0 1000\n中院 07-02 05:19 -> 07-02 05:24 114 1000\n下院 07-02 15:24 -> xx-xx xx:xx 628 x\n";
            printf("%s\n", result);
            send(sClient, result, strlen(result), 0);
        }
        closesocket(sClient);
    }
    closesocket(slisten);
    WSACleanup();
    return 0;
}