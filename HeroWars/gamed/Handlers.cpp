#include "PacketHandler.h"

bool PacketHandler::handleNull(HANDLE_ARGS)
{
	return true;
}

bool PacketHandler::handleKeyCheck(ENetPeer *peer, ENetPacket *packet)
{
	KeyCheck *keyCheck = (KeyCheck*)packet->data;

	uint8 test2[8] = {0x74, 0xF1, 0x18, 0x00, 0xF2, 0xB3, 0x67, 0x77};
	uint8 test[8] = {0xed, 0x8c, 0x4c, 0x74, 0x60, 0x99, 0x78, 0x7a};
	uint8 skeleton[8] = {0xed, 0x8c, 0xd9, 0x74, 0x62, 0xad, 0x4e, 0x56};

	uint64 userId = _blowfish->Decrypt(keyCheck->checkId);
	if(userId == keyCheck->userId)
	{
		PDEBUG_LOG_LINE(Log::getMainInstance()," User got the same key as i do, go on!\n");
		peerInfo(peer)->keyChecked = true;
		peerInfo(peer)->userId = userId;
	}
	else
	{
		Log::getMainInstance()->errorLine(" WRONG KEY, GTFO!!!\n");
		return false;
	}

	keyCheck->checkId = _blowfish->Encrypt(userId);


	//Send response as this is correct (OFC DO SOME ID CHECKS HERE!!!)
	KeyCheck response;
	response.userId = keyCheck->userId;
	//response.checkId = 5;
	memcpy(response.pCheckId(), skeleton, 8);

	//unk1 = 0xffffff makes it not work but this beneeth does not change anything...
	uint8 *key = _blowfish->getKey();
	response.header.type = key[1];
	response.header.unk1 = *(uint16*)&key[2];
	//response.unk1 = 1;
	
	return sendPacket(peer, reinterpret_cast<uint8*>(&response), sizeof(KeyCheck), 0); //channel 0 check
}

bool PacketHandler::handleGameNumber(ENetPeer *peer, ENetPacket *packet)
{
	WorldSendGameNumber world;
	world.gameId = 359250843;
	memcpy(world.data, "IMxHot", 6);

	return sendPacket(peer, reinterpret_cast<uint8*>(&world), sizeof(WorldSendGameNumber), 3); //channel 3 check
}

bool PacketHandler::handleSynch(ENetPeer *peer, ENetPacket *packet)
{
	SynchVersion *version = reinterpret_cast<SynchVersion*>(packet->data);
	Log::getMainInstance()->writeLine("Client version: %s\n", version->version);

	uint8 init[] = {0x58, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x71, 0xae, 0x33, 0x01, 0x00, 0x00, \
		0x00, 0x00, 0x1e, 0x00, 0x95, 0x86, 0x5e, 0x06, 0xa8, 0x6e, 0x49, 0x06, 0x00, 0x64, 0x00, 0x00, \
		0x00, 0x00, 0x18, 0x00, 0x50, 0x48, 0x64, 0x00, 0x5d, 0x48, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0xf5, 0xf8, 0xe6, 0x77, 0xc5, 0xd0, 0x4b, 0x77, 0xac, 0x02, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf5, 0xf8, 0xe6, 0x77, \
		0xc5, 0xd0, 0x4b, 0x77, 0xac, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0xa8, 0x6c, 0x18, 0x00, 0x90, 0x9f, 0x44, 0x05, 0x00, 0xfa, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x17, 0xd1, 0x4b, 0x77, 0x1a, 0xef, 0xa7, 0x22, 0x3c, 0x6d, 0x00, 0x00, \
		0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x76, \
		0x00, 0x6d, 0x18, 0x00, 0xe0, 0x5e, 0x4e, 0x77, 0x3a, 0xf5, 0xf1, 0x55, 0xfe, 0xff, 0xff, 0xff, \
		0x17, 0xd1, 0x4b, 0x77, 0x27, 0x3f, 0x99, 0x76, 0xac, 0x02, 0x00, 0x00, 0x90, 0x9f, 0x44, 0x05, \
		0x00, 0xfa, 0x00, 0x00, 0x3c, 0x6d, 0x18, 0x00, 0x2c, 0x3f, 0x99, 0x76, 0xd5, 0xee, 0xa7, 0x22, \
		0x20, 0x48, 0xe2, 0x74, 0xc0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0xff, 0xff, \
		0xe4, 0x6c, 0x18, 0x00, 0xb6, 0x51, 0xdc, 0x74, 0x88, 0x6d, 0x18, 0x00, 0x0e, 0x41, 0xa3, 0x76, \
		0xf5, 0xbc, 0x26, 0x54, 0xfe, 0xff, 0xff, 0xff, 0x2c, 0x3f, 0x99, 0x76, 0xb6, 0x51, 0xdc, 0x74, \
		0xac, 0x02, 0x00, 0x00, 0x90, 0x9f, 0x44, 0x05, 0x00, 0xfa, 0x00, 0x00, 0x3c, 0x6d, 0x18, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0xc0, 0x02, 0x00, 0x00, 0x20, 0x48, 0xe2, 0x74, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0xda, 0x74, \
		0xbc, 0x0c, 0x26, 0x03, 0x71, 0x56, 0xdc, 0x74, 0x0b, 0x00, 0x00, 0x00, 0x68, 0x56, 0xdc, 0x74, \
		0x0a, 0x8c, 0xb7, 0x22, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x1c, 0xe2, 0x74, 0x98, 0x00, 0x00, 0x00, \
		0x84, 0x6e, 0x18, 0x00, 0x38, 0x06, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0xa4, 0x6d, 0x18, 0x00, 0x75, 0x04, 0xde, 0x74, 0x9c, 0x6e, 0x18, 0x00, 0x00, 0xa0, 0x44, 0x05, \
		0x18, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xd8, 0x1c, 0xe2, 0x74, 0xd8, 0x6d, 0x18, 0x00, \
		0xd4, 0x5b, 0xdd, 0x74, 0x9c, 0x6e, 0x18, 0x00, 0xff, 0xff, 0xff, 0xff, 0x28, 0xa0, 0x44, 0x05, \
		0x18, 0x00, 0x00, 0x00, 0xf0, 0x0a, 0x2f, 0x05, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0a, 0x2f, 0x05, \
		0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x6d, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb2, 0x8f, 0xb7, 0x22, \
		0xf0, 0x0a, 0x2f, 0x05, 0xf0, 0x0a, 0x2f, 0x05, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, \
		0xf4, 0x6d, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00, 0xf0, 0xec, 0x18, 0x00, 0xed, 0x8c, 0xd9, 0x74, \
		0x52, 0xbc, 0x4e, 0x56, 0xfe, 0xff, 0xff, 0xff, 0x71, 0x5d, 0xdd, 0x74, 0xca, 0x5d, 0xdd, 0x74, \
		0x9c, 0x6e, 0x18, 0x00, 0xff, 0xff, 0xff, 0xff, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x48, 0x7e, 0x43, 0x05, 0x00, 0x00, 0x00, 0x00, 0x48, 0x7e, 0x43, 0x05, \
		0x01, 0x00, 0x00, 0x00, 0xd8, 0x1c, 0xe2, 0x74, 0x01, 0x00, 0x00, 0x00, 0x95, 0x95, 0x5e, 0x00, \
		0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0a, 0x2f, 0x05, 0x18, 0x97, 0x5d, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0xd4, 0xec, 0x18, 0x00, 0x48, 0x7e, 0x43, 0x00, 0x00, 0x00, 0x00, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0xf0, 0x0a, 0x2f, 0x05, \
		0x11, 0xfa, 0x37, 0x44, 0xc0, 0xca, 0x4b, 0x46, 0x4b, 0xca, 0xe5, 0x42, 0xe6, 0x13, 0x47, 0x46, \
		0x00, 0x7a, 0x1e, 0x42, 0xba, 0x7c, 0x1e, 0x42, 0x00, 0x7c, 0x1e, 0x42, 0x6f, 0x72, 0x64, 0x65, \
		0x72, 0x53, 0x68, 0x6f, 0x70, 0x30, 0x31, 0x00, 0x72, 0x33, 0x64, 0x32, 0x4d, 0x65, 0x73, 0x68, \
		0x02, 0x00, 0x02, 0x00, 0x5f, 0x5f, 0x50, 0x5f, 0x43, 0x68, 0x00, 0x6f, 0x73, 0x5f, 0x53, 0x70, \
		0x61, 0x77, 0x6e, 0x5f, 0x42, 0x61, 0x72, 0x72, 0x61, 0x63, 0x6b, 0x73, 0x5f, 0x5f, 0x52, 0x30, \
		0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, \
		0x64, 0x6f, 0x18, 0x00, 0x64, 0xfa, 0xe7, 0x77, 0x22, 0xf4, 0xe7, 0x77, 0x40, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x72, 0x18, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0xbb, 0xa7, 0xe8, 0x77, 0x18, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, \
		0xf4, 0x6f, 0x18, 0x00, 0xf4, 0x6f, 0x18, 0x00, 0xf4, 0x6f, 0x18, 0x00, 0x02, 0x00, 0x00, 0x00, \
		0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x64, 0xfa, 0xe7, 0x77, 0x64, 0xfa, 0xe7, 0x77, 0x58, 0x71, 0x18, 0x00, 0xe1, 0xf1, 0xe7, 0x77, \
		0x34, 0x70, 0x18, 0x00, 0xe8, 0x71, 0x18, 0x00, 0x00, 0x73, 0x18, 0x00, 0x4c, 0xf2, 0xe7, 0x77, \
		0x64, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x73, 0x00, 0x00, 0x00, 0x80, 0x00, 0xb4, 0x70, 0x18, 0x00, \
		0x53, 0x00, 0x78, 0x00, 0x64, 0xfa, 0xe7, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x34, 0x71, 0x18, 0x00, \
		0x34, 0x71, 0x18, 0x00, 0x34, 0x71, 0x18, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, \
		0x64, 0x00, 0x30, 0x00, 0x38, 0x00, 0x63, 0x00, 0x63, 0x00, 0x30, 0x00, 0x08, 0x00, 0x00, 0x00, \
		0xa8, 0xed, 0xc4, 0x02, 0x00, 0x00, 0xc4, 0x02, 0x20, 0xee, 0xc4, 0x02, 0x66, 0x00, 0x63, 0x00, \
		0x5c, 0x00, 0x4d, 0x00, 0x53, 0x00, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, \
		0xc4, 0x71, 0x18, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x02, 0xa8, 0xed, 0xc4, 0x02, \
		0xc4, 0x71, 0x18, 0x00, 0x74, 0x3c, 0xe8, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, \
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x8c, 0x23, 0xc4, 0x02, 0x80, 0x48, 0xf5, 0x77, \
		0x38, 0x71, 0x18, 0x00, 0xf9, 0xf9, 0xe7, 0x77, 0x20, 0xe5, 0xc4, 0x02, 0x7c, 0x23, 0xc4, 0x02, \
		0x20, 0x3e, 0xff, 0xff, 0xf0, 0x71, 0x18, 0x00, 0xa0, 0x01, 0xf5, 0x77, 0x00, 0x00, 0x00, 0x00, \
		0xa4, 0x71, 0x18, 0x00, 0x6e, 0xfe, 0xe7, 0x77, 0xf0, 0x71, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0xf8, 0x71, 0x18, 0x00, 0x64, 0xfa, 0x00, 0x77, 0x2e, 0x00, 0x00, 0x00, 0x08, 0xe5, 0xc4, 0x02, \
		0x98, 0x71, 0x18, 0x00, 0xd6, 0xfa, 0xe7, 0x77, 0x01, 0x00, 0x00, 0x00, 0xac, 0x73, 0x18, 0x00, \
		0x64, 0xfa, 0xe7, 0x77, 0xf0, 0x71, 0x18, 0x00, 0xe0, 0x71, 0x18, 0x00, 0xfc, 0x71, 0x18, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x71, 0x18, 0x00, \
		0xac, 0x73, 0x18, 0x00, 0x64, 0xfa, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x56, 0x65, \
		0x72, 0x73, 0x69, 0x6f, 0x6e, 0x20, 0x31, 0x2e, 0x30, 0x2e, 0x30, 0x2e, 0x31, 0x33, 0x36, 0x20, \
		0x5b, 0x50, 0x55, 0x42, 0x4c, 0x49, 0x43, 0x5d, 0x00, 0x01, 0xf5, 0x77, 0x92, 0xe1, 0xe7, 0x77, \
		0x58, 0x00, 0xe8, 0x77, 0xd0, 0xa1, 0xb0, 0x10, 0x01, 0x00, 0x00, 0x00, 0xf7, 0xfc, 0xe7, 0x77, \
		0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x74, 0x73, 0x18, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x01, 0x00, 0x00, 0x00, 0xa0, 0x01, 0xf5, 0x77, 0x18, 0x00, 0x1a, 0x00, 0x08, 0xe5, 0xc4, 0x02, \
		0x50, 0x23, 0xc4, 0x02, 0xf0, 0x71, 0x18, 0x00, 0x85, 0x48, 0xe8, 0x01, 0x00, 0x00, 0xf5, 0x77, \
		0x00, 0x00, 0x00, 0x00, 0xb0, 0xed, 0xc4, 0x02, 0x00, 0x00, 0x00, 0x00, 0xfb, 0x01, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x44, 0x72, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x0b, 0x00, 0x00, \
		0x3c, 0x72, 0x01, 0x01, 0x37, 0x04, 0xb2, 0x22, 0x2c, 0x76, 0x18, 0x00, 0x50, 0x48, 0x64, 0x00, \
		0x5d, 0x48, 0x64, 0x00, 0x50, 0x72, 0x18, 0x00, 0x54, 0x2c, 0xe8, 0x77, 0xcc, 0xed, 0xc4, 0x02, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x72, 0x18, 0x00, 0x59, 0x92, 0x4c, 0x77, \
		0xcc, 0xed, 0xc4, 0x02, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x75, 0x18, 0x00, 0x50, 0x75, 0x18, 0x00, \
		0x69, 0x9a, 0x4c, 0x77, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa4, 0x9a, 0x4c, 0x77, 0x18, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0xd8, 0x72, 0x18, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x4c, \
		0x41, 0x53, 0x53, 0x49, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xe2, \
		0xa0};
	return sendPacket(peer, reinterpret_cast<uint8*>(init), sizeof(init), 3); //channel 3 check
}

bool PacketHandler::handleMap(ENetPeer *peer, ENetPacket *packet)
{
	uint8 spawnStart[] = {0x67, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x17, 0x87, 0xb2, 0x22, \
		0x71, 0xae, 0x33, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0xb0, 0x4b, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x71, 0xae, 0x33, 0x01, 0x00, 0x00, 0x00, 0x00, \
		0xed, 0x8c, 0xd9, 0x74, 0x62, 0xad, 0x4e, 0x56, 0xfe, 0xff, 0xff, 0xff, 0x3c, 0xf1, 0x18, 0x00, \
		0xb2, 0x0c, 0x6a, 0x00, 0xf0, 0xf5, 0x18, 0x00, 0xed, 0x8c, 0xd9, 0x74, 0xf2, 0xad, 0x4e, 0x56, \
		0xfe, 0xff, 0xff, 0xff, 0x58, 0x4c, 0xd9, 0x74, 0xa0, 0x0c, 0x6a, 0x00, 0x38, 0xe0, 0xb3, 0x05, \
		0x70, 0x76, 0x64, 0x03, 0x8f, 0x0e, 0x5e, 0x67, 0x8f, 0x0e, 0x5e, 0x67, 0x0c, 0x27, 0x3e, 0x75, \
		0x9d, 0x6f, 0x3b, 0x04, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x70, 0x76, 0x64, 0x03, \
		0x9e, 0xbc, 0x5b, 0x00, 0x9c, 0xf7, 0x18, 0x00, 0xdd, 0xbd, 0x5b, 0x00, 0x20, 0x74, 0x6f, 0x20, \
		0xf1, 0x1a, 0x00, 0x00, 0x6d, 0x61, 0x70, 0x70, 0x69, 0x6e, 0x67, 0x73, 0x20, 0x3a, 0x2c, 0x20, \
		0x30, 0x2c, 0x20, 0x31, 0x00, 0x39, 0x2c, 0x20, 0x74, 0x6f, 0x20, 0x65, 0x6e, 0x65, 0x74, 0x20, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
		0x02, 0xc7, 0xda, 0x74, 0x50, 0xf2, 0x18, 0x00, 0xb8, 0x6d, 0x5f, 0x03, 0xa8, 0x15, 0x26, 0x03, \
		0x2f, 0x2e, 0xd9, 0x74, 0x00, 0xf2, 0x18, 0x00, 0xb4, 0xf2, 0x18, 0x00, 0x64, 0x1a, 0xda, 0x74, \
		0x0a, 0x00, 0x00, 0x00, 0x91, 0x1a, 0xda, 0x74, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1b, 0xe2, 0x74, \
		0x63, 0x20, 0xda, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x02, \
		0x00, 0x00, 0xc4, 0x02, 0xe8, 0xff, 0xc6, 0x02, 0x00, 0x00, 0x00, 0x00, 0xe8, 0xff, 0xc6, 0x02, \
		0x74, 0xf2, 0x18, 0x00, 0xf2, 0xb3, 0xe8, 0x77, 0xe8, 0xff, 0xc6, 0x02, 0xe0, 0xff, 0xc6, 0x02, \
		0x00, 0x00, 0xc4, 0x02, 0x75, 0xb3, 0xe8, 0x77, 0xe8, 0xef, 0xc6, 0x02, 0x00, 0x00, 0xc4, 0x02, \
		0xe0, 0xef, 0xc6, 0x02, 0xa8, 0xf2, 0x18, 0x00, 0x00, 0xe2, 0xc6, 0x02, 0x00, 0x00, 0xc4, 0x02, \
		0x00, 0x00, 0x00, 0x00, 0xe0, 0x19, 0x00, 0x00, 0xbc, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x02, \
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	SpawnPacket *spawnName = SpawnPacket::create(PKT_LoadName, peerInfo(peer)->name, peerInfo(peer)->nameLen);
	spawnName->userId = peerInfo(peer)->userId;

	SpawnPacket *spawnType = SpawnPacket::create(PKT_LoadType,  peerInfo(peer)->type, peerInfo(peer)->typeLen);
	spawnType->userId = peerInfo(peer)->userId;

	bool p1 = sendPacket(peer, reinterpret_cast<uint8*>(spawnStart), sizeof(spawnStart), 6);
	bool p2 = sendPacket(peer, reinterpret_cast<uint8*>(spawnName), spawnName->getPacketLength(), 6);
	bool p3 = sendPacket(peer, reinterpret_cast<uint8*>(spawnType), spawnType->getPacketLength(), 6);

	return (p1 && p2 && p3);

}

bool PacketHandler::handleSpawn(ENetPeer *peer, ENetPacket *packet)
{

	return true;
}

bool PacketHandler::handleLoadPing(ENetPeer *peer, ENetPacket *packet)
{
	PingLoadInfo *loadInfo = reinterpret_cast<PingLoadInfo*>(packet->data);

	PingLoadInfo response;
	memcpy(&response, packet->data, sizeof(PingLoadInfo));
	response.header.cmd = PKT_S2C_Ping_Load_Info;
	response.userId = peerInfo(peer)->userId;


	Log::getMainInstance()->writeLine("Loading: loaded: %f, ping: %f, %i, %f\n", loadInfo->loaded, loadInfo->ping, loadInfo->unk4, loadInfo->f3);

	//response.loaded = 50.0;
	//response.ping = 10.0;
	//response.unk4 = 66;

	//Should be of course broadcast
	
	//sendPacket(peer, 4, (uint8*)&response, sizeof(PingLoadInfo), UNRELIABLE);
	return broadcastPacket(reinterpret_cast<uint8*>(&response), sizeof(PingLoadInfo), 4, 0);
}

bool PacketHandler::handleQueryStatus(HANDLE_ARGS)
{
	QueryStatus response;
	return sendPacket(peer, reinterpret_cast<uint8*>(&response), sizeof(QueryStatus), 3);
}