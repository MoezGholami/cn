#ifndef __SIAMOZ__switchCore
#define __SIAMOZ__switchCore

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <map>

#include "packetHandler/packetHandler.h"


using namespace std;

class Macfd{
	public:
	Macfd(Macaddr mc,int filedis);
	Macaddr mac;
	int fd;
};

class SwitchCoreClerk{
	public:
	SwitchCoreClerk(int port_num);	
	int updateSourcePort(Packet comm,int fd);
	int getDestinationPort(Packet comm);
	void doServerCommand();
	void forwardClientPacket(Packet comm,int fd);
	void addFileDescriptor(int fd);
	PacketHandler ph;
	private:
	vector<int> fds;
	int port;
	vector< Macfd > routingTable;
};


#endif
