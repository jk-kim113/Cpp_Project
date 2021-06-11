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