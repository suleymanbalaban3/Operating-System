/*
* Author 	: Suleyman Balaban
* ID 		: 121044014
* Lesson	: CSE-312
* Version	: 3
* Date		: 14.06.2018
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
	int bc = ((int)(cpu.state->b) << 8) | ((int)(cpu.state->c));

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
		int i = 0;
		uint16_t address = (cpu.state->b << 8) | (cpu.state->c);
		while(cpu.memory->at(address + i) != '\0'){
			printf("%c", (cpu).memory->at(address + i));
			i++;
		}
		printf("\n");
			//std::cout<<(cpu).memory->at(address + i);
		cycleCount = 100;
	}else if (paramsForCalls == 6){															//READ_STR
		cout<<"________________________ READ_STR __________________________"<<endl;
		string sentence;
		int i = 0;
		cout << "Enter a sentence :";
    	cin>>sentence;
   		//sentence += "\0";
   		uint16_t address = (cpu.state->b << 8) | (cpu.state->c);
		while (i < sentence.size()){
			cpu.memory->at(address + i) = sentence[i];
			i++;
		}
		cpu.memory->at(address + i) = '\0';
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
	}else if(paramsForCalls == 12){															//TCreate
		cout<<"________________________ FILECREATE __________________________"<<endl;
		int j = 0;
		char fileNameTemp[255];
		while (cpu.memory->at(bc + j) != '\0'){
			fileNameTemp[j] = cpu.memory->at(bc + j);
			j++;
		}
		fileNameTemp[j] = '\0';
		File file;

		int k = 0;
		FILE *fin;
		fin=fopen(fileSystem.c_str(),"rb");
		if (!fin){
			puts("Failed to open File System File");
			return 0;
		} 
		for (k=0;;k++){
			File ff;
			int eof = fread(&ff,sizeof(File),1,fin);
			
			if (eof == 0)
				break;
			fileVector.push_back(ff);
		}
		strcpy(file.fileName, fileNameTemp);
		if (isLegalFile(file) == true){
			 
			strcpy(file.data, ""); 
			file.createTime = getTime();
			file.lastModification = getTime();
			file.lastAccess = getTime();
			file.isOpen = 1;
			file.fileSize = 0;
			file.seekNumber = 0;

			fclose(fin);
			fileVector.push_back(file);	


			FILE *fout;
			fout=fopen(fileSystem.c_str(),"wb");
			if (!fout) puts("Failed to open File System File");
			for (j=0;j<fileVector.size();j++)
				fwrite(&fileVector[j],sizeof(File),1,fout);
			fclose(fout);
			//cpu.state->b = 1;
			if (debug ==1){
				for (int i = 0; i < fileVector.size(); ++i)
					toString(fileVector[i]);
			}
		}else
			cpu.state->b = 0;
		
		/*FILE *fin;
		fin=fopen("Phonebook.dat","rb");
		if (!fin) puts("Failed to open File System File");
		cout<<"-----------------reading-------------------"<<endl;
		for (j=0;;j++){
			int eof = fread(&fileVector[j],sizeof(File),1,fin);
			if (eof == 0)
				break;
			toString(fileVector[j]);
		}
		fclose(fin);*/

		cycleCount = 0;
	}else if(paramsForCalls == 13){															//TCreate
		cout<<"________________________ FILECLOSE __________________________"<<endl;
		int fileIndex = (int)(cpu.state->b);
		int j = 0;
		fileIndex--;
		if (fileIndex > fileVector.size() || fileIndex < 1){
			cerr<<"You enter wrong file for closing!!!"<<endl;
			cpu.state->b = 0;
		}
		if (fileVector[fileIndex].isOpen == 0){
			cerr<<"File already closed!!!"<<endl;
			cpu.state->b = 0;
		}else{
			fileVector[fileIndex].lastAccess = getTime();
			cpu.state->b = 1;
			cerr<<"File closing...\n"<<endl;
			fileVector[fileIndex].isOpen = 0;

			if (debug ==1){
				for (int i = 0; i < fileVector.size(); ++i)
					toString(fileVector[i]);
			}else
				toString(fileVector[fileIndex]);
		}
		fileVector[fileIndex].isOpen = 0;
		cout<<endl;
		FILE *fout;
		fout=fopen(fileSystem.c_str(),"wb");
		if (!fout) puts("Failed to open File System File");
		for (j=0;j<fileVector.size();j++)
			fwrite(&fileVector[j],sizeof(File),1,fout);
		fclose(fout);
		cycleCount = 0;
	}else if(paramsForCalls == 14){															//TCreate
		cout<<"________________________ FILEOPEN __________________________"<<endl;
		char filename_[255]; 
		int j = 0;
		int address = ((int)(cpu.state->b) << 8) | ((int)(cpu.state->c));
		while (cpu.memory->at(address + j) != '\0'){
			filename_[j] = cpu.memory->at(address + j);
			j++;
		}
		filename_[j] = '\0';
		if (fileVector.size() > 0)
			fileVector.clear();
		FILE *fin;
		File ff;
		fin=fopen(fileSystem.c_str(),"rb");
		if (!fin) puts("Failed to open File System File");
		for (j=0;;j++){
			int eof = fread(&ff,sizeof(File),1,fin);
			if (eof == 0)
				break;
			fileVector.push_back(ff);
		}
		fclose(fin);
		if (debug ==1){
			for (int i = 0; i < fileVector.size(); ++i)
				toString(fileVector[i]);
		}
		int hand = getHandleId(filename_);
		cpu.state->b = hand+1;

		cycleCount = 0;
	}else if(paramsForCalls == 15){															//TCreate
		cout<<"________________________ FILEREAD __________________________"<<endl;
		int j = 0;
		int numberOfBytes = (int)cpu.state->d;
		int handleId = (int)cpu.state->e;
		/*cout<<"numberOfBytes :" <<numberOfBytes <<endl;
		cout<<"handle id :"<<handleId <<endl;*/
		if (numberOfBytes != 0 && handleId != 0){
			FILE *fin;
			fin=fopen(fileSystem.c_str(),"rb");
			if (!fin) puts("Failed to open File System File");
			for (j=0;;j++){
				int eof = fread(&fileVector[j],sizeof(File),1,fin);
				if (eof == 0)
					break;
			}
			fclose(fin);
			j = 0;
			while (j < numberOfBytes){
				
				cpu.memory->at(bc + j) = fileVector[handleId-1].data[fileVector[handleId-1].seekNumber + j];
				j++;
			}
			cpu.memory->at(bc + j) = '\0';
			fileVector[handleId-1].fileSize = j;
			cpu.state->b = j-1;
			fileVector[handleId-1].lastAccess = getTime();
			fileVector[handleId-1].isOpen = 1;
			if (debug ==1){
				for (int i = 0; i < fileVector.size(); ++i)
					toString(fileVector[i]);
			}else
				toString(fileVector[handleId-1]);
			FILE *fout;
			fout=fopen(fileSystem.c_str(),"wb");
			if (!fout) puts("Failed to open File System File");
			for (j=0;j<fileVector.size();j++)
				fwrite(&fileVector[j],sizeof(File),1,fout);
			fclose(fout);
		}else{
			cerr<<"Couldn't open file!!!"<<endl;
			cpu.state->b = 0;
		}
		cycleCount = 0;
	}else if(paramsForCalls == 16){															//TCreate
		cout<<"________________________ FILEWRITE __________________________"<<endl;
		int j = 0;
		int numberOfBytes = (int)cpu.state->d;
		int handleId = (int)cpu.state->e;
		/*cout<<"numberOfBytes :" <<numberOfBytes <<endl;
		cout<<"handle id :"<<handleId <<endl;*/
		cout<<"seek number :"<<fileVector[handleId-1].seekNumber<<endl;
		if (numberOfBytes != 0 && handleId != 0){
			while (j < numberOfBytes){
				fileVector[handleId-1].data[fileVector[handleId-1].seekNumber + j] = cpu.memory->at(bc + j);
				j++;
			}
			fileVector[handleId-1].data[fileVector[handleId-1].fileSize + j] = '\0';
			fileVector[handleId-1].fileSize += j;
			cpu.state->b = j-1;
			fileVector[handleId-1].lastAccess = getTime();
			fileVector[handleId-1].lastModification = getTime();
			fileVector[handleId-1].isOpen = 1;
			toString(fileVector[handleId-1]);
			FILE *fout;
			fout=fopen(fileSystem.c_str(),"wb");
			if (!fout) puts("Failed to open File System File");
			for (j=0;j<fileVector.size();j++)
				fwrite(&fileVector[j],sizeof(File),1,fout);
			fclose(fout);
			if (debug ==1){
				for (int i = 0; i < fileVector.size(); ++i)
					toString(fileVector[i]);
			}
			
		}else{
			cerr<<"Couldn't write to file!!!"<<endl;
			cpu.state->b = 0;
		}
		/*if(debug != 0)
			toString(fileVector[handleId-1]);*/
		cycleCount = 0;
	}else if(paramsForCalls == 17){															//TCreate
		cout<<"________________________ FILESEEK __________________________"<<endl;
		int seekNumber = (int)cpu.state->e;
		int handleId = (int)cpu.state->d;
		cout<<"handle id :"<<handleId<<endl;
		cout<<"seek number :"<<seekNumber<<endl;
		cout<<"file size :"<<fileVector[handleId-1].fileSize<<endl;
		if (handleId < 0 || handleId > fileVector.size() || seekNumber < 0 || seekNumber > fileVector[handleId-1].fileSize){
			cerr<<"FileSeek Error!!!"<<endl;
			cpu.state->b = 0;
		}else{
			cpu.state->b = 1;
			fileVector[handleId-1].seekNumber = seekNumber;
			FILE *fout;
			int j = 0;
			fout=fopen(fileSystem.c_str(),"wb");
			if (!fout) puts("Failed to open File System File");
			for (j=0;j<fileVector.size();j++)
				fwrite(&fileVector[j],sizeof(File),1,fout);
			fclose(fout);
			if (debug ==1){
				for (int i = 0; i < fileVector.size(); ++i)
					toString(fileVector[i]);
			}
		}
		cycleCount = 0;
	}else if(paramsForCalls == 18){															//TCreate
		cout<<"________________________ DIRREAD __________________________"<<endl;
		FILE *fin;
		int j = 0, i = 0;
		uint16_t address = (cpu.state->b << 8) | (cpu.state->c);
		fin=fopen(fileSystem.c_str(),"rb");
		if (!fin){
			puts("Failed to open File System File");
		}else{
			vector<File> tempFileVector;
			for (j=0;;j++){
				File f;
				int eof = fread(&f,sizeof(File),1,fin);
				if (eof == 0)
					break;
				tempFileVector.push_back(f);
			}
			cout<<"size :"<<tempFileVector.size()<<endl;
			fclose(fin);
			string directoryInformation;
			char str[84];
			directoryInformation = "============ Directory Informations ===========\n";
			for (i = 0; i < tempFileVector.size(); ++i){

				string isOpen;
				if (tempFileVector[i].isOpen == 1)
					isOpen = "True";
				else
					isOpen = "False";
				
				directoryInformation.append("FileName :");
				directoryInformation.append(tempFileVector[i].fileName);
				directoryInformation.append("\nCreateTime :");
				sprintf(str, "%s", asctime (&(tempFileVector[i].createTime)));
				directoryInformation.append(str);
				directoryInformation.append("LastModificationTime :");
				sprintf(str, "%s", asctime (&(tempFileVector[i].lastModification)));
				directoryInformation.append(str);
				directoryInformation.append("LastAccessTime :");
				sprintf(str, "%s", asctime (&(tempFileVector[i].lastAccess)));
				directoryInformation.append(str);
				directoryInformation.append("FileSize :");
				sprintf(str, "%d", tempFileVector[i].fileSize);
				directoryInformation.append(str);
				directoryInformation.append("\nisOpen :");
				directoryInformation.append(isOpen);
				directoryInformation.append("\n");
			}
			//directoryInformation.append("\0");
			j = 0;
		}
		cycleCount = 0;
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
struct tm  GTUOS::getTime(){
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	return *timeinfo;
}
void GTUOS::toString(File file){
	cout<<"File Name :" << file.fileName<<endl;
	/*cout<<"Create Time :" << asctime (&(file.createTime));
	cout<<"Last Modification Time :" << asctime (&(file.lastModification));
	cout<<"Last Access Time :" << asctime (&(file.lastAccess));
	cout<<"File Size :" << file.fileSize<<endl;*/
	cout<<"Data :"<<file.data<<endl;
	if (file.isOpen == 1)
		cout<<"File is open : True"<<endl;
	else
		cout<<"File is open : False"<<endl;
}
bool GTUOS::isLegalFile(File file){

	for (int i = 0; i < fileVector.size(); ++i){
		//cout<<"ilk :"<<fileVector[i].fileName<<" / ikinci "<<file.fileName<<endl;
		if(strcmp(fileVector[i].fileName, file.fileName) == 0)
			return false;
	}
	return true;
}
int GTUOS::getHandleId(char filename[255]){
	for (int i = 0; i < fileVector.size(); ++i){
		//cout<<"ilk :"<<fileVector[i].fileName<<" / ikinci "<<filename<<endl;
		if (strcmp(fileVector[i].fileName, filename) == 0)
			return i;
	}
	return -1;
}
void GTUOS::directoryInformations(CPU8080 & cpu){
	FILE *fin;
	int j = 0, i = 0;
	int bc = ((int)(cpu.state->b) << 8) | ((int)(cpu.state->c));
	fin=fopen(fileSystem.c_str(),"rb");
	if (!fin){
		puts("Failed to open File System File");
	}else{
		vector<File> v;
		File f;
		for (j=0;;j++){
			int eof = fread(&f,sizeof(File),1,fin);
			if (eof == 0)
				break;
			v.push_back(f);
		}
		fclose(fin);
		string directoryInformation;
		char str[84];
		directoryInformation = "============ Directory Informations ===========\n";
		for (i = 0; i < v.size(); ++i){

			string isOpen;
			if (v[i].isOpen == 1)
				isOpen = "True";
			else
				isOpen = "False";
			
			directoryInformation.append("FileName :");
			directoryInformation.append(v[i].fileName);
			directoryInformation.append("\nCreateTime :");
			sprintf(str, "%s", asctime (&(v[i].createTime)));
			directoryInformation.append(str);
			directoryInformation.append("LastModificationTime :");
			sprintf(str, "%s", asctime (&(v[i].lastModification)));
			directoryInformation.append(str);
			directoryInformation.append("LastAccessTime :");
			sprintf(str, "%s", asctime (&(v[i].lastAccess)));
			directoryInformation.append(str);
			directoryInformation.append("FileSize :");
			sprintf(str, "%d", v[i].fileSize);
			directoryInformation.append(str);
			directoryInformation.append("\nisOpen :");
			directoryInformation.append(isOpen);
			directoryInformation.append("\nData :");
			directoryInformation.append(v[i].data);
			directoryInformation.append("\n\n");
		}
		j = 0;
		while (directoryInformation[j] != '\0'){
			printf("%c", directoryInformation[j]);
			j++;
		}
	}
}
