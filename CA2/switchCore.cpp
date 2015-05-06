#include "switchCore.h"

SwitchCoreClerk::SwitchCoreClerk(int port_num){
	port = port_num;
}

Macfd::Macfd(Macaddr mc,int filedis){
	mac = mc;
	fd = filedis;
}

void SwitchCoreClerk::doServerCommand(){
	string comm1,comm2;
	cin>>comm1;
	int portNu;
	if(comm1 == "Connect"){
		cin>>comm2;
		if(comm2 == "Switch"){
			int n, m;//return value of read/write calls
			string temp;
			int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			fds.push_back(fd);
			cin>>temp;
			string port_no = temp;
			struct sockaddr_in serv_addr;
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			serv_addr.sin_port = htons(atoi(port_no.c_str()));
			int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			//string chert;
			//getline(cin, chert);
			
			if(status < 0)
			{
				cerr<<"Conention failed\n";
			}
			else cerr<<"Successfully Connected\n";
						
		}
	}

}

int SwitchCoreClerk::updateSourcePort(Packet comm, int fd){
	for(int i=0;i<routingTable.size();i++){
		if(routingTable[i].mac == comm.saddr ){
			return i;
		}
	}
	routingTable.push_back(Macfd(comm.saddr,fd) );
	return routingTable.size()-1;
}

int SwitchCoreClerk::getDestinationPort(Packet comm){
	for(int i=0;i<routingTable.size();i++){
		if(routingTable[i].mac == comm.daddr){
			return routingTable[i].fd;
		}
	}
	return -1;
}

void SwitchCoreClerk::addFileDescriptor(int fd){
	fds.push_back(fd);
}


void SwitchCoreClerk::forwardClientPacket(Packet comm, int fd){
	//routingTable[comm.saddr] = fd;
	cout<<"forwarding packet..."<<endl;
	char x[24] = { 0 };
	memcpy(x,comm.data,23);
	string mes;
	mes = x;
	cout<<"pp packet: "<<mes<<endl;
	int sourcefd = updateSourcePort(comm,fd);
	int destfd = getDestinationPort(comm);
	if(destfd!=-1){
		int s = write(destfd, (char*)(&comm), sizeof(Packet));
		if(s < 0)
			cerr<<"send reply error\n";
		return;
	} else {
		for(int i=0;i<fds.size();i++){	
		if(fds[i]!=fd){
			int s = write(fds[i], (char*)(&comm), sizeof(Packet));
			if(s < 0)
				cerr<<"send reply error\n";
			}
		}
	}
}





