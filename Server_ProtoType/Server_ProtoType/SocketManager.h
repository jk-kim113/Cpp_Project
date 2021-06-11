#pragma once
#include <vector>
#include "SocketClass.h"
#include <map>
#include <iostream>

class SocketManager
{
private:
	map<string, vector<SocketClass*>> _classGroup;
	vector<SocketClass*> _socketGroup;

public:
	void AddSocket(SocketClass* socket);
	void AddSocket(string classInfo, SocketClass* socket);
	void SendToGroup(string classInfo);
};
