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
#include <vector>
#include <chrono>
#include <thread>

#define MAX_ARG 20
#define MAX_LINE_SIZE 80

const char* delimiters = " \t\n"; // make sure if 't' or ' '
bool atmThreadsWorking=true;
class LogFile logy;
Bank Lloyds(15);


void readMe(int ATM, const char* filename){
    std::ifstream instructionFile;
    instructionFile.open(filename);
    int num_arg=0;
    string lineRead;
    
    
    //
    while (std::getline(instructionFile, lineRead)){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        char* cmd=NULL;
        char* tmp=NULL;
        //char* args[MAX_ARG];
        int intArg[MAX_ARG];
        num_arg=0;
        
        
        char* cmdString =new char[lineRead.length() +1];
        if (cmdString == NULL){
            instructionFile.close();
            return;
        }
        strcpy(cmdString, lineRead.c_str() );
        cmd = strtok(cmdString, delimiters);
        
        if (cmd == NULL){
            delete [] cmdString;
            instructionFile.close();
            return;
        }
        
        for (int i=0; i<MAX_ARG; i++){
            tmp = strtok(NULL, delimiters);
            if (tmp != NULL){
                num_arg++;
                intArg[i]=stoi(tmp);
            }
        }
        
        if (*cmd == 'O' && num_arg ==  3) {
            Lloyds.openAccount(ATM, intArg[0], intArg[1], intArg[2]);
        }
        else if(*cmd == 'D' && num_arg ==  3){
            Lloyds.cashDeposit(ATM, intArg[0], intArg[1], intArg[2]);
        }
        else if(*cmd == 'W' && num_arg ==  3){
            Lloyds.cashWithdrawl(ATM, intArg[0], intArg[1], intArg[2]);
        }
        else if(*cmd == 'B' && num_arg ==  2){
            Lloyds.cashBalance(ATM, intArg[0], intArg[1]);
        }
        else if(*cmd == 'Q' && num_arg ==  2){
            Lloyds.closeAccount(ATM, intArg[0], intArg[1]);
        }
        else if(*cmd == 'T' && num_arg ==  4){
            Lloyds.moneyTransfer(ATM, intArg[0], intArg[1], intArg[2], intArg[3]);
        }
        delete [] cmdString;
    }
    instructionFile.close();
}



void readMeOLD(int ATM, const char* filename){
    std::ifstream instructionFile;
    instructionFile.open(filename);
    int num_arg;
    string lineRead;
    int li=0;
    char cmdString[MAX_LINE_SIZE];
    string pr;
    string pre;
    //instructionFile.eof();
    while (std::getline(instructionFile, lineRead)){
        li++;
        //cout << "I am atm: " << ATM << " and reading line: " << li << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        strcpy(cmdString, lineRead.c_str() );
        cmdString[strlen(lineRead.c_str())]='\0';
        char* cmd;
        char* args[MAX_ARG];
        num_arg = 0;
        cmd = strtok(cmdString, delimiters);
        if (cmd == NULL){
            instructionFile.close();
            return;
        }
        args[0] = cmd;
        for (int i=1; i<MAX_ARG; i++){
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL)
                num_arg++;
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
        cmdString[0] ='\0';
    }
    instructionFile.close();
}


void checkInputArguments(int argc, const char * argv[]){
    bool illegalArguments=false;
    std::ifstream testInputFile;
    int numATM=stoi(argv[1]);
    if ((numATM != (argc - 2) ) || numATM <= 0)
        illegalArguments=true;
    
    for (int i=2; i<argc; i++){
        testInputFile.open(argv[i]);
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
        Lloyds.takeCommission();
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


int main(int argc, const char * argv[]) {
    
    // Check programme's input arguments
    checkInputArguments(argc, argv);
    
    pthread_t* threads = new pthread_t[argc];
    struct atmThreadData* atd = new atmThreadData[argc-2];
    //pthread_create (thread, attr, start_routine, arg)
    
    if(pthread_create(&threads[0], NULL, commissionRoutine, NULL)){
        perror("Fail to create commission thread!");
        exit(1);
    }
    if(pthread_create(&threads[1], NULL, accountsStatusRoutine , NULL)){
        perror("Fail to create status thread!");
        exit(1);
    }
    
    for (int i=0; i < argc-2; i++){
        atd[i].ATM_id = i+1;
        atd[i].instructionFile=argv[i+2];
        if(pthread_create(&threads[i+2], NULL, ATMRoutine, (void *)&atd[i] ) ){
            perror("Fail to create thread!");
            exit(1);
        }
    }
    void* status;
    
    for(int i=2; i < argc; i++) {
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
    Lloyds.freeAccounts();
    delete [] threads;
    delete [] atd;
   
    /*
     Checklist:
     close files
     delete memory allocations
     destroy objects
     */
    
    return 0;
}
