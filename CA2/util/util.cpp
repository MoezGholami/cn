#include "util.h"

string toString(int a)
{
	stringstream ss;
	ss << a;
	return ss.str();
}

int removeFile(const string &url)
{
	return system(("rm -f "+url).c_str());
}

string wholeAsciiFile(const string &url)
{
	ifstream t(url.c_str());
	return string((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
}

void write2File_entire(const string &url, const string &s)
{
	ofstream out(url.c_str());
	out<<s;
	out.close();
}

void append2File(const string &url, const string &s)
{
	ofstream fout;
	fout.open(url.c_str(), ios_base::app);
	fout<<endl<<s;
	fout.close();
}
