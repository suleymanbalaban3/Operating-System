#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"
#include <cstring>
#include <string>

using namespace std;

#define BCADRESSCONVERTER 256

class GTUOS{
	public:
		uint64_t handleCall(const CPU8080 & cpu);
		uint64_t systemCalls(const CPU8080 & cpu, int paramsForCalls);
		void printMemoryToFile(const CPU8080 & cpu);
		
		
};

#endif
