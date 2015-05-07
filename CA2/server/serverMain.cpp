#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "serverCore.h"

using namespace std;

#define STR_SIZE 20480
#define STDIN	(0)

int main(int argc, char* argv[])
{
	fd_set read_fdset, temp_fdset;
	struct timeval tv;
	int status, it_fd, incommingFD=0;
	ServerCore core;
	PacketHandler ph;

	FD_ZERO(&read_fdset);
	FD_SET(STDIN,&read_fdset);
	tv.tv_sec = 10 * 60;
	tv.tv_usec = 0;
	
	while(true)
	{
		memcpy(&temp_fdset, &read_fdset, sizeof(temp_fdset));
		status = select(FD_SETSIZE, &temp_fdset, (fd_set *)0, (fd_set *)0, (struct timeval*) &tv);
		if(status < 1)
		{
			cerr<<"select error\n";
			return -3;

		}
		for(it_fd = 0; it_fd<FD_SETSIZE; it_fd++)
		{
			if(FD_ISSET(it_fd, &temp_fdset))
			{
				if(it_fd==STDIN)
				{
					incommingFD=core.doServerCommand();
					if(incommingFD)
						FD_SET(incommingFD, &read_fdset);
				}
				else if(it_fd==incommingFD)
				{
					Packet p;
					Message *m=0;
					int read_status = read(incommingFD, &p, sizeof(Packet));
					if(read_status < 0)
					{
						cout<<"could not receive anything from ServiceProvider"<<endl;
						return -2;
					}
					m=ph.messageOfPackets(p);
					if(m)
					{
						Message response=core.handleClientMessage(*m);
						vector<Packet> sendingpacks=ph.packetVectorOfMessage(response);
						for(unsigned i=0; i<sendingpacks.size(); ++i)
						{
							int bytes_written = write(incommingFD, &(sendingpacks[i]), sizeof(Packet));
							if(bytes_written < 0)
							{
								cout<<"could not send anything to ServiceProvider"<<endl;
								return -1;
							}
						}
					}
				}
			}
		}
	}

	return 0;
}
