//
//  Bank.cpp
//  OS2_Bank
//
//  Created by Elior on 06/01/2022.
//  Copyright © 2022 Elior. All rights reserved.
//

#include "Bank.hpp"
#include "Account.hpp"
#include "log_file.hpp"

#include <vector>
#include <pthread.h>

extern class LogFile logy;

using std::vector;


class Bank{
    int numATM_;
    int balance_;
    int numReaders_;
    pthread_mutex_t mutex_readers;
    pthread_mutex_t mutex_writers;
    pthread_t print;
    pthread_t commision;
    pthread_t* ATM_list;
    vector<Account> accountVector;
    
    
    public:
    // explicit
    Bank(int numATM) : numATM_(numATM), balance_(0), numReaders_(0){
        pthread_mutex_init(&mutex_readers, nullptr);
        pthread_mutex_init(&mutex_writers, nullptr);
        ATM_list = new pthread_t[numATM_];
        }

    ~Bank(){
        delete [] ATM_list;
        pthread_mutex_destroy(&mutex_readers);
        pthread_mutex_destroy(&mutex_writers);

    }
    
    void openAccount(int ATM, int accountId, int password, int amount){
        lock_writers();
        sleep(1);
        if (findAccount(accountId) == NULL){
            logy.lock_log_file();
            //
            // <ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited
            Account tmp = Account(ATM, accountId, password, amount);
            accountVector.push_back(tmp);
            logy.unlock_log_file();
        }
        else{
            logy.lock_log_file();
            //
            // Error <ATM ID>: Your transaction failed – account with the same id exists
            logy.out << "Error " << ATM << " : Your transaction failed – account with the same id exists" << endl;
            logy.unlock_log_file();
        }
        unlock_writers();
    }
    // ###################### when do we minimise ?????
    void closeAccount(int ATM, int accountId, int password){
        lock_writers();
        sleep(1);
        for (std::vector<Account>::iterator it=accountVector.begin(); it !=accountVector.end(); ++it){
            if (it->getID() == accountId){
                if (it->getPassword() == password){
                    logy.lock_log_file();
                    accountVector.erase(it);
                    logy.unlock_log_file();
                    unlock_writers();
                    return;
                }
                else{
                    logy.lock_log_file();
                    //
                    // Error <ATM ID>: Your transaction failed – password for account id <id> is incorrect
                    logy.unlock_log_file();
                    unlock_writers();
                    return;
                }
            }
            logy.lock_log_file();
            //
            // Error <ATM ID>: Your transaction failed – account id <id> does not exist
            logy.unlock_log_file();
        unlock_writers();
        }
    }
    
    void cashDeposit(int ATM, int accountId, int password, int amount){
        lock_readers();
        Account* tmp= findAccount(accountId);
        if (tmp==NULL){
            logy.lock_log_file();
            //
            // Error <ATM ID>: Your transaction failed – account id <id> does not exist
            logy.unlock_log_file();
        }
        else{
            tmp->cashDeposit(ATM, password, amount, false);
        }
        unlock_readers();
    }
    
    void cashWithdrawl(int ATM, int accountId, int password, int amount){
        lock_readers();
        Account* tmp= findAccount(accountId);
        if (tmp==NULL){
            logy.lock_log_file();
            //
            // Error <ATM ID>: Your transaction failed – account id <id> does not exist
            logy.unlock_log_file();
        }
        else{
            tmp->cashWithdrawl(ATM, password, amount, false);
        }
        unlock_readers();
    }
    
    void moneyTransfer(int ATM, int sourceAcountId, int password, int targetAccountId, int amount){
        lock_readers();
        Account* sourceAccount = findAccount(sourceAcountId);
        if (sourceAccount == NULL){
            logy.lock_log_file();
            //
            //  no source
            logy.unlock_log_file();
            unlock_readers();
            return;
        }
        Account* targetAccount = findAccount(targetAccountId);
        if (targetAccount == NULL){
            logy.lock_log_file();
            //
            //  no target
            logy.unlock_log_file();
            unlock_readers();
            return;
        }
        if (sourceAccount->cashWithdrawl(ATM, password, amount, false) == true){
            targetAccount->cashDeposit(ATM, password, amount, false);
        }
            
        // Attention: prints should be different as successful withdrawl/deposit print
        
        
        
        
    }
    
    
    
    void lock_readers() {
        pthread_mutex_lock(&mutex_readers);
         numReaders_++;
        if (numReaders_ == 1){
            pthread_mutex_lock(&mutex_writers);
        }
        pthread_mutex_unlock(&mutex_readers);
    }

    void unlock_readers(){
        pthread_mutex_lock(&mutex_readers);
        numReaders_--;
        if ( numReaders_ == 0)
            pthread_mutex_unlock(&mutex_writers);
        pthread_mutex_unlock(&mutex_readers);
    }

    void lock_writers() {pthread_mutex_lock(&mutex_writers);}

    void unlock_writers() {pthread_mutex_unlock(&mutex_writers);}

    Account* findAccount(int accountId){
        //modifyJobList();
        for (std::vector<Account>::iterator it=accountVector.begin(); it !=accountVector.end(); ++it){
            if (it->getID() == accountId){
                return &(*it);
            }
        }
        return NULL;
    }
    
};
