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

	typedef struct PacketInfo {
		int _id;
		int _totalSize;
		char _data[1024];
	};

	typedef struct P_ClientInfo {
		int _schoolID;
		int _grade;
		int _group;
	};

	typedef struct P_StudentInfo {
		int _schoolID;
		int _grade;
		int _group;
		int _number;
	};

	typedef struct P_StudentUUID {
		int _UUID;
	};
};

