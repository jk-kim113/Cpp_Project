#include "MainServer.h"
#include <iostream>

#include <thread>
#include <stdlib.h>
#include <string>
#include <queue>
#include <mutex>

#include "SocketManager.h"
#include "PacketClass.h"
#include "MainDB.h"

#pragma comment(lib, "ws2_32")

using namespace std;

#define PACKET_SIZE 1032

mutex _mtx;
SocketManager _socketMgr;
map<int, vector<int>> _classGroup;
MainDB _mainDB;

queue<PacketClass> _fromClientQueue;

int _clientCnt = 1;

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

	_mainDB.InitDB();

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

			SocketClass* socketClass = new SocketClass(clientSkt, _clientCnt);
			_clientCnt++;
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
			packetClass.AccessServer(packet._id, packet._data, packet._totalSize, socketClass->_MyIndex());

			std::cout << "���� �ε��� �߰� ���� : " << socketClass->_MyIndex() << "\n";

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

				std::cout << "������ Ŭ���̾�Ʈ ���� ����" << "\n";
				P_ClientInfo pClientInfo;
				memcpy(&pClientInfo, packet._Data(), packet._DataSize());

				std::cout << "School Name : " << pClientInfo._schoolID << "\n";
				std::cout << "Grade : " << pClientInfo._grade << "�г�" << "\n";
				std::cout << "Group : " << pClientInfo._group << "��" << "\n";
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

				std::cout << "�л� ���� ����" << "\n";
				P_StudentInfo pStudentInfo;
				memcpy(&pStudentInfo, packet._Data(),  packet._DataSize());

				std::cout << "School Name : " << pStudentInfo._schoolID << "\n";
				std::cout << "Grade : " << pStudentInfo._grade << "�г�" << "\n";
				std::cout << "Group : " << pStudentInfo._group << "��" << "\n";
				std::cout << "Number : " << pStudentInfo._number << "��" << "\n";

				// DB�� ��� �ϰ� UUID�� ���������
				if (_mainDB.EnrollStudentInfo(pStudentInfo._schoolID, pStudentInfo._grade, pStudentInfo._group, pStudentInfo._number) == 0)
				{
					//��� �Ϸ�
					int myUUID = 0;
					myUUID = _mainDB.SearchUUID(pStudentInfo._schoolID, pStudentInfo._grade, pStudentInfo._group, pStudentInfo._number);

					std::cout << "ã�ƿ� UUID : " << myUUID << "\n";

					if (myUUID > 0)
					{
						// UUID �������� ����
						std::cout << "This Student's UUID : " << myUUID << "\n";

						P_StudentUUID pStudentUUID;
						pStudentUUID._UUID = myUUID;

						PacketInfo temp;
						temp._id = 0;

						memset(temp._data, 0x0, sizeof(pStudentUUID));
						memcpy(temp._data, &pStudentUUID, sizeof(pStudentUUID));

						*((MainServer::P_StudentUUID*)temp._data) = pStudentUUID;

						temp._totalSize = sizeof(temp._data);

						_socketMgr.SendStudentUUID(0, pStudentUUID, packet._CastIdentifier());

						std::cout << "���� �ε��� �߰� ����2 : " << packet._CastIdentifier() << "\n";
					}
				}

				//int checkkey;
				//checkkey = pStudentInfo._schoolID + pStudentInfo._grade + pStudentInfo._group;
				//if (_classGroup.find(checkkey) != _classGroup.end())
				//{
				//	// exist

				//	vector<int>::iterator it;
				//	it = std::find(_classGroup[checkkey].begin(), _classGroup[checkkey].end(), pStudentInfo._number);

				//	if (it != _classGroup[checkkey].end())
				//	{
				//		std::cout << "Already Exist Same Student" << "\n";
				//	}
				//	else
				//	{
				//		_classGroup[checkkey].push_back(pStudentInfo._number);

				//		std::cout << "Success to Join Student" << "\n";
				//		std::cout << "���� Ŭ������ �л� �� : " << _classGroup[checkkey].size() << "\n";

				//		// ���� �Ŵ������� ������ map<int, SocketClass> �л� map<int, vector<SocketClass>> ���� �����ϴ°� ������

				//	}
				//}
				//else
				//{
				//	// not exist
				//	std::cout << "Fail to Join Student" << "\n";
				//}

				break;

			default:
				break;
			}
		}
		_mtx.unlock();
	}
}
