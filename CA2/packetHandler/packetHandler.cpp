#include "packetHandler.h"

PacketHandler::PacketHandler()
{
}

vector<Packet> PacketHandler::packetVectorOfMessage(Message m)
{
	vector<Packet> result;
	int i;

	for(i=0; i<m.value.size()/23; ++i)
		result.push_back(packetOfMessage(Message(m.value.substr(23*i, 23), m.senderadr, m.receiveradr, m.number), 2*i));
	if(m.value.size()%23)
		result.push_back(packetOfMessage(Message(m.value.substr(23*i, m.value.size()%23), m.senderadr, m.receiveradr
							,m.number), 2*(i)));
	if(result.size()!=0)
		result[result.size()-1].packid=result[result.size()-1].packid + 1 ; //indicate last bit of message
	return result;
}

Packet PacketHandler::packetOfMessage(Message m, uint32_t packnumber)
{
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
	/*
	for(i=0; i<6; ++i)
		result.crc[i]=0;
	uint32_t *crcPos;
	crcPos=(uint32_t *)&(result.crc[0]);
	*crcPos=siamoz_crc32(&(result.data[0]),23);
	*/
	return result;
}

Message* PacketHandler::messageOfPackets(Packet p)
{
	vector<Packet>::iterator it;
	it=find_if(allPackets.begin(), allPackets.end(),PacketFinderWithCompleteID(p));
	if(it!=allPackets.end())
		return 0;	//return null if duplicate packet
	allPackets.push_back(p);
	return getMessageFromPacketVector(p);
}

Message* PacketHandler::getMessageFromPacketVector(const Packet &sample)
{
	string resultVal="";
	uint32_t i=0;
	vector<Packet>::iterator it;
	for(i=0; true; i+=2)
	{
		it=find_if(allPackets.begin(), allPackets.end(), PacketFinderWithCompleteID(sample.daddr,
							sample.saddr,*((uint8_t *)(&(sample.packid))), i));
		if(it==allPackets.end())
			break ;
		resultVal+=(char *)(it->data);
	}
	it=find_if(allPackets.begin(), allPackets.end(), PacketFinderWithCompleteID(sample.daddr,
						sample.saddr,*((uint8_t *)(&(sample.packid))), i+1));
	if(it==allPackets.end())
		return 0;
	resultVal+=(char *)(it->data);
	return new Message(resultVal, sample.saddr, sample.daddr, *((uint8_t *)(&(sample.packid))));
}

PacketFinderWithCompleteID::PacketFinderWithCompleteID(Macaddr r, Macaddr s, uint8_t mn, uint32_t pid)
{
	sender=s;
	receiver=r;
	packid=pid|((uint32_t)((mn)<<24));
};

PacketFinderWithCompleteID::PacketFinderWithCompleteID(const Packet &p)
{
	sender=p.saddr;
	receiver=p.daddr;
	packid=p.packid;
};

bool PacketFinderWithCompleteID::operator()(const Packet &p)
{
	if(p.daddr!=receiver)
		return false;
	if(p.saddr!=sender)
		return false;
	if(p.packid!=packid)
		return false;
	return true;
}
