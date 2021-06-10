#pragma once
#include <vector>
#include "SocketClass.h"

class SocketManager
{
private:
	vector<SocketClass*> _socketGroup;

public:
	void AddSocket(SocketClass* socket);
};
