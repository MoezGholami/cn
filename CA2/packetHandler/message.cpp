#include "message.h"

Message::Message(const string &val, Macaddr sadr, Macaddr recadr, uint8_t mn)
{
	value=val;
	senderadr=sadr;
	receiveradr=recadr;
	number=mn;
}

Message* generateResponse(const string &response, const Message &m)
{
	return new Message(response, m.receiveradr, m.senderadr, m.number);
}
