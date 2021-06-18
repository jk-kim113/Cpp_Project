#include "SocketClass.h"
#include <thread>

void SocketClass::ExecuteThread(thread* myThread)
{
	_myThread = myThread;
}
