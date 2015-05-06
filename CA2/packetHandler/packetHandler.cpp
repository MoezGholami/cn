#include "packetHandler.h"

PacketHandler::PacketHandler()
{
}

vector<Packet> PacketHandler::packetVectorOfMessage(Message m, uint8_t messageNumber)
{
	vector<Packet> result;
	int i;

	for(i=0; i<m.value.size()/23; ++i)
		result.push_back(packetOfMessage(Message(m.value.substr(23*i, 23), m.senderadr, m.receiveradr, m.number), 2*i));
	if(m.value.size()%23)
		result.push_back(packetOfMessage(Message(m.value.substr(23*i, m.value.size()%23), m.senderadr, m.receiveradr
							,m.number), 2*(i+1)));
	if(result.size()!=0)
		result[result.size()-1].packid=result[result.size()-1].packid + 1 ; //indicate last bit of message
	return result;
}

Packet PacketHandler::packetOfMessage(Message m, uint32_t packnumber)
{
	uint32_t *crcPos;
	Packet result;
	unsigned i=0;

	result.type=0;
	result.daddr=m.receiveradr;
	result.saddr=m.senderadr;
	result.packid=(packnumber)|((uint32_t)((m.number)<<24));
	result.ttl=10;
	result.length=m.value.size()>23?23:m.value.size();
	for(i=0; i<m.value.size() && i<23; ++i)
		result.data[i]=(uint8_t)m.value[i];
	for(; i<23; ++i)
		result.data[i]=0;
	for(i=0; i<6; ++i)
		result.crc[i]=0;
	crcPos=(uint32_t *)&(result.crc[0]);
	*crcPos=siamoz_crc32(siamoz_crc32_tab[15],&(result.data[0]),23);
	return result;
}

Message* PacketHandler::messageOfPackets(vector<Packet> &packets)
{
	return 0;
}
