#ifndef __siamoz_serviceProviderCore
#define __siamoz_serviceProviderCore

#include <string>
#include <vector>
#include <sstream>
#include "../util/util.h"

using namespace std;

class ServiceProviderCore
{
	public:
		ServiceProviderCore(int pnum);
		~ServiceProviderCore();

		string doCommand(const string &command);
	protected:
		int portNum;
		vector<string> localServices;

		string listOfServices();
		string readFile(const string &fname);
		string resultOfAppendRequest(const string &command);
		string resultOfSendReques(const string &command);
};

#endif
