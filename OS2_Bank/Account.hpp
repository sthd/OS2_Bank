//
//  Account.hpp
//  OS2_Bank
//
//  Created by Elior on 03/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#ifndef Account_hpp
#define Account_hpp

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "log_file.hpp"
#include "ATM.hpp"


class Account{
    int id_;
    int password_;
    int balance_;
    int num_readers;
    
    pthread_mutex_t mutex_readers;
    pthread_mutex_t mutex_writers;
    

    public:
    //static int jobCount;
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


#endif /* Account_hpp */
