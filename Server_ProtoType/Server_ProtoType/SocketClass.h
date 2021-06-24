#pragma once
#include <WinSock2.h>
#include <thread>
#include <queue>
#include <mutex>

using namespace std;

class SocketClass
{
private:
	SOCKET _mySocket;
	thread* _myThread;
	long _uniqueIndex;
	int _myIndex;
	queue<char*> _toClientQueue;
	mutex _mtx;
	thread* _sendOrder;

	void SendOrder();

public:
	SocketClass(SOCKET socket, int myindex)
	{
		_mySocket = socket;
		_myThread = nullptr;
		_uniqueIndex = 0;
		_myIndex = myindex;

		_sendOrder = new thread(&SocketClass::SendOrder, this);
		//thread sendOrder(&SocketClass::SendOrder, this);
	}

	void ExecuteThread(thread* myThread);
	void AddSendData(char* data);

	SOCKET _MySocket() {
		return _mySocket;
	}

	int _UUID() {
		return _uniqueIndex;
	}

	int _MyIndex() {
		return _myIndex;
	}
};
