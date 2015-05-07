#ifndef __siamoz_cn_util
#define __siamoz_cn_util

#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>

using namespace std;

string toString(int a);
int removeFile(const string &url);

string wholeAsciiFile(const string &url);
void write2File_entire(const string &url, const string &s);
void append2File(const string &url, const string &s);

#endif
