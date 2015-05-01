#ifndef __SIAMOZ__switchCore
#define __SIAMOZ__switchCore

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <sstream>

using namespace std;

class SwitchCoreClass{
	public:
	SwitchCoreClass();	
	void doServerCommand();
	void doClientCommand(string comm);

};


#endif
