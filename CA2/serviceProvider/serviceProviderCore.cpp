#include "serviceProviderCore.h"

ServiceProviderCore::ServiceProviderCore(int pnum)
{
	string tail="_"+toString(pnum);
	for(unsigned i=0; i<sizeof(localServiceNames)/sizeof(string); ++i)
		localServices.push_back(localServiceNames[i]+tail);
}

ServiceProviderCore::~ServiceProviderCore()
{
	for(unsigned i=0; i<localServices.size(); ++i)
		removeFile(localServices[i]);
}

string ServiceProviderCore::listOfServices()
{
	string result="";
	for(unsigned i=0; i<localServices; ++i)
		result+=(localServices[i]+"\n");
	return result;
}
