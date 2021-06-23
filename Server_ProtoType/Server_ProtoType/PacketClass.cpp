#include "PacketClass.h"

void PacketClass::AccessServer(int protocolID, char* data, int dataSize, int castIdentifier)
{
	_protocolID = protocolID;
	_data = data;
	_dataSize = dataSize;
	_castIdentifier = castIdentifier;
}

void PacketClass::EnrollUUID(int protocolID, char* data, int dataSize, int castIdentifier, int uuid)
{
	_protocolID = protocolID;
	_data = data;
	_dataSize = dataSize;
	_castIdentifier = castIdentifier;
}

void PacketClass::CreatePacket(int toClientID, struct MainServer::P_StudentUUID p, int castIdentifier)
{
	MainServer::PacketInfo packet;
	packet._id = toClientID;

	memset(packet._data, 0x0, sizeof(p));
	memcpy(packet._data, &p, sizeof(p));

	*((MainServer::P_StudentUUID*)packet._data) = p;

	packet._totalSize = sizeof(packet._data);

	memset(_dataPacket, 0x0, sizeof(packet));
	memcpy(_dataPacket, &packet, sizeof(packet));

	*((MainServer::PacketInfo*)_dataPacket) = packet;
}
