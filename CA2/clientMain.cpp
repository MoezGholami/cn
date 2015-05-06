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


void clear_buff(char *x,size_t s){
	for(int i=0;i<s;i++){
		x[i] = 0;
	}
}

int send_packet(string x, int fd){
	return write(fd, x.c_str(), x.size());
}

int recv_packet(char *x, int fd){
	return read(fd, x , STR_SIZE);
}

int main(int argn, char** args){

	char input_buffer[STR_SIZE];
	clear_buff(input_buffer, STR_SIZE);
	string inputComm,origin,parse1,parse2,parse3;
	int temp;
	vector<string> participants;

	while(cin>>inputComm)
	{
		/*if(inputComm != "Login"){
			cout<<"Please loggin first ...\n";
			continue;
		} else {
			cin>>inputComm>>inputComm;
		}
		cin>>inputComm;*/
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
			//getline(cin, chert);
			
			if(status < 0)
			{
				cerr<<"Conention failed\n";
				continue;
			}
			else cerr<<"Successfully Connected\n";
			// test R&D
			string qwe = "sending";
			int bytes_written = send_packet(qwe,fd);
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
			cout<<res_buff<<endl;
			// Eof test R&D
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
						clientComm = "GETSERVICES";
						int bytes_written = send_packet(clientComm,fd);
						if(bytes_written < 0){
						cerr<<"Packet not sent"<<endl;
						continue;
						}
						string reply;
						//get response from server
						char res_buff[STR_SIZE];
						clear_buff(res_buff, STR_SIZE);
						int read_status = recv_packet(res_buff,fd);
						reply = res_buff;
						cout<<res_buff<<endl;

					}
				} else if(parse1 == "Request" ){
					ss >> parse1 >> parse2 ;
					clientComm = origin;
					int bytes_written = send_packet(clientComm, fd);
					if(bytes_written < 0){
					cerr<<"Packet not sent"<<endl;
					continue;
					}
					string reply;
					//get response from server
					char res_buff[STR_SIZE];
					clear_buff(res_buff, STR_SIZE);
					int read_status = recv_packet(res_buff, fd);
					reply = res_buff;
					cout<<res_buff<<endl;

				} else if(parse1 == "Send"){
					// SEND THE FILE
				} else if(parse1 == "Append"){
					// Append the FILE
				} else if(parse1 == "Logout"){
					clientComm = origin;
					int bytes_written = send_packet(clientComm,fd);
					if(bytes_written < 0){
					cerr<<"Packet not sent"<<endl;
					continue;
					}
					cout<<"Disconnected"<<endl;
					break;
				}
				else if(parse1=="Show" && parse2=="Candidates")
				{
					
					//send command for server
					int bytes_written = send_packet(origin, fd);
					if(bytes_written < 0){
						cerr<<"Packet not sent"<<endl;
						continue;
					}
					string reply;
					//get response from server
					char res_buff[STR_SIZE];
					clear_buff(res_buff, STR_SIZE);
					int read_status = recv_packet(res_buff, fd);
					reply = res_buff;
					cout<<res_buff<<endl;
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
