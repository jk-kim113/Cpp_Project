#pragma once
#include <WinSock2.h>

class MainServer
{
private:
	const short _port = 4578;
	SOCKET _waitServer;

	void CreateServer();
	void AcceptClient();
	void ListenClient(SOCKET client);
	void DoOrder();

public:
	MainServer()
	{
		CreateServer();
	}
};

