#pragma once
#include <WinSock2.h>
#include "SocketClass.h"

class MainServer
{
private:
	const short _port = 4578;
	SOCKET _waitServer;

	void CreateServer();
	void AcceptClient();
	void ListenClient(SocketClass* socketClass);
	void DoOrder();

public:
	MainServer()
	{
		CreateServer();
	}
};

