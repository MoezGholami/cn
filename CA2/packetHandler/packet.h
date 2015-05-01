#ifndef __cn_ca2_siamoz_packet
#define __cn_ca2_siamoz_packet

#include <cstdint>

typedef struct _mac
{
	uint8_t bytes[6];
} macaddr;

//total size 72 bytes (regardless c mem management);
typedef struct _packet
{
	uint16_t type;
	uint8_t daddr[6];	//destination mac address
	uint8_t saddr[6];	// source mac address

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
