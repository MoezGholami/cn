#ifndef __cn_ca2_siamoz_packet
#define __cn_ca2_siamoz_packet

#include <stdint.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>

using namespace std;

class Macaddr
{
	public:
		//little endian address
		uint8_t bytes[6];
		Macaddr(uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4, uint8_t a5, uint8_t a6);
		Macaddr(string s);
		Macaddr();

		bool operator==(const Macaddr &b) const;
		bool operator!=(const Macaddr &b) const;
		bool operator<(const Macaddr &b)  const;
};

ostream& operator<<(ostream &out, const Macaddr &ma);

//total size 72 bytes (regardless c mem management);
typedef struct Packet
{
	uint16_t type;
	Macaddr daddr;	//destination mac address
	Macaddr saddr;	// source mac address

	/*
	 * id for a packet for concatinating it,
	 * the least significant bit determins the last packet of message (if set).
	 * last right bits indicate the packet numbers.
	 * 8 left most bits indicates the package number of pair
	 * the id is unique for one pair of sender and receiver, not unique in all net
	 */
	uint32_t packid;

	uint8_t reserve[20];	//no valid data, reserved for future.
	uint32_t ttl;	//time to leave, 32 bit

	uint8_t length;	//length of the data

	uint8_t data[23];	//the main data

	uint8_t crc[6];		//crc check
} Packet;

ostream& operator<<(ostream &out, const Packet &p);

#endif
