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
	int _myIndex;

public:
	SocketClass(SOCKET socket, int myindex)
	{
		_mySocket = socket;
		_myThread = nullptr;
		_uniqueIndex = 0;
		_myIndex = myindex;
	}

	void ExecuteThread(thread* myThread);

	SOCKET _MySocket() {
		return _mySocket;
	}

	int _UUID() {
		return _uniqueIndex;
	}
};
