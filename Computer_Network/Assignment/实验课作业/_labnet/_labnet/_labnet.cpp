// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
#define _CRT_SECURE_NO_WARNINGS  

#include "stdafx.h"
#include <iostream>
#include <winsock2.h>   //该头文件定义了Socket编程的功能  
#include <WinSock2.h>
#include <httpext.h>    //该头文件支持HTTP请求  
#include <windef.h>     //该头文件定义了Windows的所有数据基本型态  
#include <Nb30.h>       //该头文件声明了netbios的所有的函数 
#include <stdio.h>      //该头文件声明了输入输出流函数 
#include <stdlib.h>     //该头文件定义了一些通用函数
#include <Iphlpapi.h>
#include <thread>
#pragma comment(lib,"ws2_32.lib") //连接ws2_32.lib库.只要程序中用到Winsock API 函数，都要用到 Ws2_32.lib  
#pragma comment(lib,"netapi32.lib") //连接Netapi32.lib库，MAC获取中用到了NetApi32.DLL的功能  
#pragma comment(lib,"Iphlpapi.lib") 
using std::cout;
using std::endl;


void CheckIP(void)
{
    WSADATA wsaData;
    char name[155];
    char *ip;
    PHOSTENT hostinfo;

    // 调用MAKEWORD()获得Winsocl版本的正确值，用于下面的加载Winscok库  
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
    {
        if (gethostname(name, sizeof(name)) == 0)
        { // 如果成功，将本地主机名存放入由name参数指定的缓冲区中  
            if ((hostinfo = gethostbyname(name)) != NULL)
            { // 这是获取主机，如果获得主机名成功的话，将返回一个指针，指向hostinfo,hostinfo为PHOSTENT型的变量。  
                ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
                // inet_addr()函数把地址串转换为IP地址  
                // 调用inet_ntoa()函数,将hostinfo结构变量中的h_addr_list转化为标准的IP地址(如202.197.11.12.)  
                printf("IP address: %s\n", ip);
            }
        }
        WSACleanup(); // 卸载Winsock库，并释放所有资源  
    }
}


// 通过WindowsNT/Win2000中内置的NetApi32.DLL的功能来实现的。首先通过发送NCBENUM命令,获取网卡的  
// 数目和每张网卡的内部编号,然后对每个网卡标号发送NCBASTAT命令获取其MAC地址。  
int getMAC(char* mac, char* dhcp) // 用NetAPI来获取网卡MAC地址  
{
    NCB ncb; // 定义一个NCB(网络控制块)类型的结构体变量ncb  
    typedef struct _ASTAT_  // 自定义一个结构体_ASTAT_  
    {
        ADAPTER_STATUS adapt;
        NAME_BUFFER NameBuff[30] = { 0 };
    }ASTAT, *PASTAT;
    ASTAT Adapter;

    typedef struct _LANA_ENUM  // 自定义一个结构体_LANA_ENUM  
    {
        UCHAR length;
        UCHAR lana[MAX_LANA];  //存放网卡MAC地址   
    }LANA_ENUM;
    LANA_ENUM lana_enum;

    // 取得网卡信息列表       
    UCHAR uRetCode;
    memset(&ncb, 0, sizeof(ncb)); // 将已开辟内存空间ncb 的值均设为值 0  
    memset(&lana_enum, 0, sizeof(lana_enum)); // 清空一个结构类型的变量lana_enum，赋值为0  
                                              // 对结构体变量ncb赋值  
    ncb.ncb_command = NCBENUM; // 统计系统中网卡的数量  
    ncb.ncb_buffer = (unsigned char *)&lana_enum; // ncb_buffer成员指向由LANA_ENUM结构填充的缓冲区  
    ncb.ncb_length = sizeof(LANA_ENUM);
    // 向网卡发送NCBENUM命令，以获取当前机器的网卡信息，如有多少个网卡，每个网卡的编号（MAC地址）   
    uRetCode = Netbios(&ncb); // 调用netbois(ncb)获取网卡序列号      
    if (uRetCode != NRC_GOODRET)
        return uRetCode;

    // 对每一个网卡，以其网卡编号为输入编号，获取其MAC地址     
    for (int lana = 0; lana < lana_enum.length; lana++)
    {
        ncb.ncb_command = NCBRESET; // 对网卡发送NCBRESET命令，进行初始化  
        ncb.ncb_lana_num = lana_enum.lana[lana];
        uRetCode = Netbios(&ncb);

        if (uRetCode != NRC_GOODRET)
            return uRetCode;

        // 准备取得接口卡的状态块取得MAC地址  
        memset(&ncb, 0, sizeof(ncb));
        ncb.ncb_command = NCBASTAT; // 对网卡发送NCBSTAT命令，获取网卡信息  
        ncb.ncb_lana_num = lana_enum.lana[0]; // 指定网卡号，这里仅仅指定第一块网卡，通常为有线网卡   
        strcpy((char*)ncb.ncb_callname, "*"); // 远程系统名赋值为*  
        ncb.ncb_buffer = (unsigned char *)&Adapter; //指定返回的信息存放的变量  
        ncb.ncb_length = sizeof(Adapter);
        // 接着发送NCBASTAT命令以获取网卡的信息  
        uRetCode = Netbios(&ncb);
        // 取得网卡的信息，并且如果网卡正常工作的话，返回标准的冒号分隔格式。     
        if (uRetCode != NRC_GOODRET)
            return uRetCode;
        // 把网卡MAC地址格式转化为常用的16进制形式,输出到字符串mac中   
        sprintf(mac, "%02X-%02X-%02X-%02X-%02X-%02X",
            Adapter.adapt.adapter_address[0],
            Adapter.adapt.adapter_address[1],
            Adapter.adapt.adapter_address[2],
            Adapter.adapt.adapter_address[3],
            Adapter.adapt.adapter_address[4],
            Adapter.adapt.adapter_address[5]
        );
        //printf("adapter name : %s\n", Adapter.NameBuff);
    }

    ///////////////////////////////////////////////////////////////////////////
    // get DHCP
    ///////////////////////////////////////////////////////////////////////////

    PIP_ADAPTER_INFO pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
    unsigned long size = sizeof(IP_ADAPTER_INFO);

    // 调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中size参数既是一个输入量也是一个输出量
    // GetAdaptersInfo参数传递的内存空间不够,同时其传出size,表示需要的空间大小
    unsigned long nRel = GetAdaptersInfo(pIpAdapterInfo, &size);

    //如果函数返回的是ERROR_BUFFER_OVERFLOW
    //则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
    if (nRel == ERROR_BUFFER_OVERFLOW)
    {
        //释放原有的空间
        free(pIpAdapterInfo);

        //重新申请内存空间用来存储所有网卡信息
        pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(size);

        //再次调用GetAdaptersInfo获取网卡信息
        nRel = GetAdaptersInfo(pIpAdapterInfo, &size);
    }

    //如果函数返回的是ERROR_SUCCESS，则说明获取网卡信息成功
    PIP_ADAPTER_INFO pHeadIpAdapterInfo = pIpAdapterInfo;
    if (nRel == ERROR_SUCCESS)
    {
        sprintf(dhcp, pIpAdapterInfo->DhcpServer.IpAddress.String);
    }

    return 0;
}


void test_get_adapter()
{
    PIP_ADAPTER_INFO pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
    unsigned long size = sizeof(IP_ADAPTER_INFO);

    // 调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中size参数既是一个输入量也是一个输出量
    // GetAdaptersInfo参数传递的内存空间不够,同时其传出size,表示需要的空间大小
    unsigned long nRel = GetAdaptersInfo(pIpAdapterInfo, &size);

    //如果函数返回的是ERROR_BUFFER_OVERFLOW
    //则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
    if (nRel == ERROR_BUFFER_OVERFLOW)
    {
        //释放原有的空间
        free(pIpAdapterInfo);

        //重新申请内存空间用来存储所有网卡信息
        pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(size);

        //再次调用GetAdaptersInfo获取网卡信息
        nRel = GetAdaptersInfo(pIpAdapterInfo, &size);
    }

    //如果函数返回的是ERROR_SUCCESS，则说明获取网卡信息成功
    PIP_ADAPTER_INFO pHeadIpAdapterInfo = pIpAdapterInfo;
    if (nRel == ERROR_SUCCESS)
    {
        int netCardNum = 0;    //记录网卡的数量

        int IPnumPerNetCard = 0; //记录每张网卡上的IP地址数量

        while (pIpAdapterInfo)
        {
            ++netCardNum;
            cout << "网卡的编号:" << netCardNum << endl;
            cout << "网卡的名称：" << pIpAdapterInfo->AdapterName << endl;
            cout << "网卡的描述:" << pIpAdapterInfo->Description << endl;

            switch (pIpAdapterInfo->Type)
            {
            case MIB_IF_TYPE_OTHER:
                cout << "网卡类型：" << "OHTER" << endl;
                break;
            case MIB_IF_TYPE_ETHERNET:
                cout << "网卡类型：" << "ETHRENET" << endl;
                break;
            case MIB_IF_TYPE_TOKENRING:
                cout << "网卡类型：" << "TOKENRING" << endl;
                break;
            case MIB_IF_TYPE_FDDI:
                cout << "网卡类型：" << "FDDI" << endl;
                break;
            case MIB_IF_TYPE_PPP:
                printf("PP\n");
                cout << "网卡类型：" << "PP" << endl;
                break;
            case MIB_IF_TYPE_LOOPBACK:
                cout << "网卡类型：" << "SLIP" << endl;
                break;
            default:
                break;
            }
            cout << "网卡MAC地址:";
            for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; ++i)
            {
                if (i < pIpAdapterInfo->AddressLength - 1)
                {
                    printf("%02X-", pIpAdapterInfo->Address[i]);
                }
                else
                {
                    printf("%02X\n", pIpAdapterInfo->Address[i]);
                }
            }
            cout << "网卡的IP地址如下：";

            // 可能网卡有多IP，因此需要循环去判断
            PIP_ADDR_STRING pIpAddrString = &(pIpAdapterInfo->IpAddressList);
            while (pIpAddrString)
            {
                cout << "IP 地址：" << pIpAddrString->IpAddress.String << endl;
                cout << "子网地址：" << pIpAddrString->IpMask.String << endl;
                cout << "网关地址：" << pIpAdapterInfo->GatewayList.IpAddress.String << endl;
                ++IPnumPerNetCard;
                pIpAddrString = pIpAddrString->Next; //让pIpAddrString指向下一个节点
            }
            cout << "该网卡上的IP数量：" << IPnumPerNetCard << endl;

            IPnumPerNetCard = 0;


            cout << "网卡DHCP地址：" << pIpAdapterInfo->DhcpServer.IpAddress.String << endl;


            pIpAdapterInfo = pIpAdapterInfo->Next; //让pIpAdapterInfo指向下一个节点
            cout << "---------------------------------------------" << endl;
        }
    }
    // 释放空间
    if (pHeadIpAdapterInfo)
    {
        free(pHeadIpAdapterInfo);
        pHeadIpAdapterInfo = NULL;
    }
}


void run_client();
void run_server();

int main(void)
{
    /*
    CheckIP();
    char mac[64] = { 0 };
    char dhcp[64] = { 0 };
    getMAC(mac, dhcp);
    printf("MAC address  : %s \n", mac);
    printf("DHCP address : %s \n", dhcp);
    */

    //cout << endl;
    //test();

    cout << endl;
    cout << "start a thread to run server:" << endl;
    std::thread(run_server).detach();

    cout << endl;
    std::thread(run_client).detach();
    std::thread(run_client).detach();
    std::thread(run_client).detach();

    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(3s);
    }
    cout << endl;
    system("pause");
    return 0;
}
