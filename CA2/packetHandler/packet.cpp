#include "packet.h"

Macaddr::Macaddr(uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4, uint8_t a5, uint8_t a6)
{
	bytes[0]=a1;
	bytes[1]=a2;
	bytes[2]=a3;
	bytes[3]=a4;
	bytes[4]=a5;
	bytes[5]=a6;
}

Macaddr::Macaddr(string s)
{
	stringstream ss;
	uint8_t temp;
	replace(s.begin(), s.end(), ':', ' ');
	ss<<s;
	for(unsigned i=0; i<6; ++i)
	{
		ss>>temp;
		bytes[i]=temp;
	}
}

Macaddr::Macaddr()
{
	for(unsigned i=0; i<6; ++i)
		bytes[i]=0;
}

inline bool operator==(const Macaddr &a, const Macaddr &b)
{
	for(unsigned i=0; i<6; ++i)
		if(a.bytes[i]!=b.bytes[i])
			return false;
	return true;
}

inline bool operator!=(const Macaddr &a, const Macaddr &b)
{
	return !(a==b);
}