//
//  main.cpp
//  OS2_Bank
//
//  Created by Elior on 03/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include <iostream>
#include "Bank.cpp"
#include "log_file.hpp"
#include <cmath>
#include <string.h>
#include <cstring>
#include <string>
#include <fstream>
#include <pthread.h>


#define MAX_ARG 20
#define MAX_LINE_SIZE 80
#define MAXARGS 20
#define MAXHISTORY 50

//char *args[MAX_ARG];
char cmdString[MAX_LINE_SIZE];
char lineSize[MAX_LINE_SIZE];

//std::string filename = "atm1.txt" ;
char liney[MAX_LINE_SIZE];

const char* delimiters = " \t"; // make sure if 't' or ' '

class LogFile logy;
Bank Lloyds(15);
int i = 0, num_arg = 1;
bool atmThreadsWorking=true;
#include <vector>
#include <chrono>
#include <thread>
std::vector<std::string> readFileToVector(const std::string& filename){
    std::ifstream source;
    source.open(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(source, line))
    {
        lines.push_back(line);
    }
    return lines;
}

string lineRead;

void readMe(int ATM, const char* filename){
    std::ifstream instructionFile;
    instructionFile.open(filename);
    while (std::getline(instructionFile, lineRead)){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        strcpy(cmdString, lineRead.c_str() );
        cmdString[strlen(lineRead.c_str())]='\0';
        char* cmd;
        char* args[MAX_ARG];
        //int* parameters[MAX_ARG];
        num_arg = 0;
        cmd = strtok(cmdString, delimiters);
        if (cmd == NULL)
            return;
        args[0] = cmd;
        
        for (i=1; i<MAX_ARG; i++){
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL)
                num_arg++;
        }
       
        cout << cmd << " with parameters are: ";
        for (int j=1; j<num_arg+1; j++){
            cout << args[j] << " ";
        }
        
        if (*cmd == 'O' && num_arg ==  3) {
            Lloyds.openAccount(ATM, stoi(args[1]),stoi(args[2]), stoi(args[3]));
        }
        else if(*cmd == 'D' && num_arg ==  3){
            Lloyds.cashDeposit(ATM, stoi(args[1]),stoi(args[2]), stoi(args[3]));
        }
        else if(*cmd == 'W' && num_arg ==  3){
            Lloyds.cashWithdrawl(ATM, stoi(args[1]),stoi(args[2]), stoi(args[3]));
        }
        else if(*cmd == 'B' && num_arg ==  2){
            Lloyds.cashBalance(ATM, stoi(args[1]),stoi(args[2]));
        }
        else if(*cmd == 'Q' && num_arg ==  2){
            Lloyds.closeAccount(ATM, stoi(args[1]),stoi(args[2]));
        }
        else if(*cmd == 'T' && num_arg ==  4){
            Lloyds.moneyTransfer(ATM, stoi(args[1]),stoi(args[2]), stoi(args[3]), stoi(args[4]));
        }
        
        
        lineSize[0] = '\0';
        cmdString[0] ='\0';
        cout << endl;
    }
    
    instructionFile.close();
}

void testOpenAccounts(){
    Lloyds.openAccount(12, 1234, 1111, 23);
    Lloyds.openAccount(12, 900, 1111, 23);
    Lloyds.openAccount(12, 2004, 1111, 23);
    Lloyds.openAccount(12, 1870, 1111, 23);
    Lloyds.openAccount(12, 123, 1111, 23);
    Lloyds.openAccount(12, 50004, 1111, 23);
}

void checkInputArguments(int argc, const char * argv[]){
    bool illegalArguments=false;
    std::ifstream testInputFile;
    int numATM=stoi(argv[1]);
    if ((numATM != (argc - 2) ) || numATM <= 0)
        illegalArguments=true;
    
    for (int k=2; k<argc; k++){
        testInputFile.open(argv[k]);
        if (testInputFile.fail() == true){
            illegalArguments=true;
            break;
        }
        testInputFile.close();
    }
    if (illegalArguments){
        cout << "illegal arguments" << endl;
        exit(1);
    }
}

struct atmThreadData {
   int  ATM_id;
   const char* instructionFile;
};


void* ATMRoutine(void* arg){
    atmThreadData atd = *(atmThreadData*)(arg);
    int ATM_Id= atd.ATM_id;
    std::string filename = atd.instructionFile;
    readMe(ATM_Id, filename.c_str());
    pthread_exit(NULL);
}

void* commissionRoutine(void* arg){
    while(atmThreadsWorking){
        sleep(3);
        Lloyds.takeComission();
    }
    pthread_exit(NULL);
}

void* accountsStatusRoutine(void* arg){
    while(atmThreadsWorking){
        sleep(0.5);
        Lloyds.accountsStatus();
    }
    pthread_exit(NULL);
}

// declare threads

// WHAT if an acount starts with 0? e.g. 00123?  00123/123 ?
int main(int argc, const char * argv[]) {
    
    // Check programme's input arguments
    checkInputArguments(argc, argv);
    Bank halifax(15);
    pthread_t* threads = new pthread_t[argc];
    struct atmThreadData* atd = new atmThreadData[argc-2];
    //pthread_create (thread, attr, start_routine, arg)
    
    if(pthread_create(&threads[0], NULL, commissionRoutine, NULL) != 0){
        perror("Fail to create commission thread!");
        exit(1);
    }
    if(pthread_create(&threads[1], NULL, accountsStatusRoutine , NULL) !=0 ){
        perror("Fail to create status thread!");
        exit(1);
    }
    
    for (i = 0; i < argc-2; i++){
        atd[i].ATM_id = i+1;
        atd[i].instructionFile=argv[i+2];
        if(pthread_create(&threads[i], NULL, ATMRoutine, (void *)&atd[i] ) ){
            perror("Fail to create thread!");
            exit(1);
        }
    }
    void* status;
    
    for( i = 2; i < argc; i++ ) {
        if( pthread_join(threads[i], &status) ){
            perror("Fail to join ATM threads!");
            exit(1);
        }
    }
    atmThreadsWorking=false;
    if( pthread_join(threads[0], &status) ){
        perror("Fail to join commission thread!");
        exit(1);
    }
    
    if( pthread_join(threads[1], &status) ){
        perror("Fail to join accounts' status threads!");
        exit(1);
    }
    delete [] threads;
    /*
     Checklist:
     close files
     delete memory allocations
     destroy objects
     */
    
    
    return 0;
}


/*
cout << "ATM1 reading instrucions: " << endl;
readMe(1, argv[2]);
cout << endl;
cout << "ATM2 reading instrucions: " << endl;
readMe(2, argv[3]);
cout << endl;
cout << "ATM3 reading instrucions: " << endl;
readMe(3, argv[4]);
cout << "Finished reading" << endl;
*/


/*
 while(1 && num_arg!= 0){
       fgets(lineSize, MAX_LINE_SIZE, stdin); //lineSize is our variable
       strcpy(cmdString, lineSize);
       cmdString[strlen(lineSize)-1]='\0';
       char* cmd;
       char* args[MAX_ARG];
       num_arg = 0;
       cmd = strtok(cmdString, delimiters);
       if (cmd == NULL)
           return 0;
       args[0] = cmd;
       for (i=1; i<MAX_ARG; i++){
           args[i] = strtok(NULL, delimiters);
           if (args[i] != NULL)
               num_arg++;
       }
       cout << "command is: " << cmd << " num args: " << num_arg << " now the parameters are:  ";
       for (int j=1; j<num_arg+1; j++){
           cout << args[j] << " ";
       }
       lineSize[0] = '\0';
       cmdString[0] ='\0';
       cout << endl;
   }
 */



/*
 Lloyds.openAccount(12, 1234, 1111, 23);
 Lloyds.openAccount(12, 1234, 1111, 0);
 
 //Lloyds.closeAccount(13, 1234, 1110);
 //Lloyds.closeAccount(14, 1234, 1111);
 Lloyds.moneyTransfer(12, 1321, 2222, 3214, 30);  // no source
 Lloyds.moneyTransfer(12, 1234, 2222, 3214, 30); // no password
 Lloyds.moneyTransfer(12, 1234, 1111, 3214, 30); // no money
 Lloyds.moneyTransfer(12, 1234, 1111, 3214, 10); // no target
 Lloyds.openAccount(12, 3214, 1111, 25);
 
 Lloyds.moneyTransfer(12, 1234, 1111, 3214, 10); // should work
 
 cout << "Pause " << endl;

 printf("\033[2J");
 printf("\033[1;1H");
 for (std::vector<Account>::iterator it=Lloyds.accountVector.begin(); it !=Lloyds.accountVector.end(); ++it){
     cout << it->getID() << endl;
 }
 sleep(3);
 printf("\033[2J");
 printf("\033[1;1H");
 cout << " mahakti" << endl;
 for (std::vector<Account>::iterator it=Lloyds.accountVector.begin(); it !=Lloyds.accountVector.end(); ++it){
     cout << it->getID() << endl;
 }
 
 //char nextPwd[MAX_LINE_SIZE] ="";
 //char oldPwd[MAX_LINE_SIZE] ="";
 //char tmpPwd[MAX_LINE_SIZE] ="";
 //char dash[] = "-";
 */


    
    //ifstream
    //getline(input, )
        
    
    
    /*
     int ExeCmd(char* lineSize, char* cmdString){
        char* cmd;
        char* args[MAX_ARG];
        //char pwd[MAX_LINE_SIZE];
        //char* delimiters = " \t\n";
        int i = 0, num_arg = 0;
        bool illegal_cmd = false; // illegal command
            cmd = strtok(lineSize, delimiters);
        if (cmd == NULL)
            return 0;
        args[0] = cmd;
        for (i=1; i<MAX_ARG; i++)
        {
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL)
                num_arg++;
     
        }
     */
    
    

    
    //join threads
    
    
    //free memory + last prints
    
