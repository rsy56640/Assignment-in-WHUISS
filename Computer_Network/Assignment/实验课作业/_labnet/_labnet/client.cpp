#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

const char* ip_addr = "127.0.0.1";
static const int port = 1234;

int getMAC(char* mac, char* dhcp);

void encode(char(&buffer)[64], const char(&token)[64])
{
    for (int i = 0; i < 64; i++)
        buffer[i] ^= token[i];
}


void run_client()
{
    std::cout << "run client:" << std::endl;

    // 初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip_addr);
    sockAddr.sin_port = htons(port);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //接收服务器传回的数据
    char token[64] = { 0 };
    recv(sock, token, 64, NULL);

    // 输出接收到的数据
    printf("client>>> Token form server: %s\n", token);

    // MAC & DHCP
    char mac[64] = { 0 };
    char dhcp[64] = { 0 };
    getMAC(mac, dhcp);
    encode(mac, token);
    encode(dhcp, token);
    send(sock, mac, sizeof(mac), NULL);
    send(sock, dhcp, sizeof(dhcp), NULL);

    //关闭套接字
    closesocket(sock);

    //终止使用 DLL
    WSACleanup();
}