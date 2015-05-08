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
#include <sstream>
#include <algorithm>
#include "../packetHandler/message.h"
#include "../packetHandler/packet.h"
#include "../packetHandler/packetHandler.h"

using namespace std;

#define STR_SIZE 20480
const Macaddr serverMacaddr(192,168,1,1,1,1);

class Client_Like_Connection
{
	public:
		Client_Like_Connection(int portnum);
		~Client_Like_Connection();
		void close();

		bool isOkay()	const;
		int getFD()	const;
		int getPort()	const;
		string sendServiceProviderQuery(const string &q);

	protected:
		bool connectionOkay;
		int fd;
		int port;
		char input_buffer[STR_SIZE];
		string serviceProviderIP;
};

class ServerCore
{
	public:
		ServerCore();
		~ServerCore();

		//returns fd of the just connected switch, else 0
		int doServerCommand();
		//returns the response of message iff the receiver is the server
		//else returns 0, caution: the message * should be deleted after using
		Message *handleClientMessage(Message &m);
	protected:
		Client_Like_Connection *SwitchConnection;
		vector<Client_Like_Connection *> serviceProviderConnections;

		void deleteSwitchConnection();
		void connectToSwitch(int pn);
};

class Client_Like_ConnectionFinderByPort
{
	public:
		Client_Like_ConnectionFinderByPort(int p);
		bool operator()(Client_Like_Connection *cl);
	protected:
		int port;
};

void clear_buff(char *x,size_t s);

#endif
