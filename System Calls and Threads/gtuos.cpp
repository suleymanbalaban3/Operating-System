/*
* Author 	: Suleyman Balaban
* ID 		: 121044014
* Lesson	: CSE-312
* Version	: 2
* Date		: 20.04.2018
*/
#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"

GTUOS::GTUOS(){
	ID = 0;
	totalCycleNumber = 0;
}
uint64_t GTUOS::handleCall(CPU8080 & cpu){
	return (systemCalls(cpu, (int)(cpu.state->a)));		
}
uint64_t GTUOS::systemCalls(CPU8080 & cpu, int paramsForCalls){
	int cycleCount = 0;
	int bc = ((int)(cpu.state->b) << 8) + ((int)(cpu.state->c));

	if(paramsForCalls == 1){																//PRINT_B
		cout<<"________________________ PRINT_B ____________________________"<<endl;
		cout<<(int)(cpu.state->b)<<endl;
		cycleCount = 10;
	}else if (paramsForCalls == 2){															//PRINT_MEM
		cout<<"________________________ PRINT_MEM __________________________"<<endl;
		cout<<(int)(cpu.memory->at(bc))<<endl;
		cycleCount = 10;
	}else if (paramsForCalls == 3){															//READ_B
		cout<<"________________________ READ_B __________________________"<<endl;
		int integer, count;
		cout<<"Please enter an integer :";
		do{
	        count = 0;
	        cin>>integer;
	        if(cin.fail()) {
	            count = 1;
	            cin.clear();
	            cin.ignore(80,'\n');
	            cout<<"You entered wrong type input, enter again :";
	        }	        
    	}while(count ==1);
		cpu.state->b = integer;
		cycleCount = 10;
	}else if (paramsForCalls == 4){															//READ_MEM
		cout<<"________________________ READ_MEM __________________________"<<endl;
		int integer, count;
		cout<<"Please enter an integer :";
		do{
	        count = 0;
	        cin>>integer;
	        if(cin.fail()) {
	            count = 1;
	            cin.clear();
	            cin.ignore(80,'\n');
	            cout<<"You entered wrong type input, enter again :";
	        }	        
    	}while(count ==1);
		cpu.memory->at(((cpu.state->b) << 8) + (cpu.state->c)) = integer;
		cycleCount = 10;
	}else if (paramsForCalls == 5){															//PRINT_STR
		cout<<"________________________ PRINT_STR __________________________"<<endl;
		for (; cpu.memory->at(((cpu.state->b) << 8) + (cpu.state->c)) != '\0'; (cpu.state->c)++)
			std::cout<<(cpu).memory->at(((cpu.state->b) << 8) + (cpu.state->c));
		cycleCount = 100;
	}else if (paramsForCalls == 6){															//READ_STR
		cout<<"________________________ READ_STR __________________________"<<endl;
		string sentence;
		cout << "Enter a sentence :";
    	cin.get();
   		getline(cin, sentence);
   		sentence += "\n\0";
		for (int i = 0; i < sentence.size(); ++i, (cpu.state->c)++)
			cpu.memory->at(((cpu.state->b) << 8) + (cpu.state->c)) = sentence[i];
		cycleCount = 100;	
	}else if (paramsForCalls == 7){															//GET_RND
		cout<<"________________________ GET_RND __________________________"<<endl;
		uint8_t randomNumber = rand() / 256;
		cout<<"Generated random bytes :";
		for(int i = 0x80;i != 0; i >>= 1){
			if(randomNumber & i)
				cout<<"1";
			else
				cout<<"0";
		}
		cout<<endl;
        //printf("%c",(a&i)?'1':'0'); 
		cpu.state->b = randomNumber;
		cycleCount = 5;
	}else if(paramsForCalls == 8){															//TExit
		cout<<"________________________ TEXIT __________________________"<<endl;
		threadTable[findReadyThread()].isDead = true;
		threadTable[findReadyThread()].stateOfTheThread = "Blocked";
		cycleCount = 50;
	}else if(paramsForCalls == 9){															//TJoin
		cout<<"________________________ TJOIN __________________________"<<endl;

		int joinThreadId = getIndexWithThreadId(cpu.state->b);
		threadTable[joinThreadId].privileged = true;
		cycleCount = 40;
	}else if(paramsForCalls == 10){															//TYield
		cout<<"________________________ TYIELD __________________________"<<endl;
		threadSwitch(cpu, findReadyThread());
		cycleCount = 40;
	}else if(paramsForCalls == 11){															//TCreate
		cout<<"________________________ TCREATE __________________________"<<endl;
		Thread thread;
		thread.threadID = ++(this->ID);
		thread.usedCycle = 80;
		thread.isDead = false;
		thread.privileged = false;
		thread.stateOfTheThread = "Ready";
		thread.startingCycle = this->totalCycleNumber;
		thread.firstStartingTime = thread.startingCycle;
		thread.registers = *(cpu.state);
		thread.functionPointer = bc;
		threadTable.push_back(thread);
		thread.printThread();
		cycleCount = 80;
	}else
		cout<<"___!________________!_____ WRONG CALL ____!___________________!___"<<endl;
	return cycleCount;
}
void GTUOS::printMemoryToFile(const CPU8080 & cpu){
	int i = 0;
	FILE * outer;

	outer = fopen("exe.mem","w");
	if (outer == NULL){
		fprintf(stderr, "File could not open!!\n");
		return;
	}
	while(i < 65536){
		fprintf(outer, "%04x ", cpu.memory->at(i));
		if ((i+1)%16 == 0)
			fprintf(outer, "\n");
		i++;
	}
	fclose(outer);
}
int GTUOS::getPC(CPU8080 & cpu){
	return cpu.state->pc;
}
void Thread::printThread(){
	cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
	cout<<"ThreadID :"<<threadID<<endl;
	cout<<"stateOfTheThread :"<<stateOfTheThread<<endl;
	cout<<"startingCycle :"<<firstStartingTime<<endl;
	cout<<"Switch occur count :"<<usedCycle / QUANTUM<<endl;
	cout<<"Thread is Alive? \n -";
	if (isDead)
		cout<<"NO"<<endl;
	else
		cout<<"YES"<<endl;
	cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
}
bool GTUOS::roundRobinSchedular(CPU8080 & cpu, int previousTotalCycle){
	int runningThreadIndex = findReadyThread();

	if (runningThreadIndex != -1){
		int prev = threadTable[runningThreadIndex].usedCycle / QUANTUM;
		threadTable[runningThreadIndex].usedCycle += totalCycleNumber - previousTotalCycle;/* code */
		int after = threadTable[runningThreadIndex].usedCycle / QUANTUM;
		bool isAnotherThreadLiving = isAnotherThreadAlive(runningThreadIndex);
		if (after > prev && threadTable[runningThreadIndex].privileged == false && isAnotherThreadLiving == true){
			if(threadTable.size() != 1)
				threadSwitch(cpu, runningThreadIndex);
		}	
	}else{
		return false;
	}
	return true;
}
bool GTUOS::allThreadDone(){
	for (int i = 0; i < threadTable.size(); ++i)
		if (threadTable[i].isDead == false)
			return false;
	return true;
}
void GTUOS::loadStartThread(CPU8080 & cpu){
	cout<<"Loading first thread..."<<endl;
	*(cpu.state) = threadTable[0].getState();
	cpu.state->pc = threadTable[0].functionPointer;
}
void GTUOS::threadSwitch(CPU8080 & cpu, int currentThreadIndex){	
	for (int i = currentThreadIndex; i < currentThreadIndex + threadTable.size(); ++i){
		if (threadTable[(i + 1)% threadTable.size()].isDead == false){
			threadTable[i].setState(*(cpu.state));
			threadTable[i].functionPointer = cpu.state->pc;
			*(cpu.state) = threadTable[(i + 1)% threadTable.size()].getState();
			threadTable[i].stateOfTheThread = "Blocked";
			threadTable[(i + 1)% threadTable.size()].startingCycle = totalCycleNumber;
			threadTable[(i + 1)% threadTable.size()].stateOfTheThread = "Running";
			cpu.state->pc = threadTable[(i + 1)% threadTable.size()].functionPointer;
			if (debug == 2){
				cout<<"Blocked thread ID :"<< threadTable[i].threadID << " and it's used cycle :"<<threadTable[i].usedCycle<<endl; 
				cout<<"Switch the another thread, it's ID :"<<threadTable[(i + 1)% threadTable.size()].threadID<<endl; 
			}else if (debug == 3)
			{
				cout<<"Cpu cycle time :"<<totalCycleNumber<<endl;
				threadTable[i].printThread();
			}
			break;
		}
	}
}
int GTUOS::findReadyThread(){
	if (threadTable.size() == 0)
		return -1;
	for (int i = 0; i < threadTable.size(); ++i)
		if (threadTable[i].stateOfTheThread == "Running")
			return i;
	return 0;
}
bool GTUOS::isAnotherThreadAlive(int currentThreadIndex){
	for (int i = 0; i < threadTable.size(); ++i)
		if (threadTable[i].isDead == false && i != currentThreadIndex)
			return true;
	return false;
}
int GTUOS::getIndexWithThreadId(int id){
	for (int i = 0; i < threadTable.size(); ++i)
		if (threadTable[i].threadID == id)
			return i;
	return -1;
}	