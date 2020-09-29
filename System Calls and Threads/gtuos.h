/*
* Author 	: Suleyman Balaban
* ID 		: 121044014
* Lesson	: CSE-312
* Version	: 2
* Date		: 20.04.2018
*/
#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"
#include <cstring>
#include <string>
#include <vector>
#include "memory.h"
#include <cstring>

using namespace std;

#define BCADRESSCONVERTER 256
#define QUANTUM 100
class Thread{
public:
	State8080 registers;
	int threadID;
	int startingCycle;
	int usedCycle;
	string stateOfTheThread;
	int firstStartingTime;
	uint32_t stack;
	int privileged;
	bool isDead;
	int functionPointer;
	void printThread();
	State8080 getState(){return registers;}
	void setState(State8080 state){registers = state;}
};
class GTUOS{
public:
	GTUOS();
	uint64_t handleCall(CPU8080 & cpu);
	uint64_t systemCalls(CPU8080 & cpu, int paramsForCalls);
	void printMemoryToFile(const CPU8080 & cpu);
	int totalCycleNumber;
	int ID;
	int getPC(CPU8080 & cpu);
	void setPC(CPU8080 & cpu, uint16_t pc){cpu.state->pc = pc;}
	void loadStartThread(CPU8080 & cpu);
	int findReadyThread();
	void threadManager();
	bool roundRobinSchedular(CPU8080 & cpu, int previousTotalCycle);
	void threadSwitch(CPU8080 & cpu, int currentThreadIndex);
	bool allThreadDone();
	bool isAnotherThreadAlive(int currentThreadIndex);
	int getIndexWithThreadId(int id);
	bool isEnd(CPU8080 & cpu);
	vector<Thread> threadTable;
	int debug;
};



#endif
