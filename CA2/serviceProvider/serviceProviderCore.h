#ifndef __siamoz_serviceProviderCore
#define __siamoz_serviceProviderCore

#include <string>
#include <vector>
#include "../util/util.h"

using namespace std;

string localServiceNames[]={"shangul", "mangul", "habbe", "angur"};

class ServiceProviderCore
{
	public:
		ServiceProviderCore(int pnum);
		~ServiceProviderCore();

		string listOfServices();
	protected:
		int portNum;
		vector<string> localServices;
};

#endif
