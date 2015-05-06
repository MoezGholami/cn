#ifndef __cn_ca2_siamoz_packetHandler
#define __cn_ca2_siamoz_packetHandler

#include "packet.h"
#include "message.h"
#include <vector>
#include <algorithm>
//#include "crc.h"

using namespace std;

class PacketHandler
{
	public:
		PacketHandler();
		vector<Packet> packetVectorOfMessage(Message m);
		Message* messageOfPackets(Packet p);

	protected:
		Packet packetOfMessage(Message m, uint32_t packnumber);
		Message* getMessageFromPacketVector(const Packet &sample);
		vector<Packet> allPackets;
};

class PacketFinderWithCompleteID
{
	public:
		Macaddr sender;
		Macaddr receiver;
		uint32_t packid;
		PacketFinderWithCompleteID(Macaddr r, Macaddr s, uint8_t mn, uint32_t pid);
		PacketFinderWithCompleteID(const Packet &p);
		bool operator()(const Packet &p);
};

#endif
