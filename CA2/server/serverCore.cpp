#include "serverCore.h"

ServerCore::ServerCore()
{
	SwitchConnection=0;
}

ServerCore::~ServerCore()
{
	deleteSwitchConnection();
	for(unsigned i=0; i<serviceProviderConnections.size(); ++i)
		delete serviceProviderConnections[i];
}

int ServerCore::doServerCommand()
{
	string command, parse;
	stringstream ss;
	getline(cin, command);
	ss<<command;
	ss>>parse;
	if(parse!="Connect")
		return 0;
	ss>>parse;
	if(parse=="Switch")
	{
		int pn;
		ss>>pn;
		connectToSwitch(pn);
		if(SwitchConnection!=0 && SwitchConnection->isOkay())
			return SwitchConnection->getFD();
		else
			return 0;
	}
	else if(parse=="Service")
	{
		ss>>parse;
		int pn;
		ss>>pn;
		if(find_if(serviceProviderConnections.begin(), serviceProviderConnections.end(),
					Client_Like_ConnectionFinderByPort(pn))!=serviceProviderConnections.end())
			cout<<"already connected.\n";
		else
		{
			Client_Like_Connection *cl=new Client_Like_Connection(pn);
			if(cl->isOkay())
				serviceProviderConnections.push_back(cl);
			else
				delete cl;
		}
	}
	else
	{
		cout<<"input not recognized\n";
	}
	return 0;
}

void ServerCore::connectToSwitch(int pn)
{
	if(SwitchConnection!=0)
	{
		cout<<"already connected to a switch.\n";
		return ;
	}
	SwitchConnection=new Client_Like_Connection(pn);
	if(!SwitchConnection->isOkay())
		deleteSwitchConnection();
}

void ServerCore::deleteSwitchConnection()
{
	if(SwitchConnection)
		delete SwitchConnection;
	SwitchConnection=0;
}

Client_Like_Connection::Client_Like_Connection(int portnum)
{
	connectionOkay=false;
	serviceProviderIP="127.0.0.1";

	port=portnum;
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

Client_Like_Connection::~Client_Like_Connection()
{
	close();
}

void Client_Like_Connection::close()
{
	if(!connectionOkay)
		return ;
	connectionOkay=false;
	::close(fd);
}

int Client_Like_Connection::getFD() const
{
	return fd;
}

int Client_Like_Connection::getPort() const
{
	return port;
}

bool Client_Like_Connection::isOkay() const
{
	return connectionOkay;
}

string Client_Like_Connection::sendServiceProviderQuery(const string &q)
{
	if(!isOkay())
		return "connection is corrupted.";
	
	Message sending(q,Macaddr("0.0.0.0.0.0"),Macaddr("0.0.0.0.0.0"),0);
	PacketHandler ph;
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

Client_Like_ConnectionFinderByPort::Client_Like_ConnectionFinderByPort(int p)
{
	port=p;
}

bool Client_Like_ConnectionFinderByPort::operator()(Client_Like_Connection *cl)
{
	return port==cl->getPort();
}

void clear_buff(char *x,size_t s){
	for(int i=0;i<s;i++){
		x[i] = 0;
	}
}
