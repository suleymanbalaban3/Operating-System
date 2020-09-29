/*
* Author 	: Suleyman Balaban
* ID 		: 121044014
* Lesson	: CSE-312
* Version	: 2
* Date		: 20.04.2018
*/
#include <iostream>
#include <time.h>
#include "8080emuCPP.h"
#include "gtuos.h"


using namespace std;
	// This is just a sample main function, you should rewrite this file to handle problems 
	// with new multitasking and virtual memory additions.


int main (int argc, char**argv){
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1); 
	}
	int cycleCount = 0;
	int previousTotalCycle = 0;
	srand(time(0));
	int DEBUG = atoi(argv[2]);
	
	memory mem;
	CPU8080 theCPU(&mem);
	GTUOS	theOS;
	theOS.debug = DEBUG;
	theCPU.ReadFileIntoMemoryAt(argv[1], 0);	
	
 	cout<<"------------------------ System Calls ----------------------------"<<endl;
	do	
	{
		theOS.totalCycleNumber += theCPU.Emulate8080p(0);
		if(theCPU.isSystemCall()){
			theOS.totalCycleNumber += theOS.handleCall(theCPU);
			
		}
		previousTotalCycle = theOS.totalCycleNumber;
	}while (!theCPU.isHalted());
	if (theOS.threadTable.size()!=0){
	
		theOS.totalCycleNumber = 0;
		previousTotalCycle = 0;
		theOS.loadStartThread(theCPU);
		do	
		{
			int currentThreadIndex = theOS.findReadyThread();
			if (DEBUG == 4){
				cout<<"\n=================== Running thread ID :"<<theOS.threadTable[currentThreadIndex].threadID;
				cout<<"  Cpu cycle :" <<theOS.totalCycleNumber<<" =================="<<endl;
			}
			
			theOS.totalCycleNumber += theCPU.Emulate8080p(0);		//istediginiz debug modu kendim olusturdum

			if(theCPU.isSystemCall()){
				if (DEBUG != 4){
					cout<<"\n=================== Running thread ID :"<<theOS.threadTable[currentThreadIndex].threadID;
					cout<<"  Cpu cycle :" <<theOS.totalCycleNumber<<" =================="<<endl;
				}
				
				theOS.totalCycleNumber += theOS.handleCall(theCPU);
				
			}
			if (theOS.threadTable[currentThreadIndex].isDead == true){
				theOS.threadSwitch(theCPU, theOS.findReadyThread());
			}else{		
				if(theCPU.isHalted()){
					
					theOS.threadTable[currentThreadIndex].isDead = true;
					if (theOS.allThreadDone())				
						break;	
					theOS.threadSwitch(theCPU, theOS.findReadyThread());			
				}else if(theOS.isAnotherThreadAlive(currentThreadIndex)){
					theOS.roundRobinSchedular(theCPU, previousTotalCycle);
				}
			}
			previousTotalCycle = theOS.totalCycleNumber;
		}while (!theOS.allThreadDone());
	}
	cout<<endl<<"!!!!!!!!!!!!!!!!!!!!!!!!!! System Halted !!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	cout<<"All cycle count is "<<theOS.totalCycleNumber<<endl<<endl;
	cout<<"++++++++++++ Thread Table's final situation +++++++++++++"<<endl;
	for (int i = 0; i < theOS.threadTable.size(); ++i)
		theOS.threadTable[i].printThread();
	
	if (DEBUG == 0 || DEBUG == 1)
		theOS.printMemoryToFile(theCPU);
	cout<<"##################################### FINISH ###################################"<<endl<<endl;
	return 0;
}