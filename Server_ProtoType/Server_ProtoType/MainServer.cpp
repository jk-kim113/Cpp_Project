#include "MainServer.h"
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <stdlib.h>
#include <string>
#include <queue>
#include <mutex>

#include "SocketManager.h"
#include "PacketClass.h"

#pragma comment(lib, "ws2_32")

using namespace std;

#define PACKET_SIZE 1032

mutex _mtx;
SocketManager _socketMgr;
map<int, vector<int>> _classGroup;

typedef struct PacketInfo {
	int _id;
	int _totalSize;
	char _data[1024];
};

typedef struct P_ClientInfo {
	int _schoolID;
	int _grade;
	int _group;
};

typedef struct P_StudentInfo {
	int _schoolID;
	int _grade;
	int _group;
	int _number;
};

queue<PacketClass> _fromClientQueue;

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
	while (true)
	{
		if (listen(_waitServer, SOMAXCONN) == 0)
		{
			SOCKADDR_IN client = {};
			int clntSize = sizeof(client);
			ZeroMemory(&client, clntSize);

			SOCKET clientSkt = accept(_waitServer, (SOCKADDR*)&client, &clntSize);

			SocketClass* socketClass = new SocketClass(clientSkt, _socketMgr._SocketCount());
			thread* clientThr = new thread(&MainServer::ListenClient, this, socketClass);
			socketClass->ExecuteThread(clientThr);

			_socketMgr.AddSocket(socketClass);

			std::cout << "Accept Client!\n";
		}
	}
}

void MainServer::ListenClient(SocketClass* socketClass)
{
	std::cout << "Listen Client!\n";

	char buffer[PACKET_SIZE] = { 0 };
	ZeroMemory(&buffer, PACKET_SIZE);

	while (true)
	{
		int recvLen = recv(socketClass->_MySocket(), buffer, PACKET_SIZE, 0);
		if (recvLen > 0)
		{
			PacketInfo packet;
			memcpy(&packet, &buffer, sizeof(buffer));

			PacketClass packetClass;
			packetClass.AccessServer(packet._id, packet._data, packet._totalSize, 0);

			_mtx.lock();
			_fromClientQueue.push(packetClass);
			_mtx.unlock();
		}
	}
}

void MainServer::DoOrder()
{
	while (true)
	{
		_mtx.lock();
		if (!_fromClientQueue.empty())
		{
			PacketClass packet = _fromClientQueue.front();
			_fromClientQueue.pop();

			std::cout << "Do Order!\n";

			switch (packet._ProtocolID())
			{
			case 0:

				std::cout << "관리자 클라이언트 정보 들어옴" << "\n";
				P_ClientInfo pClientInfo;
				memcpy(&pClientInfo, packet._Data(), packet._DataSize());

				std::cout << "School Name : " << pClientInfo._schoolID << "\n";
				std::cout << "Grade : " << pClientInfo._grade << "학년" << "\n";
				std::cout << "Group : " << pClientInfo._group << "반" << "\n";
				int key;
				key = pClientInfo._schoolID + pClientInfo._grade + pClientInfo._group;

				_socketMgr.AddManagerSocket(key, packet._CastIdentifier());

				//if (_classGroup.find(key) != _classGroup.end())
				//{
				//	// exist
				//	std::cout << "There is Group" << "\n";
				//}
				//else
				//{
				//	// not exist
				//	std::cout << "There is no Group" << "\n" << "Success to Create Group" << "\n";
				//	vector<int> temp;
				//	_classGroup[key] = temp;
				//}

				break;

			case 100:

				std::cout << "학생 정보 들어옴" << "\n";
				P_StudentInfo pStudentInfo;
				memcpy(&pStudentInfo, packet._Data(),  packet._DataSize());

				std::cout << "School Name : " << pStudentInfo._schoolID << "\n";
				std::cout << "Grade : " << pStudentInfo._grade << "학년" << "\n";
				std::cout << "Group : " << pStudentInfo._group << "반" << "\n";
				std::cout << "Number : " << pStudentInfo._number << "번" << "\n";

				int checkkey;
				checkkey = pStudentInfo._schoolID + pStudentInfo._grade + pStudentInfo._group;
				if (_classGroup.find(checkkey) != _classGroup.end())
				{
					// exist

					vector<int>::iterator it;
					it = std::find(_classGroup[checkkey].begin(), _classGroup[checkkey].end(), pStudentInfo._number);

					if (it != _classGroup[checkkey].end())
					{
						std::cout << "Already Exist Same Student" << "\n";
					}
					else
					{
						_classGroup[checkkey].push_back(pStudentInfo._number);

						std::cout << "Success to Join Student" << "\n";
						std::cout << "현재 클래스의 학생 수 : " << _classGroup[checkkey].size() << "\n";

						// 소켓 매니저에서 관리자 map<int, SocketClass> 학생 map<int, vector<SocketClass>> 따로 관리하는게 좋을듯

					}
				}
				else
				{
					// not exist
					std::cout << "Fail to Join Student" << "\n";
				}

				break;

			default:
				break;
			}
		}
		_mtx.unlock();
	}
}