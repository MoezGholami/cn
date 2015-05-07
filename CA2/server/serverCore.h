#ifndef __siamoz_serverCore
#define __siamoz_serverCore

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include "../packetHandler/message.h"
#include "../packetHandler/packet.h"
#include "../packetHandler/packetHandler.h"

using namespace std;

#define STR_SIZE 20480

class ServerCore
{
	public:
		ServerCore();

		//returns fd of the just connected switch, else 0
		int doServerCommand();
		Message handleClientMessage(Message &m);
	protected:
};

class ServiceProviderConnection
{
	public:
		ServiceProviderConnection(int portnum);
		~ServiceProviderConnection();
		void close();

		bool isOkay();
		int getFD();
		string sendQuery(const string &q);

	protected:
		bool connectionOkay;
		int fd;
		char input_buffer[STR_SIZE];
		string serviceProviderIP;
};

void clear_buff(char *x,size_t s);

#endif
