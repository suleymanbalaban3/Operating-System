#include <iostream>
#include <time.h>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"

using namespace std;
	// This is just a sample main function, you should rewrite this file to handle problems 
	// with new multitasking and virtual memory additions.
int main (int argc, char**argv)
{
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1); 
	}
	string junk;
	int i = 0;
	int cycleCount = 0;

	srand(time(0));
	int DEBUG = atoi(argv[2]);
	
	memory mem;
	CPU8080 theCPU(&mem);
	GTUOS	theOS;

	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);	
 	cout<<"------------------------ System Calls ----------------------------"<<endl;
	do	
	{
		cycleCount += theCPU.Emulate8080p(DEBUG);
		if (DEBUG == 2)
			cin >> junk;
		if(theCPU.isSystemCall())
			cycleCount += theOS.handleCall(theCPU);
	}while (!theCPU.isHalted());
	cout<<endl<<"------------------------ System Halted ----------------------------"<<endl;
	cout<<"All cycle count is "<<cycleCount<<endl;
	theOS.printMemoryToFile(theCPU);
	return 0;
}

