#include "MainServer.h"
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <stdlib.h>
#include <string>
#include <queue>
#include <mutex>
#include "SocketClass.h"
#include "SocketManager.h"

#pragma comment(lib, "ws2_32")

using namespace std;

#define PACKET_SIZE 1032

mutex _mtx;
SocketManager _socketMgr;

typedef struct PacketInfo {
	int _id;
	int _totalSize;
	char _data[1024];
};

queue<PacketInfo> _fromClientQueue;

void MainServer::CreateServer()
{
	std::cout << "Create Server!\n";

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	_waitServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(_waitServer, (SOCKADDR*)&addr, sizeof(addr));

    //thread* acceptClient = new thread(&AcceptClient, _waitServer);

	thread acceptClient(&MainServer::AcceptClient, this);
	thread doOrder(&MainServer::DoOrder, this);

	char buffer[PACKET_SIZE] = { 0 };
	while (!WSAGetLastError())
	{
		cin >> buffer;
	}
}

void MainServer::AcceptClient()
{
	std::cout << "Accept Client!\n";

	while (true)
	{
		if (listen(_waitServer, SOMAXCONN) == 0)
		{
			SOCKADDR_IN client = {};
			int clntSize = sizeof(client);
			ZeroMemory(&client, clntSize);

			SOCKET clientSkt = accept(_waitServer, (SOCKADDR*)&client, &clntSize);
			thread* clientThr = new thread(&MainServer::ListenClient, this, clientSkt);

			SocketClass* socketClass = new SocketClass(clientSkt, clientThr);
			_socketMgr.AddSocket(socketClass);
		}
	}
}

void MainServer::ListenClient(SOCKET client)
{
	std::cout << "Listen Client!\n";

	char buffer[PACKET_SIZE] = { 0 };
	ZeroMemory(&buffer, PACKET_SIZE);

	while (true)
	{
		int recvLen = recv(client, buffer, PACKET_SIZE, 0);
		if (recvLen > 0)
		{
			PacketInfo packet;
			memcpy(&packet, &buffer, sizeof(buffer));

			_mtx.lock();
			_fromClientQueue.push(packet);
			_mtx.unlock();
		}
	}
}

void MainServer::DoOrder()
{
	while (true)
	{
		_mtx.lock();
		if (_fromClientQueue.empty())
		{
			PacketInfo packet = _fromClientQueue.front();
			_fromClientQueue.pop();

			std::cout << "Do Order!\n";

			switch (packet._id)
			{
			case 0:
				break;

			default:
				break;
			}
		}
		_mtx.unlock();
	}
}