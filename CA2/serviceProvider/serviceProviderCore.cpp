#include "serviceProviderCore.h"

string localServiceNames[]={"shangul", "mangul", "habbe", "angur"};

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

string ServiceProviderCore::doCommand(const string &command)
{
	stringstream ss;
	string parse;

	ss<<command;
	ss>>parse;

	cout<<"command:\n"<<command<<"\n\n";

	if(parse=="Get")
		return listOfServices();
	if(parse=="read")
		return readFile(command);
	if(parse=="Append")
		return resultOfAppendRequest(command);
	if(parse=="Send")
		return resultOfSendReques(command);
	cout<<"no valid command got from server. command =" << command<<endl;
	return "no valid command.";
}

string ServiceProviderCore::listOfServices()
{
	string result="";
	for(unsigned i=0; i<localServices.size(); ++i)
		result+=(localServices[i]+"\n");
	return result;
}

string ServiceProviderCore::readFile(const string &command)
{
	string parse, fname;
	stringstream ss;

	ss<<command;
	ss>>parse>>fname;

	if(find(localServices.begin(), localServices.end(), fname)==localServices.end())
		return "no such service.";
	if(!FileExist(fname))
		return "::the file is empty::";
	return wholeAsciiFile(fname);
}

string ServiceProviderCore::resultOfAppendRequest(const string &command)
{
	string parse, fname;
	stringstream ss;

	ss<<command;
	ss>>parse>>fname>>parse;

	append2File(fname, parse);
	return "successfully appended.";
}

string ServiceProviderCore::resultOfSendReques(const string &command)
{
	string fname;
	stringstream ss;

	ss<<command;
	ss>>fname>>fname;

	write2File_entire(fname, command);
	deleteFirstLineOfFile(fname);
	return "successfully replaced.";
}
