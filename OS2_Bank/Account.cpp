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


pthread_mutex_t mutex_log_file{};

void lock_log_file() {pthread_mutex_lock(&mutex_log_file);}

void unlock_log_file() {pthread_mutex_unlock(&mutex_log_file);}



class Account{
    int id_;
    int password_;
    int balance_;
    int num_readers;
    
    pthread_mutex_t mutex_readers{};
    pthread_mutex_t mutex_writers{};
    

    public:
    static int jobCount;
    Account(int id, int password, int balance) : id_(id), password_(password), balance_(balance), num_readers(0){
        pthread_mutex_init(&mutex_readers, nullptr);
        pthread_mutex_init(&mutex_writers, nullptr);
        
        lock_log_file();
        //write into logfile
        // <ATM ID>: New account id is <id> with password <pass> and initial balance <bal>
        unlock_log_file();
    }
    
    ~Account(){
        pthread_mutex_destroy(&mutex_readers);
        pthread_mutex_destroy(&mutex_writers);
    }
    
    void cashDeposit(int ATM, int password, int amount){
        lock_writers();
        sleep(1);
        if (password==password_){
            balance_+=amount;
            lock_log_file();
            //write into logfile
            // <ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited
            unlock_log_file();
        }
        else{
            lock_log_file();
            //write into logfile
            // Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
            unlock_log_file();
        }
        unlock_writers();
    }
    
    bool cashWithdrawl(int ATM, int password, int amount){
        lock_writers();
        sleep(1);
        bool success = false;
        if (password==password_){
            if (balance_ >= amount){
                balance_ -= amount;
                lock_log_file();
                //write into logfile
                // <ATM ID>: Account <id> new balance is <bal> after <amount> $ was withdrew
                unlock_log_file();
                success=true;
            }
            else{
                lock_log_file();
                //write into logfile
                // <ATM ID>: Your transaction failed - account id <id> is lower than <amount>
                unlock_log_file();
            }
        }
        else{
            lock_log_file();
            //write into logfile
            // Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
            unlock_log_file();
        }
        unlock_writers();
        return success;
    }
    
    void checkBalance(int ATM, int password){
       lock_readers();
       sleep(1);
       if (password==password_){
           lock_log_file();
           //write into logfile
           // <ATM ID>: Account <id> balance is <bal>
           unlock_log_file();
       }
       else{
           lock_log_file();
           //write into logfile
           // Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
           unlock_log_file();
       }
       unlock_readers();
    }
    
    int takeCommision(double rate){
        lock_writers();
        sleep(1);
        int amount = int(balance_ * rate);
        balance_-=amount;
        lock_log_file();
        // write into logfile
        // Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
        unlock_log_file();
        unlock_writers();
        return amount;
    }
    
    
    
    
    
    void printAccount(){
    }
        
    int getID(){return id_;}
    int getPassword(){return password_;}
    int getBalance(){return balance_;}
    
    void lock_readers() {
        pthread_mutex_lock(&mutex_readers);
        num_readers++;
        if (num_readers == 1){
            pthread_mutex_lock(&mutex_writers);
        }
        pthread_mutex_unlock(&mutex_readers);
    }
    
    
    void unlock_readers(){
        pthread_mutex_lock(&mutex_readers);
        num_readers--;
        if (num_readers == 0)
            pthread_mutex_unlock(&mutex_writers);
        pthread_mutex_unlock(&mutex_readers);
    }
    
    void lock_writers() {pthread_mutex_lock(&mutex_writers);}
    
    void unlock_writers() {pthread_mutex_unlock(&mutex_writers);}
    

};






/*

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

 */
