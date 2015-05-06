#include "message.h"

Message::Message(string val, Macaddr sadr, Macaddr recadr, uint8_t mn)
{
	value=val;
	senderadr=sadr;
	receiveradr=recadr;
	number=mn;
}
