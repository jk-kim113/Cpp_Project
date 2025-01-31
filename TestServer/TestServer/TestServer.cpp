﻿#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <stdlib.h>

#pragma comment(lib, "ws2_32")

using namespace std;

#define PORT        4578
#define PACKET_SIZE 1032

SOCKET skt;
bool _isSendTest = false;

vector<SOCKET> _clientGroup;
vector<thread*> _clientThread;

mutex _mtx;

bool _isFileDownTest = false;

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

void proc_recvs(SOCKET clientSkt)
{
    printf("Wait Client Data\n");
    char buffer[PACKET_SIZE] = { 0 };
    ZeroMemory(&buffer, PACKET_SIZE);

    while (true)
    {   
        int recvLen = recv(clientSkt, buffer, PACKET_SIZE, 0);
        if (recvLen > 0)
        {
            PacketInfo packet;
            memcpy(&packet, &buffer, sizeof(buffer));

            _mtx.lock();
            _fromClientQueue.push(packet);
            printf("Push Data\n");
            _mtx.unlock();
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

            char szPacket[1032];
            memset(szPacket, 0x0, sizeof(szPacket));
            memcpy(szPacket, &packet, sizeof(packet));

            *((PacketInfo*)szPacket) = packet;

            /*vector<SOCKET>::iterator sitr;
            for (sitr = _clientGroup.begin(); sitr != _clientGroup.end(); ++sitr)
            {
                send(*sitr, szPacket, 1032, 0);
            }*/
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
            
            SOCKET clientSkt = accept(skt, (SOCKADDR*)&client, &iClntSize);
            _clientGroup.push_back(clientSkt);

            thread* clientThr = new thread(&proc_recvs, clientSkt);
            _clientThread.push_back(clientThr);

            if (!_isFileDownTest)
            {
                _isFileDownTest = true;
                int sendBytes;
                long file_size;
                char buf[1024];
                FILE* fp;
                fp = fopen("D:\\1_triceratops", "rb");
                fseek(fp, 0, SEEK_END);
                file_size = ftell(fp);
                printf("File Size : %d", file_size);
                fseek(fp, 0, SEEK_SET);
                snprintf(buf, sizeof(buf), "%d", file_size);
                send(clientSkt, buf, sizeof(buf), 0);
                while ((sendBytes = fread(buf, sizeof(char), sizeof(buf), fp)) > 0)
                {
                    send(clientSkt, buf, sendBytes, 0);
                }
                fclose(fp);
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
    thread doOrder(DoOrder);

    while (!WSAGetLastError())
    {
        cin >> buffer;
        //send(client_sock, buffer, strlen(buffer), 0);
    }

    printf("클라이언트 쪽에 보내는 부분에서 서버가 종료됩니다");

    acceptClient.join();
    doOrder.join();

    _mtx.lock();
    vector<SOCKET>::iterator sitr;
    for (sitr = _clientGroup.begin(); sitr != _clientGroup.end(); ++sitr)
        closesocket(*sitr);
    _clientGroup.clear();
    _mtx.unlock();

    vector<thread*>::iterator itr;
    for (itr = _clientThread.begin(); itr != _clientThread.end(); ++itr)
    {
        if ((*itr)->joinable())
            (*itr)->join();

        delete (*itr);
    }
    _clientThread.clear();

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
