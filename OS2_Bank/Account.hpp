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

extern class LogFile logy;

class Account{
    int id_;
    int password_;
    int balance_;
    int num_readers;
    
    pthread_mutex_t mutex_readers;
    pthread_mutex_t mutex_writers;
    

    public:
      //make private later
    
    
    //static int jobCount;
    Account(int ATM, int id, int password, int balance);
    
    ~Account();
    
    //void moneyTransfer(int ATM, int password, int amount, bool transfer);
    
    void cashDeposit(int ATM, int password, int amount);
    
    void cashWithdrawl(int ATM, int password, int amount);
    
    void checkBalance(int ATM, int password);
    
    int takeCommision(double rate, int percentage);
    
    void printAccount();
        
    int getID() const;
    int getPassword() const;
    int getBalance() const;
    void setBalance(int transferAmount);
    
    void lock_readers();
    
    void unlock_readers();
    
    void lock_writers();
    
    void unlock_writers();
    
};


#endif /* Account_hpp */
