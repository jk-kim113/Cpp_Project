#include "SocketManager.h"

void SocketManager::AddSocket(SocketClass* socket)
{
	_socketGroup.push_back(socket);
}

void SocketManager::AddSocket(string classInfo, SocketClass* socket)
{
	_classGroup[classInfo].push_back(socket);
}

void SocketManager::SendToGroup(string classInfo)
{
	vector<SocketClass*>::iterator it;
	for (it = _classGroup[classInfo].begin(); it != _classGroup[classInfo].end(); it++)
	{
		//send(it, )
	}
}

void SocketManager::AddManagerSocket(int classID, int castIdentifier)
{
	for (int n = 0; n < _socketGroup.size(); n++)
	{
		if (_socketGroup[n]->_UUID() == 0) // ���� �ε����� ��
		{
			// ���⼭ ������ �Ŵ��� ���Ϳ� �߰�
			// ���ÿ� �л� �׷� �ʱ�ȭ

			if (_managerGroup.find(classID) != _managerGroup.end())
			{
				// exist
				std::cout << "There is Group" << "\n";
			}
			else
			{
				// not exist
				std::cout << "There is no Group" << "\n" << "Success to Create Group" << "\n";
			}

			break;
		}
	}
}

void SocketManager::AddStudentSocket(int classID, int castIdentifier)
{
	for (int n = 0; n < _studentGroup.size(); n++)
	{
		
	}
}

PacketClass SocketManager::SendStudentUUID(int toClientID, struct MainServer::P_StudentUUID p, int castIdentifier)
{
	PacketClass packet;
	packet.CreatePacket(toClientID, p, castIdentifier);

	return packet;
}

void SocketManager::Send(char* data, int size, int castID)
{
	for (int n = 0; n < _socketGroup.size(); n++)
	{
		if (_socketGroup[n]->_MyIndex() == castID)
		{
			send(_socketGroup[n]->_MySocket(), data, 1032, 0);
			return;
		}
	}
}
