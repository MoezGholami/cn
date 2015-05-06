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
	cout<<"size of packet (in mem management): "<<sizeof(Packet)<<endl;
	Macaddr a("192:168:0:0:0:1");
	Macaddr b("192:168:1:0:0:1");
	if(a==b)
		cout<<"mosavi\n";
	vector<Packet> packs=ph.packetVectorOfMessage(Message("salami cho buye khoshe ashenaayiiiiiiiiiiiii", a, b, 0));
	ofstream fout("temp.txt");
	for(int i=0; i<(int)packs.size(); ++i)
		fout.write((const char *)&(packs[i]), sizeof(Packet));
	fout.close();
	ifstream fin("temp.txt");
	Packet p;
	return 0;
}
