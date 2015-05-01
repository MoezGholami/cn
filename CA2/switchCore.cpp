#include "switchCore.h"

SwichCoreClerk::SwitchCoreClerk(){}

void SwitchCoreClerk::doServerCommand(){
	string comm1,comm2;
	cin>>comm1;
	int portNu;
	if(comm1 == "Connect"){
		cin>>comm2;
		if(comm2 == "Switch"){
			cin>>portNu;
		}
	}

}

void SwitchCoreClerk::doClientCommand(string comm){
	
}
