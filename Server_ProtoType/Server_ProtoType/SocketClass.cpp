#include "SocketClass.h"

void SocketClass::ExecuteThread(thread* myThread)
{
	_myThread = myThread;
}

void SocketClass::SendOrder()
{
	while (true)
	{
		_mtx.lock();
		if (!_toClientQueue.empty())
		{
			char* data = _toClientQueue.front();
			_toClientQueue.pop();
			
			send(_mySocket, data, sizeof(data), 0);
			printf("������ Ŭ���̾�Ʈ���� ������");
		}
		_mtx.unlock();
	}
}

void SocketClass::AddSendData(char* data)
{
	_toClientQueue.push(data);
}