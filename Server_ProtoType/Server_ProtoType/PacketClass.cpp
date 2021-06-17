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
