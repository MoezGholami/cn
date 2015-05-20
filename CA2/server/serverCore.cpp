#include "serverCore.h"

ServerCore::ServerCore()
{
	SwitchConnection=0;
}

ServerCore::~ServerCore()
{
	deleteSwitchConnection();
	for(unsigned i=0; i<serviceProviderConnections.size(); ++i)
		if(serviceProviderConnections[i]!=0)
			delete serviceProviderConnections[i];
}

void ServerCore::removeCorruptedServiceProviders()
{
	for(unsigned i=0; i<serviceProviderConnections.size(); ++i)
		if(serviceProviderConnections[i]!=0 && !(serviceProviderConnections[i])->isOkay())
		{
			delete serviceProviderConnections[i];
			serviceProviderConnections[i]=0;
		}
	vector<Client_Like_Connection*>::iterator i = serviceProviderConnections.begin();
	while(i!=serviceProviderConnections.end())
		if(*i)
			++i;
		else
			i=serviceProviderConnections.erase(i);
}

int ServerCore::doServerCommand()
{
	string command, parse;
	stringstream ss;
	getline(cin, command);
	ss<<command;
	ss>>parse;
	if(parse!="Connect")
	{
		cout<<"input not recognized\n";
		return 0;
	}
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

Message* ServerCore::handleClientMessage(Message &m)
{
	if(m.receiveradr!=serverMacaddr)
		return 0;
	return handleOurMessage(m);
}

Message* ServerCore::handleOurMessage(const Message &m)
{
	string parse, parse2;
	stringstream ss;

	cout<<"client message:\n"<<m.value<<"\n\n";

	ss<<m.value;
	ss>>parse;
	if(parse=="Login")
		return handleLogin(m);
	if(parse=="Logout")
		return handleLogout(m);
	if(parse=="Get")
		return handleGetList(m);
	if(parse=="Request")
		return handleRequest(m);
	if(parse=="Append")
		return handleAppend(m);
	if(parse=="Send")
		return handleAppend(m);
		//the difference between appen and send is not in ServerCore, it is in ServiceProvider.
	cout << "no valid command got fron client. command = "<< m.value <<endl;
	return generateResponse("no valid command.",m);
}

Message* ServerCore::handleLogin(const Message &m)
{
	stringstream ss;
	string parse;
	ss<<m.value;
	ss>>parse>>parse;
	if(clientNames.find(m.senderadr)!=clientNames.end())
	{
		return generateResponse("already logged in as: "+clientNames[m.senderadr]+"\nplease log out.",m);
	}
	login(m.senderadr,parse);
	return generateResponse("successfully logged in as: "+clientNames[m.senderadr],m);
}

Message* ServerCore::handleLogout(const Message &m)
{
	if(clientNames.find(m.senderadr)==clientNames.end())
		return generateResponse("you have not logged in.",m);
	logout(m.senderadr);
	return generateResponse("logged out.", m);
}

Message* ServerCore::handleGetList(const Message &m)
{
	if(clientNames.find(m.senderadr)==clientNames.end())
		return generateResponse("you should login first.",m);

	string reply;
	for(unsigned i=0; i<serviceProviderConnections.size(); ++i)
		reply+=(serviceProviderConnections[i])->sendServiceProviderQuery(m.value);
	removeCorruptedServiceProviders();
	return generateResponse(reply,m);
}

Message* ServerCore::handleRequest(const Message &m)
{
	if(clientNames.find(m.senderadr)==clientNames.end())
		return generateResponse("you should login first.",m);

	string parse, fname, type;
	stringstream ss;

	ss<<m.value;
	ss>>parse>>fname>>type;
	if(type=="read")
	{
		Client_Like_Connection *provider=findProviderOf(fname);
		if(provider==0)
			return generateResponse("404 service not found :(", m);
		return generateResponse("request granted. here is the file content:\n\n"+
				provider->sendServiceProviderQuery("read "+fname),m);
	}
	if(type=="write")
	{
		Client_Like_Connection *provider=findProviderOf(fname);
		if(provider==0)
			return generateResponse("404 service not found :(",m);
		if(find(writableFiles[m.senderadr].begin(),
					writableFiles[m.senderadr].end(),
					fname)!=writableFiles[m.senderadr].end())
			return generateResponse("you already got the permission.",m);
		writableFiles[m.senderadr].push_back(fname);
		return generateResponse("successfully acquired permission.",m);
	}
	return generateResponse("only read & write .",m);
}

Message* ServerCore::handleAppend(const Message &m)
{
	if(clientNames.find(m.senderadr)==clientNames.end())
		return generateResponse("you should login first.",m);

	string parse, fname;
	stringstream ss;
	Client_Like_Connection *provider=0;

	ss<<m.value;
	ss>>parse>>fname>>parse;
	provider=findProviderOf(fname);

	if(provider==0)
		return generateResponse("404 service not found :(", m);
	if(find(writableFiles[m.senderadr].begin(), writableFiles[m.senderadr].end(),fname)==
			writableFiles[m.senderadr].end())
		return generateResponse("you have not permission to modify the file: "+fname+" .",m);
	return generateResponse(provider->sendServiceProviderQuery(m.value),m);
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

void ServerCore::login(const Macaddr &s, const string &name)
{
	if(clientNames.find(s)!=clientNames.end())
		return ;
	clientNames[s]=name;
	writableFiles[s]=vector<string> ();
}

void ServerCore::logout(const Macaddr &s)
{
	if(clientNames.find(s)==clientNames.end())
		return ;
	clientNames.erase(s);
	writableFiles.erase(s);
}

Client_Like_Connection* ServerCore::findProviderOf(const string &fname)
{
	Client_Like_Connection *result=0;
	for(unsigned i=0; i<serviceProviderConnections.size(); ++i)
		if(string::npos != ((serviceProviderConnections[i])
					->sendServiceProviderQuery("Get List Of Services")).find(fname+"\n"))
		{
			result=serviceProviderConnections[i];
			break;
		}
	removeCorruptedServiceProviders();
	return result;
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
		return "";
	
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
			return "";
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
			return "";
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
	for(int i=0;i<(int)s;i++){
		x[i] = 0;
	}
}
