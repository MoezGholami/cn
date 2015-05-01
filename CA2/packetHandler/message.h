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
		Message(string val, Macaddr sadr, Macaddr recadr);
};

#endif
