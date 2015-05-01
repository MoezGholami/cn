#ifndef __cn_ca2_siamoz_packet
#define __cn_ca2_siamoz_packet

#include <cstdint>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

class Macaddr
{
	public:
		uint8_t bytes[6];
		Macaddr(uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4, uint8_t a5, uint8_t a6);
		Macaddr(string s);
		Macaddr();
};

inline bool operator==(const Macaddr &a, const Macaddr &b);
inline bool operator!=(const Macaddr &a, const Macaddr &b);

//total size 72 bytes (regardless c mem management);
typedef struct Packet
{
	uint16_t type;
	Macaddr daddr;	//destination mac address
	Macaddr saddr;	// source mac address

	/*
	 * id for a packet for concatinating it,
	 * the most significant bit determins the last packet of message (if set).
	 * the id is unique for one pair of sender and receiver, not unique in all net
	 */
	uint32_t packid;

	uint8_t reserve[20];	//no valid data, reserved for future.
	uint32_t ttl;	//time to leave, 32 bit

	uint8_t length;	//length of the data

	uint8_t data[23];	//the main data

	uint8_t crc[6];		//crc check
} Packet;

#endif
