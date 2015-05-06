#include "switchCore.h"

SwitchCoreClerk::SwitchCoreClerk(int port_num){
	port = port_num;
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


void SwitchCoreClerk::forwardClientPacket(Packet comm, int fd){
	routingTable[comm.saddr] = fd;
	for(map<Macaddr,int>::iterator it=routingTable.begin();it!=routingTable.end();++it){
		if( it->first == comm.daddr ){
			int s = write(it->second, (char*)(&comm), sizeof(Packet));
			if(s < 0)
				cerr<<"send reply error\n";
			return;
		}
	}
	for(int i=0;i<fds.size();i++){	
		int s = write(fds[i], (char*)(&comm), sizeof(Packet));
			if(s < 0)
				cerr<<"send reply error\n";
	}
}





