#include "packet.h"
#include "packetHandler.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	PacketHandler ph;
	Packet p;
	cout<<"size of packet (in mem management): "<<sizeof(Packet)<<endl;
	Macaddr a("192:168:0:0:0:1");
	Macaddr b("192:168:1:0:0:1");

	cout<<"a:"<<a<<endl;

	if(a==b)
		cout<<"mosavi\n";
	vector<Packet> packs=ph.packetVectorOfMessage(Message("ssssssssssssssssssssssss", a, b, 255));
	p=packs[0];

	cout<<"p type: "<<(unsigned) p.type<<endl;
	cout<<"p saddr: "<<p.saddr<<endl;
	cout<<"p saddr: "<<p.daddr<<endl;

	ofstream fout("temp.txt");
	for(int i=0; i<(int)packs.size(); ++i)
		fout.write((const char *)&(packs[i]), sizeof(Packet));
	fout.close();
	ifstream fin("temp.txt");
	Message *m;
	for(int i=0; i<2; ++i)
	{
		fin.read((char*)&p, sizeof(Packet));
		m=ph.messageOfPackets(p);
		if(m==0)
			cout<<"nul bud.\n";
	}
	if(m==0)
		return -1;
	cout<<"payam:\n"<<m->value<<endl<<endl;
	fin.close();
	return 0;
}
