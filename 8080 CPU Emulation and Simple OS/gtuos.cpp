#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"

uint64_t GTUOS::handleCall(const CPU8080 & cpu){
	return (systemCalls(cpu, (int)(cpu.state->a)));
}
uint64_t GTUOS::systemCalls(const CPU8080 & cpu, int paramsForCalls){
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
	}else
		cout<<"___!_____________________ WRONG CALL ____!___________________!___"<<endl;
	if (cycleCount == 0)
		throw -1;	
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