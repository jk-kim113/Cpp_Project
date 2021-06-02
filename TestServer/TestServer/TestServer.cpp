#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>

#pragma comment(lib, "ws2_32")

using namespace std;

#define PORT        4578
#define PACKET_SIZE 1032
#define CLIENT_COUNT 4

SOCKET skt, client_sock;
bool _isSendTest = false;

SOCKET clientSocket[CLIENT_COUNT];

vector<SOCKET> _clientGroup;
vector<thread*> _clientThread;

mutex _mtx;

typedef struct serverData {
    int _id;
    int _age;
};

typedef struct PacketInfo {
    int _id;
    int _totalSize;
    char _data[1024];
};

typedef struct Packet_Chat {
    char _chat[100];
};

queue<PacketInfo> _fromClientQueue;

void proc_recvs()
{
    printf("Wait Client Data\n");
    char buffer[PACKET_SIZE] = { 0 };
    ZeroMemory(&buffer, PACKET_SIZE);

    while (true)
    {   
        for (int n = 0; n < CLIENT_COUNT; n++)
        {
            if (clientSocket[n] != NULL)
            {
                int recvLen = recv(clientSocket[n], buffer, PACKET_SIZE, 0);
                if (recvLen > 0)
                {
                    PacketInfo packet;
                    memcpy(&packet, &buffer, sizeof(buffer));

                    _mtx.lock();
                    _fromClientQueue.push(packet);
                    printf("Push Data\n");
                    _mtx.unlock();

                    break;
                }
            }
        }

        //recv(client_sock, buffer, PACKET_SIZE, 0);

        /*if (!_isSendTest)
        {
            serverData sData;
            sData._id = 1;
            sData._age = 10;

            char szPacket[1024];
            memset(szPacket, 0x0, sizeof(szPacket));
            memcpy(szPacket, &sData, sizeof(sData));

            *((serverData*)szPacket) = sData;

            send(client_sock, szPacket, 1024, 0);

            _isSendTest = true;
        }*/
    }

    printf("클라이언트를 받아오는 쪽에서 서버가 종료됩니다");
}

void DoOrder()
{
    while (true)
    {
        _mtx.lock();
        if (!_fromClientQueue.empty())
        {
            PacketInfo packet = _fromClientQueue.front();
            _fromClientQueue.pop();

            Packet_Chat pChat;
            memcpy(&pChat, &packet._data, sizeof(packet._data));

            std::cout << "Packet Message : " << pChat._chat << "\n";
        }
        _mtx.unlock();
    }
}

void AcceptClient()
{
    while (true)
    {
        if (listen(skt, SOMAXCONN) == 0)
        {
            SOCKADDR_IN client = {};
            int iClntSize = sizeof(client);
            ZeroMemory(&client, iClntSize);
            //client_sock = accept(skt, (SOCKADDR*)&client, &iClntSize);

            /*_clientSocketList.push_back(accept(skt, (SOCKADDR*)&client, &iClntSize));
            printf("%d번째 클라이언트 접속\n", (int)_clientSocketList.size());*/

            _clientGroup.push_back(accept(skt, (SOCKADDR*)&client, &iClntSize));

            for (int n = 0; n < CLIENT_COUNT; n++)
            {
                if (clientSocket[n] == NULL)
                {
                    clientSocket[n] = accept(skt, (SOCKADDR*)&client, &iClntSize);
                    printf("%d번째 클라이언트 접속\n", n + 1);
                    break;
                }
            }
        }
    }
}

void ServerTest()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(skt, (SOCKADDR*)&addr, sizeof(addr));
    
    thread acceptClient(AcceptClient);

    char buffer[PACKET_SIZE] = { 0 };
    thread proc2(proc_recvs);
    thread doOrder(DoOrder);

    while (!WSAGetLastError())
    {
        cin >> buffer;
        send(client_sock, buffer, strlen(buffer), 0);
    }

    printf("클라이언트 쪽에 보내는 부분에서 서버가 종료됩니다");

    acceptClient.join();
    proc2.join();
    doOrder.join();
    closesocket(client_sock);
    closesocket(skt);

    WSACleanup();
}

void ByteArrayToStructure()
{
    serverData sData;
    sData._id = 1;
    sData._age = 10;

    char szPacket[1024];
    memset(szPacket, 0x0, sizeof(szPacket));
    memcpy(szPacket, &sData, sizeof(sData));

    *((serverData*)szPacket) = sData;
}

int main()
{
    std::cout << "Hello World!\n";

    ServerTest();
    //ByteArrayToStructure();
    
    return 0;
}
