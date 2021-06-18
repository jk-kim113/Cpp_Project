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
	map<int, SocketClass*> _managerGroup;
	map<int, vector<SocketClass>> _studentGroup;

public:
	void AddSocket(SocketClass* socket);
	void AddSocket(string classInfo, SocketClass* socket);
	void SendToGroup(string classInfo);

	int _SocketCount() {
		return _socketGroup.size();
	}

	void AddManagerSocket(int classID, int castIdentifier);
	void AddStudentSocket(int classID, int castIdentifier);
};
