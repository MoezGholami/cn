#include "./packetHandler/message.h"
#include "./packetHandler/packetHandler.h"
#include "./packetHandler/packet.h"
#include "./util/util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

#define STR_SIZE 20480
const string adminPass = "1234";
const string adminName = "Admin";
const string serverMacAddr = "192:168:1:1:1:1";

void clear_buff(char *x,size_t s){
	for(int i=0;i<s;i++){
		x[i] = 0;
	}
}

int send_packet(string clientComm, int fd, string clientMacAddr,int gMID){
	int s;
	PacketHandler ph;
	vector<Packet> packets = ph.packetVectorOfMessage(Message(clientComm,clientMacAddr,serverMacAddr,gMID));
		cout<<"num of packets = "<<packets.size()<<endl;
		for(int i=0;i<packets.size();i++){
			s = write(fd, (char*)(&packets[i]), sizeof(Packet));
		}
	return s;
}

int recv_packet(char *serverReply, int fd){
	int n;
	PacketHandler ph;
	Packet recvPacket ;
	Message *mssg = NULL;
	while( mssg == NULL){
		n = read(fd, (char*)(&recvPacket), sizeof(Packet));
		mssg = ph.messageOfPackets(recvPacket);
		if( n<0 ){
			return n;
		}
		cerr<<"before"<<endl;
		cerr<<"after"<<endl;
	}
	if(mssg==NULL)
		cout<<"WTF"<<endl;
	cout<< &(mssg->value[0]);
	serverReply = &(mssg->value[0]);
	return n;
}

int main(int argn, char** args){
	int gMID=0;
	char input_buffer[STR_SIZE];
	clear_buff(input_buffer, STR_SIZE);
	string inputComm,origin,parse1,parse2,parse3;
	int temp;
	vector<string> participants;
	string clientMacAddr = args[1];

	while(1)
	{
		cin>>inputComm;
		//parsing input_buffer
		if(inputComm == "Connect")// don't forget to check reconnect!!
		{
			cin>>inputComm;
			if(inputComm!="Switch")
				continue;
			int n, m;//return value of read/write calls
			int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			cin>>temp;
			int port_no = temp;
			struct sockaddr_in serv_addr;
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			serv_addr.sin_port = htons(port_no);
			int status = connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
			//string chert;
			string chert;
			getline(cin, chert);
			
			if(status < 0)
			{
				cerr<<"Conention failed\n";
				continue;
			}
			else cerr<<"Successfully Connected\n";
			// test R&D
		/*	string qwe = "sending";
			int bytes_written = send_packet(qwe,fd);
						int bytes_written = send_packet(clientComm,fd,clientMacAddr,gMID);
			if(bytes_written < 0){
				cerr<<"Packet not sent"<<endl;
				continue;
			}
			string reply,clientComm;
			//get response from server
			char res_buff[STR_SIZE];
			clear_buff(res_buff, STR_SIZE);
			int read_status = recv_packet(res_buff, fd);
			reply = res_buff;
			cout<<res_buff<<endl;*/
			// Eof test R&D
			string clientComm,reply;
			while(1)
			{
				//read command
				stringstream ss;
				getline(cin,inputComm);
				origin = inputComm;
				ss << inputComm;
				ss >> parse1; //ssn
				if(parse1 == "Get"){
					ss >> parse1 >> parse2 >> parse3 ;
					if(parse1 == "List" && parse2 == "of" && parse3 == "Services"){
						clientComm = origin;
						int bytes_written = send_packet(clientComm,fd,clientMacAddr,gMID);
						if(bytes_written < 0){
						cerr<<"Packet not sent"<<endl;
						continue;
						}
						gMID++;
						if(gMID==256) gMID = 0;
						string reply;
						//get response from server
						char res_buff[STR_SIZE];
						clear_buff(res_buff, STR_SIZE);
						int read_status = recv_packet(res_buff,fd);
						reply = res_buff;
						if(read_status <0){
							cout<<"recieve failed"<<endl;
							continue;
						}
						//cout<<res_buff<<endl;
					}
				} else if(parse1 == "Login"){
					ss >> parse1 >> parse2;
					clientComm = origin;
					int bytes_written = send_packet(clientComm,fd,clientMacAddr,gMID);
					if(bytes_written < 0){
					cerr<<"Packet not sent"<<endl;
					continue;
					}
					gMID++;
					if(gMID==256) gMID = 0;
					string reply;
					//get response from server
					char res_buff[STR_SIZE];
					clear_buff(res_buff, STR_SIZE);
					int read_status = recv_packet(res_buff,fd);
					cerr<<"123"<<endl;
					reply = res_buff;
					if(read_status <0){
						cout<<"recieve failed"<<endl;
						continue;
					}
					//cout<<res_buff<<endl;
				} else if(parse1 == "Request" ){
					ss >> parse1 >> parse2 ;
					clientComm = origin;
					int bytes_written = send_packet(clientComm, fd, clientMacAddr,gMID);
					if(bytes_written < 0){
					cerr<<"Packet not sent"<<endl;
					continue;
					}
					gMID++;
					if(gMID==256) gMID = 0;
					string reply;
					//get response from server
					char res_buff[STR_SIZE];
					clear_buff(res_buff, STR_SIZE);
					int read_status = recv_packet(res_buff, fd);
					reply = res_buff;
					if(read_status <0){
						cout<<"recieve failed"<<endl;
						continue;
					}
					//cout<<res_buff<<endl;
	
				} else if(parse1 == "Send"){
					ss>>parse2;
					if( !FileExist(parse2) ){
						cout<<"The specifed file doesn't exist\n";
						continue;
					}
					string datax = wholeAsciiFile(parse2);
					clientComm = origin+"\n"+datax;
					int bytes_written = send_packet(clientComm, fd, clientMacAddr,gMID);
					if(bytes_written < 0){
					cerr<<"Packet not sent"<<endl;
					continue;
					}
					gMID++;
					if(gMID==256) gMID = 0;
					string reply;
					//get response from server
					char res_buff[STR_SIZE];
					clear_buff(res_buff, STR_SIZE);
					int read_status = recv_packet(res_buff, fd);
					reply = res_buff;
					if(read_status <0){
						cout<<"recieve failed"<<endl;
						continue;
					}

				} else if(parse1 == "Append"){
					ss>> parse1 >> parse2 ;
					clientComm = origin;
					int bytes_written = send_packet(clientComm, fd, clientMacAddr,gMID);
					if(bytes_written < 0){
					cerr<<"Packet not sent"<<endl;
					continue;
					}
					gMID++;
					if(gMID==256) gMID = 0;
					string reply;
					//get response from server
					char res_buff[STR_SIZE];
					clear_buff(res_buff, STR_SIZE);
					int read_status = recv_packet(res_buff, fd);
					reply = res_buff;
					if(read_status <0){
						cout<<"recieve failed"<<endl;
						continue;
					}

				} else if(parse1 == "Logout"){
					clientComm = origin;
					int bytes_written = send_packet(clientComm, fd, clientMacAddr,gMID);
					if(bytes_written < 0){
					cerr<<"Packet not sent"<<endl;
					continue;
					}
					gMID++;
					if(gMID==256) gMID = 0;
					string reply;
					//get response from server
					char res_buff[STR_SIZE];
					clear_buff(res_buff, STR_SIZE);
					int read_status = recv_packet(res_buff, fd);
					reply = res_buff;
					if(read_status <0){
						cout<<"recieve failed"<<endl;
						continue;
					}
				}
				else
				{
					cerr<<"invalid command\n";
					cout<<parse1<<endl;
				}
			}
			close(fd);
		} else {
			cout<<"A connection must be established\n";
		}
	}

	//free allocated memories
	return 0;
}
