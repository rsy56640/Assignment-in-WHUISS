#include "stdafx.h"
#include <cstdio>
#include <winsock2.h>
#include <random>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll

#include <unordered_map>
#include <iostream>
#include <thread>
#include <string>
using std::cout;
using std::endl;

static const char* client_name[] =
{
    "student1",
    "student2",
    "student3",
};
static const char* client_address[] =
{
    "127.0.0.1",
    "127.0.0.1",
    "127.0.0.1",
};
static const int port = 1234;
static const int client_num = sizeof(client_address) / sizeof(const char*);

static char tokens[client_num][64] = { 0 };
static std::vector<SOCKET> clntSocks(client_num);
static std::vector<sockaddr_in> sockAddrs(client_num);
static std::vector<SOCKADDR> addrs(client_num);

struct info_t {
    std::string mac_;
    std::string dhcp_;
};
using name_t = std::string;
std::unordered_map<name_t, info_t> attendee;

void decode(char(&buffer)[64], const char(&token)[64])
{
    for (int i = 0; i < 64; i++)
        buffer[i] ^= token[i];
}


void init_tokens() {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> uniform_dist(0, 63);
    for (int i = 0; i < client_num; i++) {
        for (int j = 0; j < 63; j++) {
            tokens[i][j] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789#$"[uniform_dist(e)];
        }
        tokens[i][63] = '\0';
    }
}


void run_server()
{
    init_tokens();

    // 初始化 DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 创建套接字
    SOCKET socks = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 创建完成端口句柄
    HANDLE completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    // TODO: epoll on windows
    for (int i = 0; i < client_num; i++)
    {
        // 绑定套接字
        memset(&sockAddrs[i], 0, sizeof(sockaddr_in));                  // 每个字节都用0填充
        sockAddrs[i].sin_family = PF_INET;                              // 使用IPv4地址
        sockAddrs[i].sin_addr.s_addr = inet_addr(client_address[i]);    // 具体的IP地址
        sockAddrs[i].sin_port = htons(port);                            // 端口
        bind(socks, (SOCKADDR*)&sockAddrs[i], sizeof(SOCKADDR));

        // 进入监听状态
        listen(socks, 20);

        // 接收客户端请求
        int nSize = sizeof(SOCKADDR);
        clntSocks[i] = accept(socks, (SOCKADDR*)&addrs[i], &nSize);

        // 向客户端发送数据
        send(clntSocks[i], tokens[i], sizeof(tokens[i]), NULL);

        char buffer[64];

        memset(buffer, 0, sizeof(buffer));
        recv(clntSocks[i], buffer, sizeof(buffer), NULL);
        decode(buffer, tokens[i]);
        std::string mac = buffer;
        printf("server>>> recv from client \"%s\" MAC: %s\n", client_name[i], buffer);

        memset(buffer, 0, sizeof(buffer));
        recv(clntSocks[i], buffer, sizeof(buffer), NULL);
        decode(buffer, tokens[i]);
        std::string dhcp = buffer;
        printf("server>>> recv from client \"%s\" DHCP: %s\n", client_name[i], buffer);

        attendee[client_name[i]] = { std::move(mac), std::move(dhcp) };

        // 关闭套接字
        closesocket(clntSocks[i]);
    }
    closesocket(socks);

    // 终止 DLL 的使用
    WSACleanup();


    std::cout << "atendee info:" << endl;
    for (auto const&[name, info] : attendee) {
        std::cout << name << "\t";
        std::cout << "MAC: " << info.mac_ << "\t";
        std::cout << "DHCP: " << info.dhcp_ << "\t";
        std::cout << endl;
    }

}