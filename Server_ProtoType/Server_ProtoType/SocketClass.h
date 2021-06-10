#pragma once
#include <WinSock2.h>
#include <thread>

using namespace std;

class SocketClass
{
private:
	SOCKET _mySocket;
	thread* _myThread;
	long _uniqueIndex;

public:
	SocketClass(SOCKET socket, thread* thread)
	{
		_mySocket = socket;
		_myThread = thread;
		_uniqueIndex = 0;
	}
};
