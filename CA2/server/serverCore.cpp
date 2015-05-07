#include "serverCore.h"

ServiceProviderConnection::ServiceProviderConnection(int portnum)
{
	connectionOkay=false;
	serviceProviderIP="127.0.0.1";

	clear_buff(input_buffer, STR_SIZE);
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serviceProviderIP.c_str());
	serv_addr.sin_port = htons(portnum);
	int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

	if(status < 0)
		cout<<"Conention failed\n";
	else
	{
		cout<<"Successfully Connected\n";
		connectionOkay=true;
	}
}

ServiceProviderConnection::~ServiceProviderConnection()
{
	close();
}

void ServiceProviderConnection::close()
{
	if(!connectionOkay)
		return ;
	connectionOkay=false;
	close(fd);
}

int ServiceProviderConnection::getFD()
{
	return fd;
}

bool ServiceProviderConnection::isOkay()
{
	return connectionOkay;
}

string ServiceProviderConnection::sendQuery(const string &q)
{
	if(!isOkay())
		return "connection is corrupted.";
	
	Message sending(q,Macaddr("0.0.0.0.0.0"),Macaddr("0.0.0.0.0.0"),0);
	packetHandler ph;
	vector<Packet> sendingpacks=ph.packetVectorOfMessage(sending);
	for(unsigned i=0; i<sendingpacks.size(); ++i)
	{
		int bytes_written = write(fd, &(sendingpacks[i]), sizeof(Packet));
		if(bytes_written < 0)
		{
			cout<<"could not send anything to ServiceProvider"<<endl;
			close();
			return "connection is corrupted.";
		}
	}
	//get response from server
	Packet p;
	Message *m=0;
	while(!m)
	{
		int read_status = read(fd, &p, sizeof(Packet));
		if(read_status < 0)
		{
			cout<<"could not receive anything from ServiceProvider"<<endl;
			close();
			return "connection is corrupted.";
		}
		m=ph.messageOfPackets(p);
	}
	return m->value;
}

void clear_buff(char *x,size_t s){
	for(int i=0;i<s;i++){
		x[i] = 0;
	}
}
