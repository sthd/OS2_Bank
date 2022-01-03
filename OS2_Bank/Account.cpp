//
//  ATM.cpp
//  OS2_Bank
//
//  Created by Elior on 03/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include "Account.hpp"

#include <iostream>
#include <unistd.h>


#define MAX_ARG 20
#define MAX_LINE_SIZE 80


char lineSize[MAX_LINE_SIZE];
char cmdString[MAX_LINE_SIZE];


const char* delimiters = " \t\n";

bool lockLofFile; // 1 lock shared with everyone (bank account, programme, atm, accounts)

bool lock_readAccount;
bool lock_writeAccount;


class ATM{
    int instructionsForATM;
    
};

int readFileForATM(){
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
    
    if (!strcmp(cmd, "O") ){
        if (num_arg == 0){
            //cerr << "Not enough parameters!" << endl;
            printf("smash error: > \"%s\"\n", cmdString);
            return 1;
        }
        else if(num_arg > 1){
            //cerr << "Too many parameters!" << endl;
            printf("smash error: > \"%s\"\n", cmdString);
            return 1;
        }
    }
    return 0;
}







class Account{
    int id_;
    int password_;
    int balance_;

    public:
    static int jobCount;
    Account(int id, int password, int balance) : id_(id), password_(password), balance_(balance){}
    
    ~Account(){}
    
    int cashDeposit(int password, int amount){
        if (password==password_){
            lock_writeAccount=true;
            
        }
            
        
        
    }
    
    int cashWithdrawl(int withdrawl){
        if (this->balance_ > )
        return command_;
    }
    
    int getBalance(){return balance_;}
    
    int takeCommision(){}
    
    void printAccount(){
    }
        
    
    int getID(){return id_;}
    int getPassword(){return password_;}
    int getBalance(){return balance_;}
        

}



