#include "SocketManager.h"

void SocketManager::AddSocket(SocketClass* socket)
{
	_socketGroup.push_back(socket);
}