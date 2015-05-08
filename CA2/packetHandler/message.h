#ifndef __cn_ca2_siamoz_message
#define __cn_ca2_siamoz_message

#include "packet.h"
#include <string>

using namespace std;

class Message
{
	public:
		Macaddr senderadr;
		Macaddr receiveradr;
		string value;
		uint8_t number;
		Message(const string &val, Macaddr sadr, Macaddr recadr, uint8_t mn);
};

//caution: the result shoud be deleted after using.
Message* generateResponse(const string &response, const Message &m);

#endif
