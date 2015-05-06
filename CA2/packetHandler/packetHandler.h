#ifndef __cn_ca2_siamoz_packetHandler
#define __cn_ca2_siamoz_packetHandler

#include "packet.h"
#include "message.h"
#include <vector>

using namespace std;

class PacketHandler
{
	public:
		PacketHandler();
		vector<Packet> packetsOfMessage(Message m, uint8_t messageNumber);
		Message messageOfPackets(vector<Packet> &packets);
};

#endif
