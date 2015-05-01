#include "message.h"

Message::Message(string val, Macaddr sadr, Macaddr recadr)
{
	value=val;
	senderadr=sadr;
	receiveradr=recadr;
}
