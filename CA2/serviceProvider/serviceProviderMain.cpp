#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include "../packetHandler/packetHandler.h"
#include "../packetHandler/message.h"
#include "../packetHandler/packet.h"
#include "serviceProviderCore.h"

using namespace std;

void clear_buff(char *x,size_t s){
	for(int i=0;i<(int)s;i++){
		x[i] = 0;
	}
}


#define STDIN 0
#define STDOUT 1
#define STR_SIZE 20480
int main(int argc, char* argv[])
{
	if(argc<2)
	{
		cerr<<"too few args, please enter port number as argument.\n";
		return -1;
	}
	int port_number=atoi(argv[1]);
	ServiceProviderCore core(port_number);
	const int num_of_connection = 4;
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port_number);

	cout<<"Listening from socket ...\n";

	//binding 
	int binding_st = bind(server_fd,(struct sockaddr*) &server_addr, sizeof(server_addr));
	if(binding_st == -1)
	{
		cerr<<"bind error\n";
		return -1;
	}
	//listenning
	int listening_st = listen(server_fd, num_of_connection);
	if(listening_st == -1)
	{
		cerr<<"listen error\n";
		return -2;
	}
	fd_set read_fdset, temp_fdset;
	struct timeval tv;
	int new_sock_fd, it_fd;

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&read_fdset);
	FD_SET(server_fd, &read_fdset);
	FD_SET(STDIN, &read_fdset);

	/* Wait up to five seconds. */
	tv.tv_sec = 10 * 60;
	tv.tv_usec = 0;

	int status;

	PacketHandler *php=0;
	Message *mes;

	while(1)
	{
		mes=0;
		if(php==0)
			php=new PacketHandler();
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
				if(it_fd == STDIN)
				{
					string throwaway;
					getline(cin,throwaway);
					cout<<"man faghat service provideram, behem chizi nagu\n";
					//read from stdin
				}
				else if(it_fd == server_fd)
				{	
					new_sock_fd = accept(server_fd, NULL, NULL);
					if(new_sock_fd < 0)
					{
						cerr<<"error on accepting.\n";
						return -4;
					}
					cout<<"new client"<<endl;
					FD_SET(new_sock_fd, &read_fdset);
				}
				else
				{
					int n;
					char buff_read [STR_SIZE], response_buff[STR_SIZE];
					clear_buff(buff_read, STR_SIZE);
					clear_buff(response_buff, STR_SIZE);
					Packet p;

					n = read(it_fd, &p, sizeof(Packet));
					if(n == 0)
					{
						close(it_fd);
						FD_CLR(it_fd, &read_fdset);
						cerr<<"client out"<<endl;
					}
					else if(n < 0)
					{
						cerr<<"Error reading"<<endl;
					}
					//after reading successfully
					else
					{
						mes=php->messageOfPackets(p);
						if(mes!=0)
						{
							cout<<"new query got.\n";
							Message *response=0;
							string clientInput = buff_read, serverReply;
							//receive from client, repeat it
							serverReply=core.doCommand(clientInput);
							response=generateResponse(serverReply, *mes);
							vector<Packet> packs=php->packetVectorOfMessage(*response);
							for(unsigned i=0; i<packs.size(); ++i)
							{
								int s = write(it_fd, &(packs[i]), sizeof(Packet));
								if(s < 0)
								{
									cerr<<"send reply error\n";
									return -1;
								}
							}
							delete response;
							delete php;
							php=0;
						}
					}
				}
			}
		}
	}


	close(server_fd);
	return 0;
}
