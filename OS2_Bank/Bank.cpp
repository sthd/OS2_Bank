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
            // logy.lock_log_file();
            //
            // <ATM ID>: Account <id> new balance is <bal> after <amount> $ was deposited
            Account tmp = Account(ATM, accountId, password, amount);
            accountVector.push_back(tmp);
            //logy.unlock_log_file();
        }
        else{
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account with the same id exists" << endl;
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
                    logy.out << ATM << ": Account " << accountId << " is now closed. Balance was " << it->getBalance() << endl;
                    accountVector.erase(it);
                    logy.unlock_log_file();
                    unlock_writers();
                    return;
                }
                else{
                    logy.lock_log_file();
                    logy.out << "Error " << ATM << ": Your transaction failed – password for account id " << accountId << " is incorrect" << endl;
                    logy.unlock_log_file();
                    unlock_writers();
                    return;
                }
            }
        }
        logy.lock_log_file();
        logy.out << "Error " << ATM << ": Your transaction failed – account id " << accountId << " does not exist" << endl;
        logy.unlock_log_file();
        unlock_writers();
    }
    
    void cashDeposit(int ATM, int accountId, int password, int amount){
        lock_readers();
        Account* tmp= findAccount(accountId);
        if (tmp==NULL){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << accountId << " does not exist" << endl;
            logy.unlock_log_file();
        }
        else{
            tmp->cashDeposit(ATM, password, amount);
        }
        unlock_readers();
    }
    
    void cashWithdrawl(int ATM, int accountId, int password, int amount){
        lock_readers();
        Account* tmp= findAccount(accountId);
        if (tmp==NULL){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << accountId << " does not exist" << endl;
            logy.unlock_log_file();
        }
        else{
            tmp->cashWithdrawl(ATM, password, amount);
        }
        unlock_readers();
    }
    
    
    void moneyTransfer(int ATM, int sourceAccountId, int password, int targetAccountId, int amount){
        lock_readers();
        int minId;
        int maxId;
        bool sourceIsMin=false;
        bool targetExist=true;
        Account* maxAccount;
        Account* sourceAccount;
        Account* targetAccount;
        
        if (sourceAccountId < targetAccountId){
            minId = sourceAccountId;
            maxId = targetAccountId;
            sourceIsMin = true;
        }
        else {
             minId = targetAccountId;
             maxId = sourceAccountId;
         }
        Account* minAccount = findAccount(minId);
        if (minAccount == NULL){
            maxAccount = findAccount(maxId);
            if (maxAccount == NULL){
                logy.lock_log_file();
                logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
                logy.unlock_log_file();
                unlock_readers();
                return;
            }
            maxAccount->lock_writers();
            if (sourceIsMin){
                logy.lock_log_file();
                logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
                logy.unlock_log_file();
                maxAccount->unlock_writers();
                unlock_readers();
                return;
            }
            targetExist=false;
        }
        else{
            minAccount->lock_writers();
            maxAccount = findAccount(maxId);
            if (maxAccount == NULL){
                if (sourceIsMin == false){
                    logy.lock_log_file();
                    logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
                    logy.unlock_log_file();
                    minAccount->unlock_writers();
                    unlock_readers();
                    return;
                }
                targetExist=false;
            }
            else{
                maxAccount->lock_writers();
            }
        }
        if (sourceIsMin){
            sourceAccount = minAccount;
            targetAccount = maxAccount;
        }
        else{
            sourceAccount = maxAccount;
            targetAccount = minAccount;
        }
        
        if (sourceAccount->getPassword() != password){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – password for account id " << sourceAccountId << " is incorrect" << endl;
            logy.unlock_log_file();
            if (targetExist){
                targetAccount->unlock_writers();
            }
            sourceAccount->unlock_writers();
            unlock_readers();
            return;
        }
        
        if (sourceAccount->getBalance() < amount){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " balance is lower than " << amount << endl;
            logy.unlock_log_file();
            if (targetExist){
                targetAccount->unlock_writers();
            }
            sourceAccount->unlock_writers();
            unlock_readers();
            return;
        }
        
        if (targetExist == false){
            logy.lock_log_file();
            logy.out << "Error " << ATM << ": Your transaction failed – account id " << targetAccountId << " does not exist" << endl;
            logy.unlock_log_file();
            sourceAccount->unlock_writers();
            unlock_readers();
            return;
        }
        
        sourceAccount->setBalance(-amount);
        targetAccount->setBalance(amount);
        logy.lock_log_file();
        logy.out << ATM << ": Transfer " << amount << " from account " << sourceAccountId << " to account " << targetAccountId << " new account balance is " << sourceAccount->getBalance() << " new target account balance is " << targetAccount->getBalance() << endl;
        logy.unlock_log_file();
        minAccount->unlock_writers();
        maxAccount->unlock_writers();
        unlock_readers();
        return;
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






/*
 void moneyTransfer(int ATM, int sourceAccountId, int password, int targetAccountId, int amount){
     lock_readers();
     int minId;
     int maxId;
     bool sourceIsMin=false;
     if (sourceAccountId < targetAccountId){
         minId = sourceAccountId;
         maxId = targetAccountId;
         sourceIsMin = true;
         
     }
     else {
         minId = targetAccountId;
         maxId = sourceAccountId;
     }
     Account* minAccount = findAccount(minId);
     if (minAccount == NULL) {
         if (minId==sourceAccountId){
             logy.lock_log_file();
             logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
             logy.unlock_log_file();
             unlock_readers();
             return;
         }
         //minId doesn't exist & minId = tagret
         Account* maxAccount = findAccount(maxId);
         if (maxAccount == NULL){
             logy.lock_log_file();
             logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
             logy.unlock_log_file();
             unlock_readers();
             return;
         }
         maxAccount->lock_writers();
         if (maxAccount->getPassword() != password){
             logy.lock_log_file();
             logy.out << "Error " << endl; //password
             logy.unlock_log_file();
             maxAccount->unlock_writers();
             unlock_readers();
             return;
         }
         if (maxAccount->getBalance() < amount){
             logy.lock_log_file();
             logy.out << "Error " << endl; // no money balance
             logy.unlock_log_file();
             maxAccount->unlock_writers();
             unlock_readers();
             return;
         }
         logy.lock_log_file();
         logy.out << "Error " << ATM  << endl; // no target account
         logy.unlock_log_file();
         maxAccount->unlock_writers();
         unlock_readers();
         return;
     }
     
     minAccount->lock_writers(); // minAccount isn't NULL
     Account* maxAccount = findAccount(maxId);
     if (maxAccount == NULL){
         if (maxId == sourceAccountId){
             logy.lock_log_file();
             logy.out << "Error " << ATM << ": Your transaction failed – account id " << sourceAccountId << " does not exist" << endl;
             logy.unlock_log_file();
             minAccount->unlock_writers();
             unlock_readers();
             return;
         }
         // source < tagret
         if (minAccount->getPassword() != password){
             logy.lock_log_file();
             logy.out << "Error " << endl; //password
             logy.unlock_log_file();
             minAccount->unlock_writers();
             unlock_readers();
             return;
         }
         
         if (minAccount->getBalance() < amount){
             logy.lock_log_file();
             logy.out << "Error " << endl; // no money balance
             logy.unlock_log_file();
             minAccount->unlock_writers();
             unlock_readers();
             return;
         }
         logy.lock_log_file();
         logy.out << "Error " << ATM  << endl; // no target account
         logy.unlock_log_file();
         minAccount->unlock_writers();
         unlock_readers();
         return;
     }
     
     if (sourceIsMin){
         
         minAccount->setBalance(-amount);
     }
     
 }
 */
